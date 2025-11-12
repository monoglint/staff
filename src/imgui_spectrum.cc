/*

modified by monoglint

*/

#include "imgui_spectrum.hh"
#include "inter_medium.hh"

#include "imgui/imgui.h"

namespace themes {
    namespace spectrum_dark {
        void load() {
            ImGuiIO& io = ImGui::GetIO();
            ImFont* font = io.Fonts->AddFontFromMemoryTTF(fonts::inter_medium::raw_ttf, fonts::inter_medium::ttf_len, 16.0f);
            IM_ASSERT(font != nullptr);
            io.FontDefault = font;

            ImGuiStyle& style = ImGui::GetStyle();
            style.GrabRounding = 4.0f;
            style.FrameRounding = 4.0f;
            style.TabRounding = 4.0f;
            style.ScrollbarRounding = 2.0f;
            style.ChildRounding = 8.0f;
            style.PopupRounding = 8.0f;
            style.WindowRounding = 8.0f;

            style.WindowPadding = ImVec2(16.0f, 16.0f);
            style.FramePadding = ImVec2(16.0f, 8.0f);
            style.ItemSpacing = ImVec2(8.0f, 8.0f);
            style.CellPadding = ImVec2(8.0f, 4.0f);
            style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
            style.TitleBarPadding = ImVec2(16.0f, 12.0f);
            
            style.ScrollbarSize = 8;
            style.ScrollbarPadding = 32;

            ImVec4* colors = style.Colors;
            colors[ImGuiCol_Text] = ImGui::ColorConvertU32ToFloat4(spectrum_dark::GRAY800); // text on hovered controls is gray900
            colors[ImGuiCol_TextDisabled] = ImGui::ColorConvertU32ToFloat4(spectrum_dark::GRAY500);
            colors[ImGuiCol_WindowBg] = ImGui::ColorConvertU32ToFloat4(spectrum_dark::GRAY100);
            colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
            colors[ImGuiCol_PopupBg] = ImGui::ColorConvertU32ToFloat4(spectrum_dark::GRAY50); // not sure about this. Note: applies to tooltips too.
            colors[ImGuiCol_Border] = ImGui::ColorConvertU32ToFloat4(spectrum_dark::GRAY300);
            colors[ImGuiCol_BorderShadow] = ImGui::ColorConvertU32ToFloat4(spectrum_dark::static_colors::WHITE); // We don't want shadows. Ever.
            colors[ImGuiCol_FrameBg] = ImGui::ColorConvertU32ToFloat4(spectrum_dark::GRAY300); // this isnt right, spectrum does not do this, but it's a good fallback
            colors[ImGuiCol_FrameBgHovered] = ImGui::ColorConvertU32ToFloat4(spectrum_dark::GRAY400);
            colors[ImGuiCol_FrameBgActive] = ImGui::ColorConvertU32ToFloat4(spectrum_dark::GRAY400);
            colors[ImGuiCol_TitleBg] = ImGui::ColorConvertU32ToFloat4(spectrum_dark::GRAY50); // those titlebar values are totally made up, spectrum does not have this.
            colors[ImGuiCol_TitleBgActive] = ImGui::ColorConvertU32ToFloat4(spectrum_dark::GRAY50);
            colors[ImGuiCol_TitleBgCollapsed] = ImGui::ColorConvertU32ToFloat4(spectrum_dark::GRAY50);
            colors[ImGuiCol_MenuBarBg] = ImGui::ColorConvertU32ToFloat4(spectrum_dark::GRAY100);
            colors[ImGuiCol_ScrollbarBg] = ImGui::ColorConvertU32ToFloat4(spectrum_dark::GRAY100); // same as regular background
            colors[ImGuiCol_ScrollbarGrab] = ImGui::ColorConvertU32ToFloat4(spectrum_dark::GRAY400);
            colors[ImGuiCol_ScrollbarGrabHovered] = ImGui::ColorConvertU32ToFloat4(spectrum_dark::GRAY600);
            colors[ImGuiCol_ScrollbarGrabActive] = ImGui::ColorConvertU32ToFloat4(spectrum_dark::GRAY700);
            colors[ImGuiCol_SliderGrab] = ImGui::ColorConvertU32ToFloat4(spectrum_dark::GRAY700);
            colors[ImGuiCol_SliderGrabActive] = ImGui::ColorConvertU32ToFloat4(spectrum_dark::GRAY800);
            colors[ImGuiCol_Button] = ImGui::ColorConvertU32ToFloat4(spectrum_dark::GRAY300); // match default button to Spectrum's 'Action Button'.
            colors[ImGuiCol_ButtonHovered] = ImGui::ColorConvertU32ToFloat4(spectrum_dark::GRAY400);
            colors[ImGuiCol_ButtonActive] = ImGui::ColorConvertU32ToFloat4(spectrum_dark::GRAY200);
            colors[ImGuiCol_Header] = ImGui::ColorConvertU32ToFloat4(spectrum_dark::GRAY100);
            colors[ImGuiCol_HeaderHovered] = ImGui::ColorConvertU32ToFloat4(spectrum_dark::GRAY200);
            colors[ImGuiCol_HeaderActive] = ImGui::ColorConvertU32ToFloat4(spectrum_dark::GRAY75);
            colors[ImGuiCol_Separator] = ImGui::ColorConvertU32ToFloat4(spectrum_dark::GRAY400);
            colors[ImGuiCol_SeparatorHovered] = ImGui::ColorConvertU32ToFloat4(spectrum_dark::GRAY600);
            colors[ImGuiCol_SeparatorActive] = ImGui::ColorConvertU32ToFloat4(spectrum_dark::GRAY700);
            colors[ImGuiCol_ResizeGrip] = ImGui::ColorConvertU32ToFloat4(spectrum_dark::GRAY400);
            colors[ImGuiCol_ResizeGripHovered] = ImGui::ColorConvertU32ToFloat4(spectrum_dark::GRAY600);
            colors[ImGuiCol_ResizeGripActive] = ImGui::ColorConvertU32ToFloat4(spectrum_dark::GRAY300);
            colors[ImGuiCol_PlotLines] = ImGui::ColorConvertU32ToFloat4(spectrum_dark::BLUE400);
            colors[ImGuiCol_PlotLinesHovered] = ImGui::ColorConvertU32ToFloat4(spectrum_dark::BLUE600);
            colors[ImGuiCol_PlotHistogram] = ImGui::ColorConvertU32ToFloat4(spectrum_dark::BLUE400);
            colors[ImGuiCol_PlotHistogramHovered] = ImGui::ColorConvertU32ToFloat4(spectrum_dark::BLUE600);
            colors[ImGuiCol_TextSelectedBg] = ImGui::ColorConvertU32ToFloat4((spectrum_dark::BLUE400 & 0x00FFFFFF) | 0x33000000);
            colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
            colors[ImGuiCol_NavCursor] = ImGui::ColorConvertU32ToFloat4((spectrum_dark::GRAY900 & 0x00FFFFFF) | 0x0A000000);
            colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
            colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
            colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
            colors[ImGuiCol_CheckMark] = ImGui::ColorConvertU32ToFloat4(spectrum_dark::BLUE700);
            colors[ImGuiCol_Tab] = ImGui::ColorConvertU32ToFloat4(spectrum_dark::GRAY300);
            colors[ImGuiCol_TabSelected] = ImGui::ColorConvertU32ToFloat4(spectrum_dark::GRAY500);
            colors[ImGuiCol_TabHovered] = ImGui::ColorConvertU32ToFloat4(spectrum_dark::GRAY700);
            colors[ImGuiCol_TabDimmed] = ImGui::ColorConvertU32ToFloat4(spectrum_dark::GRAY400);
            colors[ImGuiCol_TabDimmedSelected] = ImGui::ColorConvertU32ToFloat4(spectrum_dark::BLUE700);
            colors[ImGuiCol_TableHeaderBg] = ImGui::ColorConvertU32ToFloat4(spectrum_dark::GRAY100);
            colors[ImGuiCol_TableRowBg] = ImGui::ColorConvertU32ToFloat4(spectrum_dark::GRAY100);
            colors[ImGuiCol_TableRowBgAlt] = ImGui::ColorConvertU32ToFloat4(spectrum_dark::GRAY75);
            colors[ImGuiCol_TableBorderStrong] = ImGui::ColorConvertU32ToFloat4(spectrum_dark::GRAY200);
            colors[ImGuiCol_TableBorderLight] = ImGui::ColorConvertU32ToFloat4(spectrum_dark::GRAY200);
        }
    }
}