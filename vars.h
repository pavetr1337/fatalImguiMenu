#pragma once
namespace vars {
    inline bool isMenuOpen = true;

    inline bool isJopa123 = true;
    inline float scrollerTest = 0.f;
    inline int numTest = 0;
    inline int obkak_list = 0;

    inline ImGuiKey keybind = ImGuiKey_None;

    namespace colors
    {
        inline float window_bg[4] = { 0.066f, 0.059f, 0.141f, 1.f };
        inline float child_bg[4] = { 0.098f, 0.086f, 0.208f, 1.f };
        inline float text_inactive[4] = { 1.f, 1.f, 1.f, 0.5f };
        inline float text[4] = { 1.f, 1.f, 1.f, 1.f };
        inline float selection[4] = { 0.761f, 0.09f, 0.314f, 1.f };
        inline float selection_high[4] = { 0.82f, 0.106f, 0.345f, 1.f };
        inline float transp[4] = { 0.f, 0.f, 0.f, 0.f };
        inline float border[4] = { 1.f, 1.f, 1.f, 0.1f };
        inline float button[4] = { 0.118f, 0.098f, 0.243f, 1.f };
        inline float button_hover[4] = { 0.145f, 0.122f, 0.302f, 1.f };
        inline float button_clicked[4] = { 0.169f, 0.141f, 0.349f, 1.f };

        inline float frame_bg_hover[4] = { frame_bg_hover[0] * 1.1f, frame_bg_hover[1] * 1.1f, frame_bg_hover[2] * 1.1f, 1.f };
        inline float frame_bg_active[4] = { frame_bg_hover[0] * 1.5f, frame_bg_hover[1] * 1.5f, frame_bg_hover[2] * 1.5f, 1.f};
        //rgba(189, 28, 98, 255)
    }

    namespace fonts
    {
        inline ImFont* josefin_sans_regular;
        inline ImFont* josefin_sans_header;
        inline ImFont* font_icon;
    }

    namespace styles
    {
        inline float rounding = 4.f;
        inline float margin = 10.f;
        inline ImVec2 tab_padding = {10.f,10.f};
        inline ImVec2 win_size = { 848.f,588.f };
    }
}