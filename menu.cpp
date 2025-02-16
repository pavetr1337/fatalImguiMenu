#include "menu.h"
#include "vars.h"

const char* tabs[6] = {
        ICON_FA_BOMB " RAGE",
        ICON_FA_CROSSHAIRS " LEGIT",
        ICON_FA_EYE " VISUALS",
        ICON_FA_COG " MISC",
        ICON_FA_CODE " LUA",
        ICON_FA_PAINT_BRUSH " SKINS"
};

void load_fonts() {
    ImGuiIO& io = ImGui::GetIO();

    ImFontConfig fontCfg;
    fontCfg.FontDataOwnedByAtlas = false;
    vars::fonts::josefin_sans_regular = io.Fonts->AddFontFromMemoryTTF(josefinSansRegular, josefinSansRegularSize, 16.0f, &fontCfg);
    vars::fonts::josefin_sans_header = io.Fonts->AddFontFromMemoryTTF(josefinSansBold, josefinSansBoldSize, 18.0f, &fontCfg);

    ImFontConfig fontCfg1;
    static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    io.Fonts->AddFontFromMemoryTTF(josefinSansRegular, josefinSansRegularSize, 16.0f, &fontCfg1);
    fontCfg1.MergeMode = true;
    vars::fonts::font_icon = io.Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, 16.0f, &fontCfg1, icon_ranges);
}

int curtab = 0;

std::unordered_map<const char*, ImVec2> child_labels;

void add_child_label(ImVec2 child_pos, const char* label) {
    ImVec2 label_size = ImGui::CalcTextSize(label);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + label_size[1] / 2);
    child_labels[label] = child_pos;
}

void draw_over_items() {
    for (const auto& item : child_labels) {
        const char* text = item.first;
        ImVec2 pos = item.second;

        ImVec2 label_size = ImGui::CalcTextSize(text);
        ImGui::GetWindowDrawList()->AddText(ImVec2(pos[0], pos[1] - label_size[1]), ImGui::GetColorU32(ImGuiCol_Text), text);
    }
}

void imgui_hotkey(ImGuiKey* k, const ImVec2& size_arg = ImVec2(0, 0))
{
    static bool waitingForKey = false;
    static int delayCounter = 0;

    if (!waitingForKey) {
        auto needKeyName = ImGui::GetKeyName(*k);
        if (ImGui::Button(needKeyName, ImVec2(std::max(ImGui::CalcTextSize(needKeyName)[0], size_arg.x), size_arg.y))) {
            waitingForKey = true;
            delayCounter = 0;
        }
    }
    else {
        ImGui::Button("...", size_arg);

        if (++delayCounter > 3) {
            for (int i = ImGuiKey_NamedKey_BEGIN; i < ImGuiKey_NamedKey_END; i++) {
                ImGuiKey curKey = static_cast<ImGuiKey>(i);
                if (ImGui::IsKeyDown(curKey)) {
                    *k = curKey;
                    waitingForKey = false;
                    delayCounter = 0;
                    break;
                }
            }
        }
    }
}

void draw_menu() {
	ImGuiStyle& style = ImGui::GetStyle();
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, vars::styles::rounding);
    ImGui::PushStyleVar(ImGuiStyleVar_TabBarBorderSize, 0.f);
    ImGui::PushStyleVar(ImGuiStyleVar_TabRounding, 0.f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, vars::styles::rounding);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, vars::styles::rounding);
    ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, vars::styles::rounding);

    ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.f);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(vars::colors::window_bg[0], vars::colors::window_bg[1], vars::colors::window_bg[2], vars::colors::window_bg[3]));
	ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(vars::colors::window_bg[0], vars::colors::window_bg[1], vars::colors::window_bg[2], vars::colors::window_bg[3]));
	ImGui::PushStyleColor(ImGuiCol_TextDisabled, ImVec4(vars::colors::text_inactive[0], vars::colors::text_inactive[1], vars::colors::text_inactive[2], vars::colors::text_inactive[3]));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(vars::colors::text[0], vars::colors::text[1], vars::colors::text[2], vars::colors::text[3]));
    ImGui::PushStyleColor(ImGuiCol_TabSelected, ImVec4(vars::colors::selection[0], vars::colors::selection[1], vars::colors::selection[2], vars::colors::selection[3]));
    ImGui::PushStyleColor(ImGuiCol_TabHovered, ImVec4(vars::colors::selection[0], vars::colors::selection[1], vars::colors::selection[2], vars::colors::selection[3]));
    ImGui::PushStyleColor(ImGuiCol_TabDimmed, ImVec4(vars::colors::transp[0], vars::colors::transp[1], vars::colors::transp[2], vars::colors::transp[3]));
    ImGui::PushStyleColor(ImGuiCol_TabDimmedSelected, ImVec4(vars::colors::transp[0], vars::colors::transp[1], vars::colors::transp[2], vars::colors::transp[3]));
    ImGui::PushStyleColor(ImGuiCol_Tab, ImVec4(vars::colors::transp[0], vars::colors::transp[1], vars::colors::transp[2], vars::colors::transp[3]));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(vars::colors::border[0], vars::colors::border[1], vars::colors::border[2], vars::colors::border[3]));
    ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(vars::colors::border[0], vars::colors::border[1], vars::colors::border[2], vars::colors::border[3]));
    
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(vars::colors::button[0], vars::colors::button[1], vars::colors::button[2], vars::colors::button[3]));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(vars::colors::button_hover[0], vars::colors::button_hover[1], vars::colors::button_hover[2], vars::colors::button_hover[3]));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(vars::colors::button_clicked[0], vars::colors::button_clicked[1], vars::colors::button_clicked[2], vars::colors::button_clicked[3]));
    
    ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(vars::colors::selection[0], vars::colors::selection[1], vars::colors::selection[2], vars::colors::selection[3]));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(vars::colors::window_bg[0], vars::colors::window_bg[1], vars::colors::window_bg[2], vars::colors::window_bg[3]));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(vars::colors::frame_bg_active[0], vars::colors::frame_bg_active[1], vars::colors::frame_bg_active[2], vars::colors::frame_bg_active[3]));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(vars::colors::frame_bg_hover[0], vars::colors::frame_bg_hover[1], vars::colors::frame_bg_hover[2], vars::colors::frame_bg_hover[3]));
    
    ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(vars::colors::selection[0], vars::colors::selection[1], vars::colors::selection[2], vars::colors::selection[3]));
    ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(vars::colors::selection_high[0], vars::colors::selection_high[1], vars::colors::selection_high[2], vars::colors::selection_high[3]));

    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(vars::colors::selection[0], vars::colors::selection[1], vars::colors::selection[2], vars::colors::selection[3]));
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(vars::colors::selection_high[0], vars::colors::selection_high[1], vars::colors::selection_high[2], vars::colors::selection_high[3]));
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(vars::colors::transp[0], vars::colors::transp[1], vars::colors::transp[2], vars::colors::transp[3]));

    ImGui::SetNextWindowSize(vars::styles::win_size, ImGuiCond_Once);

    ImGui::PushFont(vars::fonts::josefin_sans_regular);
	ImGui::Begin(xorstr("FATALMENU by Pavetr"), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    
    child_labels = {};

    float maintab_h = ImGui::GetFontSize() * 2.75f;
    float endtab_h = ImGui::GetFontSize() * 1.5f;

    ImVec2 winpos = ImGui::GetWindowPos();

    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(vars::colors::child_bg[0], vars::colors::child_bg[1], vars::colors::child_bg[2], vars::colors::child_bg[3]));
    
    // Header
    ImGui::SetCursorPos(ImVec2(0.f,0.f));
    ImGui::BeginChild(xorstr("fatal_shapka"), ImVec2(vars::styles::win_size[0], maintab_h), ImGuiChildFlags_None, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    
    ImGui::PopFont();
    ImGui::PushFont(vars::fonts::josefin_sans_header);
    ImGui::SetCursorPos(ImVec2(vars::styles::margin, maintab_h*0.5f-ImGui::GetFontSize()*0.4f));
    
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(vars::colors::selection[0], vars::colors::selection[1], vars::colors::selection[2], vars::colors::selection[3]));
    ImGui::Text(xorstr("FATAL"));
    ImGui::PopStyleColor();
    ImGui::SetCursorPos(ImVec2(vars::styles::margin*1.2f, maintab_h * 0.5f - ImGui::GetFontSize() * 0.4f));
    ImGui::Text(xorstr("FATAL"));
    float header_size = ImGui::CalcTextSize(xorstr("FATAL"))[0];
    ImGui::PopFont();
    ImGui::PushFont(vars::fonts::josefin_sans_regular);

    ImGui::PushFont(vars::fonts::font_icon);
    float prev_b_w = 0.f;
    for (int i = 0; i < 6; ++i) {
        const char* txt = tabs[i];
        ImVec2 buttonSize = ImVec2(ImGui::CalcTextSize(txt)[0]+vars::styles::margin*2, ImGui::CalcTextSize(txt)[1] + vars::styles::margin);
        ImGui::SetCursorPos(ImVec2(header_size + vars::styles::margin * 2 + prev_b_w, maintab_h * 0.5f - buttonSize[1] * 0.5f));
        if (ImGui::Button(txt, buttonSize, curtab == i ? ImGuiCol_Text : ImGuiCol_TextDisabled))
            curtab = i;
        prev_b_w += buttonSize[0] + vars::styles::margin;
    }
    ImGui::PopFont();


    ImGui::EndChild();
    ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(winpos[0], winpos[1]+maintab_h-1.f), ImVec2(winpos[0]+vars::styles::win_size[0], winpos[1]+maintab_h + 1.f), ImGui::GetColorU32(ImGuiCol_Separator));
        
    // Footer
    ImGui::SetCursorPos(ImVec2(0.f, vars::styles::win_size[1] - endtab_h));
    ImGui::BeginChild(xorstr("fatal_footer"), ImVec2(vars::styles::win_size[0], endtab_h), ImGuiChildFlags_None);
    ImGui::PushFont(vars::fonts::font_icon);
    ImGui::SetCursorPosX(vars::styles::margin);
    if (ImGui::Button(ICON_FA_SAVE)) {
        ImGui::OpenPopup("save_configs_popup");
    }
    ImGui::SameLine();
    ImGui::SetCursorPosX(vars::styles::margin*2+ImGui::CalcTextSize(ICON_FA_SAVE)[0]);
    if (ImGui::Button(ICON_FA_FOLDER)) {
        ImGui::OpenPopup("load_configs_popup");
    }

    if (ImGui::BeginPopup("load_configs_popup"))
    {
        const char* configs[3] = { "rage.cfg","semi-rage.cfg","legit.cfg" };
        ImGui::SeparatorText("Load cfg");
        for (int i = 0; i < IM_ARRAYSIZE(configs); i++)
            ImGui::Selectable(configs[i]);
        ImGui::Button("Open folder");
        ImGui::EndPopup();
    }

    if (ImGui::BeginPopup("save_configs_popup"))
    {
        ImGui::SeparatorText("Save cfg");
        static char cfg_name[32] = "";
        ImGui::Text("Enter config name:");
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
        ImGui::InputText("##cfg_name", cfg_name, IM_ARRAYSIZE(cfg_name));
        ImGui::PopStyleVar();
        ImGui::Button("Save");
        ImGui::EndPopup();
    }

    ImGui::SameLine();
    ImGui::SetCursorPosX(vars::styles::win_size[0] - vars::styles::margin * 2 - ImGui::CalcTextSize(ICON_FA_INFO_CIRCLE)[0]);
    if (ImGui::Button(ICON_FA_INFO_CIRCLE)) {
        ImGui::OpenPopup("info_popup");
    }
    if (ImGui::BeginPopup("info_popup"))
    {
        ImGui::Text("Fatal Menu paste by pavetr\npavetr.ru/ds\ngithub.com/pavetr1337\ngithub.com/ocornut/imgui");
        ImGui::EndPopup();
    }

    ImGui::PopFont();
    ImGui::EndChild();
    ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(winpos[0], winpos[1] + vars::styles::win_size[1] - endtab_h - 1.f), ImVec2(winpos[0] + vars::styles::win_size[0], winpos[1] + vars::styles::win_size[1] - endtab_h + 1.f), ImGui::GetColorU32(ImGuiCol_Separator));
    
    ImGui::PopStyleColor();

    // Main block
    ImGui::SetCursorPos(ImVec2(vars::styles::margin, maintab_h+1.f+vars::styles::margin));
    ImGui::BeginChild(xorstr("fatal_main"), ImVec2(vars::styles::win_size[0] - vars::styles::margin * 2, vars::styles::win_size[1] - endtab_h - maintab_h - 2.f - vars::styles::margin * 2), ImGuiChildFlags_None, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(vars::colors::child_bg[0], vars::colors::child_bg[1], vars::colors::child_bg[2], vars::colors::child_bg[3]));
    switch (curtab) {
    case 0:
        if (ImGui::BeginTabBar(xorstr("aim_tabbar"), ImGuiTabBarFlags_None))
        {
            if (ImGui::BeginTabItem(xorstr("Aimbot")))
            {
                ImGui::SetCursorPosY(maintab_h);

                float row_width = (vars::styles::win_size[0]) / 3 - vars::styles::margin*1.3f;
                float row_height = vars::styles::win_size[1] - endtab_h - maintab_h - 2.f - vars::styles::margin * 7;

                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(vars::styles::margin, vars::styles::margin));

                ImGui::BeginChild(xorstr("aim_row_1"), ImVec2(row_width, row_height), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
                add_child_label(ImGui::GetCursorScreenPos(), xorstr("WEAPON"));

                ImVec2 checkboxSize = ImVec2(row_width - vars::styles::margin * 2, 16.f);

                ImGui::CheckboxSized(xorstr("Test1"), &vars::isJopa123, checkboxSize);
                ImGui::SliderFloat(xorstr("FloatSlide"), &vars::scrollerTest,0.f, 100.f, "%.3f", ImGuiSliderFlags_FixedWidth, checkboxSize[0]);
                ImGui::CheckboxSized(xorstr("Test2"), &vars::isJopa123, checkboxSize);
                ImGui::CheckboxSized(xorstr("Test3"), &vars::isJopa123, checkboxSize);
                ImGui::SliderInt(xorstr("IntSlide"), &vars::numTest, 0, 100, "%d", ImGuiSliderFlags_FixedWidth, checkboxSize[0]);
                ImGui::EndChild();

                ImGui::SameLine(row_width+vars::styles::margin);

                ImGui::BeginChild(xorstr("aim_row_2"), ImVec2(row_width, row_height), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
                add_child_label(ImGui::GetCursorScreenPos(), xorstr("EXTRA"));
                const char* items[] = { xorstr("Choice1"),xorstr("Choice2"),xorstr("Choice3") };
                ImGui::Combo(xorstr("Select item"),&vars::obkak_list, items, 3);
                ImGui::EndChild();

                ImGui::SameLine((row_width + vars::styles::margin)*2);

                ImGui::BeginChild(xorstr("aim_row_3"), ImVec2(row_width, row_height), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
                add_child_label(ImGui::GetCursorScreenPos(), xorstr("GENERAL"));

                float check_size = 16.f;

                ImGui::CheckboxSized(xorstr("Test1"), &vars::isJopa123, ImVec2(checkboxSize[0], check_size));
                

                ImGui::CheckboxSized(xorstr("ColorPicker"), &vars::isJopa123, ImVec2(row_width - vars::styles::margin * 3 - check_size, check_size));
                ImGui::SameLine();
                ImGui::SetCursorPos(ImVec2(row_width - vars::styles::margin * 1.5 - check_size,ImGui::GetCursorPosY()-check_size*0.4f));
                ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
                ImGui::ColorEdit4(xorstr("Background color"), vars::colors::window_bg, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
                ImGui::PopStyleVar();

                ImGui::CheckboxSized(xorstr("Setting pop"), &vars::isJopa123, ImVec2(row_width - vars::styles::margin * 3 - check_size, check_size));
                ImGui::SameLine();
                ImGui::SetCursorPos(ImVec2(row_width - vars::styles::margin * 1.5 - check_size, ImGui::GetCursorPosY() - check_size * 0.4f));
                ImGui::PushFont(vars::fonts::font_icon);
                if (ImGui::Button(ICON_FA_COG, ImVec2(check_size, check_size))) {
                    ImGui::OpenPopup("setting_popup");
                }
                if (ImGui::BeginPopup("setting_popup"))
                {
                    ImGui::Text("Keybind:");
                    imgui_hotkey(&vars::keybind,ImVec2(check_size*3, check_size));
                    ImGui::EndPopup();
                }
                ImGui::PopFont();
                ImGui::EndChild();

                ImGui::PopStyleVar();

                ImGui::EndTabItem();

            }
            if (ImGui::BeginTabItem(xorstr("Anti-Aim")))
            {
                ImGui::Text(xorstr("AA tab"));
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
    }
    ImGui::PopStyleColor();

    draw_over_items();
    
    ImGui::EndChild();

	ImGui::End();

    ImGui::PopFont();

	ImGui::PopStyleColor(23);
	ImGui::PopStyleVar(8);
}