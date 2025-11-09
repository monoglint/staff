#define IMGUI_DEFINE_MATH_OPERATORS

#include <GLFW/glfw3.h>
#include <iostream>

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/misc/cpp/imgui_stdlib.h"

#include "imgui_spectrum.hh"

#include "api.hh"

static std::string schedule_result_text;

constexpr float NAME_COLUMN_WIDTH = 256;
constexpr float ACTIONS_COLUMN_WIDTH = 71;
constexpr float BOX_COLUMN_WIDTH = 32;
constexpr float SINGLE_LETTER_DROPDOWN_COLUMN_WIDTH = 72;

constexpr ImVec2 TOPBAR_PADDING(16, 12);
constexpr float WINDOW_LAYOUT_PADDING = 16;

ImVec2 work_pos;
ImVec2 window_size;
float viewport_width;

constexpr ImGuiWindowFlags WINDOW_FLAGS =  ImGuiWindowFlags_None
    | ImGuiWindowFlags_NoMove 
    | ImGuiWindowFlags_NoResize 
    | ImGuiWindowFlags_NoCollapse;

static GLFWwindow* init_glfw() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return nullptr;
    }

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Staff Scheduler", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    return window;
}

static ImGuiIO& init_imgui(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    io.IniFilename = "";
    
    ImGui::Spectrum::StyleColorsSpectrum();
    ImGui::Spectrum::LoadFont(16);

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    return io;
}

static void faculty_window(staff_session& session) {
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, TOPBAR_PADDING);

    ImGui::SetNextWindowPos(work_pos + ImVec2(WINDOW_LAYOUT_PADDING + 0 * (window_size.x + WINDOW_LAYOUT_PADDING), WINDOW_LAYOUT_PADDING));
    ImGui::SetNextWindowSize(window_size);
    if (!ImGui::Begin("Faculty List", NULL, WINDOW_FLAGS)) {
        ImGui::End();
        ImGui::PopStyleVar();
        return;
    }

    ImGui::PopStyleVar();
    
    if (!ImGui::BeginTable("##Staff", 7, ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders)) {
        ImGui::EndTable();
        ImGui::End();
        return;
    }

    ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch, NAME_COLUMN_WIDTH);
    ImGui::TableSetupColumn("AA", ImGuiTableColumnFlags_WidthFixed, BOX_COLUMN_WIDTH);
    ImGui::TableSetupColumn("AB", ImGuiTableColumnFlags_WidthFixed, BOX_COLUMN_WIDTH);
    ImGui::TableSetupColumn("AC", ImGuiTableColumnFlags_WidthFixed, BOX_COLUMN_WIDTH);
    ImGui::TableSetupColumn("BA", ImGuiTableColumnFlags_WidthFixed, BOX_COLUMN_WIDTH);
    ImGui::TableSetupColumn("BB", ImGuiTableColumnFlags_WidthFixed, BOX_COLUMN_WIDTH);
    ImGui::TableSetupColumn("BC", ImGuiTableColumnFlags_WidthFixed, BOX_COLUMN_WIDTH);

    ImGui::TableHeadersRow();

    ImGui::TableNextColumn();

    for (int entry_id = 0; entry_id < session.state.staff_list.size(); entry_id++) {
        staff_entry& entry = session.state.staff_list[entry_id];

        ImGui::TableSetColumnIndex(0);
        ImGui::SetNextItemWidth(ImGui::GetColumnWidth());
        ImGui::InputText((std::string("##Name") + std::to_string(entry_id)).c_str(), &entry.name, 0, 0);

        if (ImGui::BeginPopupContextItem((std::string("##Context") + std::to_string(entry_id)).c_str())) {
            if (ImGui::MenuItem("Remove")) {
                session.remove_staff(entry_id);
                ImGui::EndPopup();
                break;
            }

            ImGui::EndPopup();
        }

        for (t_period_index_base i = 0; i < (t_period_index_base)e_period_index::_MAX; i++) {
            ImGui::TableNextColumn();

            e_period_index as_enum = (e_period_index)i;
            f_period_index as_flag = enum_to_flag<e_period_index, f_period_index>(as_enum);

            bool active = flag_match(entry.availability, as_flag);

            ImGui::SetNextItemWidth(ImGui::GetColumnWidth());
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

    if (ImGui::Button("Add")) {
        session.add_staff("Click to Add Name", f_period_index::AA);
    }

    ImGui::End();
}

static void students_window(staff_session& session) {
    static const char* dropdown_options[] = {
        "A",
        "B",
        "C"    
    };

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, TOPBAR_PADDING);

    ImGui::SetNextWindowPos(work_pos + ImVec2(WINDOW_LAYOUT_PADDING + 1 * (window_size.x + WINDOW_LAYOUT_PADDING), WINDOW_LAYOUT_PADDING));
    ImGui::SetNextWindowSize(window_size);

    if (!ImGui::Begin("Student List", NULL, WINDOW_FLAGS)) {
        ImGui::PopStyleVar();
        ImGui::End();
        return;
    }

    ImGui::PopStyleVar();
    
    if (!ImGui::BeginTable("##Students", 3, ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders)) {
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

    if (ImGui::Button("Add"))
        session.add_student("Click to Add Name", e_lunch_period::A, e_lunch_period::A);

    ImGui::End();
}

static void schedule_window(staff_session& session) {
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, TOPBAR_PADDING);

    ImGui::SetNextWindowPos(work_pos + ImVec2(WINDOW_LAYOUT_PADDING + 2 * (window_size.x + WINDOW_LAYOUT_PADDING), WINDOW_LAYOUT_PADDING));
    ImGui::SetNextWindowSize(window_size);

    if (!ImGui::Begin("Schedule", NULL, WINDOW_FLAGS)) {
        ImGui::PopStyleVar();
        ImGui::End();
        return;
    }

    ImGui::PopStyleVar();

    if (ImGui::Button("Create Schedule")) {
        schedule_result_text.clear();
        schedule_result_text = session.print_distribution_export(session.generate_distribution_export()).str();
    }

    ImGui::Text(schedule_result_text.c_str());

    ImGui::End();
}

int main() {
    GLFWwindow* window = init_glfw();

    if (window == nullptr)
        return 1;

    ImGuiIO& io = init_imgui(window);

    staff_session session("staffsave.ls"); // RAII
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGuiViewport* viewport = ImGui::GetMainViewport();
        work_pos = viewport->WorkPos;
        viewport_width = viewport->WorkSize.x;

        // Compute column width so we have padding on top/left/right/bottom and between columns.
        const int NUM_COLUMNS = 3;
        const float pad = WINDOW_LAYOUT_PADDING;
        const float total_w = viewport->WorkSize.x - (2.0f * pad); // exclude left+right padding
        const float total_h = viewport->WorkSize.y - (2.0f * pad); // exclude top+bottom padding

        // Space between columns is (NUM_COLUMNS-1) * pad; distribute remaining width evenly.
        const float column_width = (total_w - (NUM_COLUMNS - 1) * pad) / (float)NUM_COLUMNS;

        window_size = ImVec2(column_width, total_h);

        faculty_window(session);
        students_window(session);
        schedule_window(session);

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.11f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    session.state.save();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
