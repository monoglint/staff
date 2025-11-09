#include "imgui_spectrum.hh"
#include "inter_medium.hh"

#include "imgui/imgui.h"

namespace ImGui {
    namespace Spectrum {
        void LoadFont(float size) {
            ImGuiIO& io = ImGui::GetIO();
            ImFont* font = io.Fonts->AddFontFromMemoryTTF(fonts::inter_medium::raw_ttf, fonts::inter_medium::ttf_len, 16.0f);
            IM_ASSERT(font != nullptr);
            io.FontDefault = font;
        }

        void StyleColorsSpectrum() {
            ImGuiStyle* style = &ImGui::GetStyle();
            style->GrabRounding = 8.0f;
            style->FrameRounding = 8.0f;
            style->TabRounding = 8.0f;
            style->ScrollbarRounding = 8.0f;
            style->ChildRounding = 8.0f;
            style->PopupRounding = 8.0f;
            style->WindowRounding = 16.0f;

            style->WindowPadding = ImVec2(16.0f, 16.0f);
            style->FramePadding = ImVec2(16.0f, 8.0f);
            style->ItemSpacing = ImVec2(4.0f, 16.0f);
            style->CellPadding = ImVec2(4.0f, 4.0f);
            style->ItemInnerSpacing = ImVec2(4.0f, 4.0f);
            
            ImVec4* colors = style->Colors;
            colors[ImGuiCol_Text] = ColorConvertU32ToFloat4(Spectrum::GRAY800); // text on hovered controls is gray900
            colors[ImGuiCol_TextDisabled] = ColorConvertU32ToFloat4(Spectrum::GRAY500);
            colors[ImGuiCol_WindowBg] = ColorConvertU32ToFloat4(Spectrum::GRAY100);
            colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
            colors[ImGuiCol_PopupBg] = ColorConvertU32ToFloat4(Spectrum::GRAY50); // not sure about this. Note: applies to tooltips too.
            colors[ImGuiCol_Border] = ColorConvertU32ToFloat4(Spectrum::GRAY300);
            colors[ImGuiCol_BorderShadow] = ColorConvertU32ToFloat4(Spectrum::Static::NONE); // We don't want shadows. Ever.
            colors[ImGuiCol_FrameBg] = ColorConvertU32ToFloat4(Spectrum::GRAY75); // this isnt right, spectrum does not do this, but it's a good fallback
            colors[ImGuiCol_FrameBgHovered] = ColorConvertU32ToFloat4(Spectrum::GRAY50);
            colors[ImGuiCol_FrameBgActive] = ColorConvertU32ToFloat4(Spectrum::GRAY200);
            colors[ImGuiCol_TitleBg] = ColorConvertU32ToFloat4(Spectrum::GRAY300); // those titlebar values are totally made up, spectrum does not have this.
            colors[ImGuiCol_TitleBgActive] = ColorConvertU32ToFloat4(Spectrum::GRAY200);
            colors[ImGuiCol_TitleBgCollapsed] = ColorConvertU32ToFloat4(Spectrum::GRAY400);
            colors[ImGuiCol_MenuBarBg] = ColorConvertU32ToFloat4(Spectrum::GRAY100);
            colors[ImGuiCol_ScrollbarBg] = ColorConvertU32ToFloat4(Spectrum::GRAY100); // same as regular background
            colors[ImGuiCol_ScrollbarGrab] = ColorConvertU32ToFloat4(Spectrum::GRAY400);
            colors[ImGuiCol_ScrollbarGrabHovered] = ColorConvertU32ToFloat4(Spectrum::GRAY600);
            colors[ImGuiCol_ScrollbarGrabActive] = ColorConvertU32ToFloat4(Spectrum::GRAY700);
            colors[ImGuiCol_CheckMark] = ColorConvertU32ToFloat4(Spectrum::GRAY900);
            colors[ImGuiCol_SliderGrab] = ColorConvertU32ToFloat4(Spectrum::GRAY700);
            colors[ImGuiCol_SliderGrabActive] = ColorConvertU32ToFloat4(Spectrum::GRAY800);
            colors[ImGuiCol_Button] = ColorConvertU32ToFloat4(Spectrum::GRAY75); // match default button to Spectrum's 'Action Button'.
            colors[ImGuiCol_ButtonHovered] = ColorConvertU32ToFloat4(Spectrum::GRAY50);
            colors[ImGuiCol_ButtonActive] = ColorConvertU32ToFloat4(Spectrum::GRAY200);
            colors[ImGuiCol_Header] = ColorConvertU32ToFloat4(Spectrum::BLUE400);
            colors[ImGuiCol_HeaderHovered] = ColorConvertU32ToFloat4(Spectrum::BLUE500);
            colors[ImGuiCol_HeaderActive] = ColorConvertU32ToFloat4(Spectrum::BLUE600);
            colors[ImGuiCol_Separator] = ColorConvertU32ToFloat4(Spectrum::GRAY400);
            colors[ImGuiCol_SeparatorHovered] = ColorConvertU32ToFloat4(Spectrum::GRAY600);
            colors[ImGuiCol_SeparatorActive] = ColorConvertU32ToFloat4(Spectrum::GRAY700);
            colors[ImGuiCol_ResizeGrip] = ColorConvertU32ToFloat4(Spectrum::GRAY400);
            colors[ImGuiCol_ResizeGripHovered] = ColorConvertU32ToFloat4(Spectrum::GRAY600);
            colors[ImGuiCol_ResizeGripActive] = ColorConvertU32ToFloat4(Spectrum::GRAY700);
            colors[ImGuiCol_PlotLines] = ColorConvertU32ToFloat4(Spectrum::BLUE400);
            colors[ImGuiCol_PlotLinesHovered] = ColorConvertU32ToFloat4(Spectrum::BLUE600);
            colors[ImGuiCol_PlotHistogram] = ColorConvertU32ToFloat4(Spectrum::BLUE400);
            colors[ImGuiCol_PlotHistogramHovered] = ColorConvertU32ToFloat4(Spectrum::BLUE600);
            colors[ImGuiCol_TextSelectedBg] = ColorConvertU32ToFloat4((Spectrum::BLUE400 & 0x00FFFFFF) | 0x33000000);
            colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
            colors[ImGuiCol_NavCursor] = ColorConvertU32ToFloat4((Spectrum::GRAY900 & 0x00FFFFFF) | 0x0A000000);
            colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
            colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
            colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
            colors[ImGuiCol_CheckMark] = ColorConvertU32ToFloat4(Spectrum::BLUE700);
            colors[ImGuiCol_Tab] = ColorConvertU32ToFloat4(Spectrum::GRAY300);
            colors[ImGuiCol_TabSelected] = ColorConvertU32ToFloat4(Spectrum::BLUE500);
            colors[ImGuiCol_TabHovered] = ColorConvertU32ToFloat4(Spectrum::BLUE700);
            colors[ImGuiCol_TabDimmed] = ColorConvertU32ToFloat4(Spectrum::GRAY400);
            colors[ImGuiCol_TabDimmedSelected] = ColorConvertU32ToFloat4(Spectrum::BLUE700);
            colors[ImGuiCol_TableHeaderBg] = ColorConvertU32ToFloat4(Spectrum::GRAY100);
            colors[ImGuiCol_TableRowBg] = ColorConvertU32ToFloat4(Spectrum::GRAY75);
            colors[ImGuiCol_TableRowBgAlt] = ColorConvertU32ToFloat4(Spectrum::GRAY75);
            colors[ImGuiCol_TableBorderStrong] = ColorConvertU32ToFloat4(Spectrum::GRAY300);
            colors[ImGuiCol_TableBorderLight] = ColorConvertU32ToFloat4(Spectrum::GRAY300);
        }
    }
}