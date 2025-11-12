#define IMGUI_DEFINE_MATH_OPERATORS

#include <GLFW/glfw3.h>
#include <bitset>

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/misc/cpp/imgui_stdlib.h"

#include "imgui_spectrum.hh"

#include "api.hh"
#include "save_file.hh"

static std::string schedule_result_text;

// Basic table info
constexpr float NAME_COLUMN_WIDTH = 256;
constexpr float BOX_COLUMN_WIDTH = 32;
constexpr float SINGLE_LETTER_DROPDOWN_COLUMN_WIDTH = 72;

// Distance between windows and the app edge and each other.
constexpr float PANEL_LAYOUT_PADDING = 64;

// How many windows there are being divided into the app window
constexpr int NUM_PANEL_COLUMNS = 2;

constexpr float BG_COLOR_R = 0.122f * 0.75;
constexpr float BG_COLOR_G = 0.149f * 0.75;
constexpr float BG_COLOR_B = .184f * 0.75;

constexpr uint8_t TITLEBAR_COLOR_R = BG_COLOR_R * 2 * 255;
constexpr uint8_t TITLEBAR_COLOR_G = BG_COLOR_G * 2 * 255;
constexpr uint8_t TITLEBAR_COLOR_B = BG_COLOR_B * 2 * 255;

// active info that is correlated to the process and the active glfw window
namespace cache {
    // For scaling contents
    static ImVec2 work_pos;
    static ImVec2 panel_size;

    // Etc
    int win_size_x = 0;
    int win_size_y = 0;

    // call each frame
    static void update_state() {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        work_pos = viewport->WorkPos;

        win_size_x = viewport->WorkSize.x;
        win_size_y = viewport->WorkSize.y;

        const float pad = PANEL_LAYOUT_PADDING;
        const float total_w = win_size_x - (2.0f * pad); // exclude left+right padding
        const float total_h = win_size_y - (2.0f * pad); // exclude top+bottom padding

        const float column_width = (total_w - (NUM_PANEL_COLUMNS - 1) * pad) / (float)NUM_PANEL_COLUMNS;

        panel_size = ImVec2(column_width, total_h);
    }

    // Based on column index, return where the window should be located. Size should just equate to panel_size
    static inline ImVec2 get_panel_pos(const int column_index) {
        return ImVec2(PANEL_LAYOUT_PADDING + column_index * (cache::panel_size.x + PANEL_LAYOUT_PADDING) + cache::work_pos.x, PANEL_LAYOUT_PADDING + cache::work_pos.y);
    }
};

constexpr ImGuiWindowFlags WINDOW_FLAGS = ImGuiWindowFlags_None
    | ImGuiWindowFlags_NoMove 
    | ImGuiWindowFlags_NoResize
    | ImGuiWindowFlags_NoCollapse
;

constexpr ImGuiTableFlags TABLE_FLAGS = ImGuiTableFlags_None
    | ImGuiTableFlags_BordersV
;

static GLFWwindow* init_glfw() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return nullptr;
    }

    GLFWwindow* window = glfwCreateWindow(1000, 618, "Staff Scheduler", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return nullptr;
    }

    glfwSetWindowSizeLimits(window, 1000, 618, GLFW_DONT_CARE, GLFW_DONT_CARE);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    return window;
}

static ImGuiIO& init_imgui(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    io.IniFilename = nullptr;

    themes::spectrum_dark::load();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    return io;
}

static void entry_window(staff_session& session) {
    ImGui::SetNextWindowPos(cache::get_panel_pos(0));
    ImGui::SetNextWindowSize(cache::panel_size);
    if (!ImGui::Begin("People", NULL, WINDOW_FLAGS)) {
        ImGui::End();
        return;
    }

start_staff:

    if (!ImGui::CollapsingHeader("Staff")) {
        goto start_students;
    }
    
    if (!ImGui::BeginTable("##Staff", 3, TABLE_FLAGS)) {
        ImGui::EndTable();
        ImGui::End();
        return;
    }

    {
        const ImGuiStyle style = ImGui::GetStyle();

        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch, NAME_COLUMN_WIDTH);
        ImGui::TableSetupColumn("A Day", ImGuiTableColumnFlags_WidthFixed, BOX_COLUMN_WIDTH * 3 + (style.ItemSpacing.x * 2));
        ImGui::TableSetupColumn("B Day", ImGuiTableColumnFlags_WidthFixed, BOX_COLUMN_WIDTH * 3 + (style.ItemSpacing.x * 2));
    }

    ImGui::TableHeadersRow();

    ImGui::TableNextColumn();

    for (int entry_id = 0; entry_id < session.state.staff_list.size(); entry_id++) {
        staff_entry& entry = session.state.staff_list[entry_id];

        ImGui::TableSetColumnIndex(0);
        ImGui::SetNextItemWidth(ImGui::GetColumnWidth());
        ImGui::InputText((std::string("##Name") + std::to_string(entry_id)).c_str(), &entry.name, 0, 0);

        remove_delim_in_string(entry.name);

        if (ImGui::BeginPopupContextItem((std::string("##Context") + std::to_string(entry_id)).c_str())) {
            if (ImGui::MenuItem("Remove")) {
                session.remove_staff(entry_id);
                ImGui::EndPopup();
                break;
            }

            ImGui::EndPopup();
        }

        for (t_period_index_base i = 0; i < (t_period_index_base)e_period_index::_MAX; i++) {
            if (i % 3 == 0)
                ImGui::TableNextColumn();
            else
                ImGui::SameLine();

            e_period_index as_enum = (e_period_index)i;
            f_period_index as_flag = enum_to_flag<e_period_index, f_period_index>(as_enum);

            bool active = flag_match(entry.availability, as_flag);

            ImGui::SetNextItemWidth(BOX_COLUMN_WIDTH);
            if (ImGui::Checkbox((std::string("##Period") + std::to_string(entry_id) + std::to_string(i)).c_str(), &active)) {
                if (active)
                    entry.availability = entry.availability | as_flag;
                else
                    entry.availability = entry.availability & ~as_flag;
            }
        }

        if (entry_id + 1 != session.state.staff_list.size())
            ImGui::TableNextRow();
    }

    ImGui::EndTable();

    if (ImGui::Button("Add Staff")) {
        session.add_staff("New Staff", f_period_index::AA);
    }

start_students:

    ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0, 0, 0, 0));
    ImGui::Separator();
    ImGui::PopStyleColor();

    static const char* dropdown_options[] = {
        "A",
        "B",
        "C"    
    };

    if (!ImGui::CollapsingHeader("Students")) {
        goto end_window;
    }
    
    if (!ImGui::BeginTable("##Students", 3, TABLE_FLAGS)) {
        ImGui::EndTable();
        ImGui::End();
        return;
    }

    ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch, NAME_COLUMN_WIDTH);
    ImGui::TableSetupColumn("A Day", ImGuiTableColumnFlags_WidthFixed, SINGLE_LETTER_DROPDOWN_COLUMN_WIDTH);
    ImGui::TableSetupColumn("B Day", ImGuiTableColumnFlags_WidthFixed, SINGLE_LETTER_DROPDOWN_COLUMN_WIDTH);

    ImGui::TableHeadersRow();

    ImGui::TableNextColumn();

    for (int entry_id = 0; entry_id < session.state.student_list.size(); entry_id++) {
        student_entry& entry = session.state.student_list[entry_id];

        ImGui::TableSetColumnIndex(0);
        ImGui::SetNextItemWidth(ImGui::GetColumnWidth());
        ImGui::InputText((std::string("##Name") + std::to_string(entry_id)).c_str(), &entry.name, 0, 0);

        remove_delim_in_string(entry.name);
       
        if (ImGui::BeginPopupContextItem((std::string("##Context") + std::to_string(entry_id)).c_str())) {
            if (ImGui::MenuItem("Remove")) {
                session.remove_student(entry_id);
                ImGui::EndPopup();
                break;
            }

            ImGui::EndPopup();
        }

        int a_value = (int)entry.a_day_lunch;
        ImGui::TableNextColumn();
        ImGui::SetNextItemWidth(ImGui::GetColumnWidth());
        if (ImGui::Combo(
            (std::string("##A Day") + std::to_string(entry_id)).c_str(),
            &a_value,
            dropdown_options,
            IM_ARRAYSIZE(dropdown_options)
        )) {
            entry.a_day_lunch = (e_lunch_period)a_value;
        }

        int b_value = (int)entry.b_day_lunch;
        ImGui::TableNextColumn();
        ImGui::SetNextItemWidth(ImGui::GetColumnWidth());
        if (ImGui::Combo(
            (std::string("##B Day") + std::to_string(entry_id)).c_str(),
            &b_value,
            dropdown_options,
            IM_ARRAYSIZE(dropdown_options)
        )) {
            entry.b_day_lunch = (e_lunch_period)b_value;
        }

        if (entry_id + 1 != session.state.student_list.size())
            ImGui::TableNextRow();
    }

    ImGui::EndTable();

    if (ImGui::Button("Add Student"))
        session.add_student("New Student", e_lunch_period::A, e_lunch_period::A);

end_window:

    ImGui::End();
}

static void schedule_window(staff_session& session) {
    ImGui::SetNextWindowPos(cache::get_panel_pos(1));
    ImGui::SetNextWindowSize(cache::panel_size);

    if (!ImGui::Begin("Schedule", NULL, WINDOW_FLAGS)) {
        ImGui::End();
        return;
    }

    if (ImGui::Button("Export")) {
        file_save_info info = request_save_file();

        if (info.saved == true) {
            session.export_csv(info.file_path);
        }
    }

    schedule_result_text.clear();
    schedule_result_text = session.print_distribution_export(session.generate_distribution_export()).str();

    ImGui::Text(schedule_result_text.c_str());

    ImGui::End();
}

int main() {
    GLFWwindow* window = init_glfw();

    if (window == nullptr)
        return 1;

    ImGuiIO& io = init_imgui(window);

    staff_session session("staffsave.ls");

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        cache::update_state();

        entry_window(session);
        schedule_window(session);

        ImGui::Render();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(BG_COLOR_R, BG_COLOR_G, BG_COLOR_B, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    session.state.save();

    // ImGui_ImplOpenGL3_Shutdown();
    // ImGui_ImplGlfw_Shutdown();
    // ImGui::DestroyContext();
    // ^^^^ app is exiting anyways, memory will be reclaimed

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
