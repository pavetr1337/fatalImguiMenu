#include "menu.h"
#include "vars.h"
#include "texture.hpp"
#include "img/vis_preview.hpp"
#include "img/vis_preview_ent.hpp"

const char* tabs[6] = {
        ICON_FA_BOMB " RAGE",
        ICON_FA_CROSSHAIRS " LEGIT",
        ICON_FA_EYE " VISUALS",
        ICON_FA_COG " MISC",
        ICON_FA_CODE " LUA",
        ICON_FA_PAINT_BRUSH " SKINS"
};

float dpi_scales_num[] = { 0.85f, 1.f, 1.25f, 1.5f, 1.75f, 2.f };
float checkboxSizeEx = 16.f;

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

std::unordered_map<const char*, std::pair<int,ImVec2>> child_labels;

void add_child_label(ImVec2 child_pos, const char* label, int tab) {
    ImVec2 label_size = ImGui::CalcTextSize(label);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + label_size[1] / 2);
    child_labels[label] = { tab, child_pos };
}

void draw_over_items() {
    for (const auto& item : child_labels) {
        if (item.second.first == curtab) {
            const char* text = item.first;
            ImVec2 pos = item.second.second;

            ImVec2 label_size = ImGui::CalcTextSize(text);
            if (curtab < 3) {
                ImGui::GetWindowDrawList()->AddText(ImVec2(pos[0], pos[1] - label_size[1]), ImGui::GetColorU32(ImGuiCol_Text), text);
            }
            else {
                ImGui::GetForegroundDrawList()->AddText(ImVec2(pos[0], pos[1] - label_size[1]), ImGui::GetColorU32(ImGuiCol_Text), text);
            }
        }
    }
}

void imgui_hotkey(ImGuiKey* k, const ImVec2& size_arg = ImVec2(0, 0))
{
    static bool waitingForKey = false;
    static int delayCounter = 0;

    if (!waitingForKey) {
        auto needKeyName = ImGui::GetKeyName(*k);
        if (ImGui::Button(needKeyName, ImVec2(max(ImGui::CalcTextSize(needKeyName)[0], size_arg.x), size_arg.y))) {
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

int hashes = 0;

void imgui_checkbox_bind(const char* icon, const char* label, bool* v, ImGuiKey* k_v, float width) {
    float check_size = checkboxSizeEx * dpi_scales_num[vars::settings::misc::dpi_scale];
    ImGui::CheckboxSized(label, v, ImVec2(width - vars::styles::margin * 3 - check_size, check_size));
    ImGui::SameLine();
    ImGui::SetCursorPos(ImVec2(width - vars::styles::margin * 1.5f - check_size, ImGui::GetCursorPosY() - check_size * 0.25f));
    
    std::string bind_name(label);
    bind_name.append("_keybind");

    ImGui::PushFont(vars::fonts::font_icon);
    bool onclick = ImGui::Button(icon, ImVec2(check_size, check_size));
    ImGui::PopFont();

    if (onclick) {
        ImGui::OpenPopup(bind_name.c_str());
    }
    

    if (ImGui::BeginPopup(bind_name.c_str()))
    {
        ImGui::Text(xorstr("Keybind:"));
        imgui_hotkey(k_v, ImVec2(check_size * 3, check_size));
        ImGui::EndPopup();
    }
    hashes++;
}

void imgui_checkbox_col(const char* label, bool* v, float* col, float width) {
    float check_size = checkboxSizeEx * dpi_scales_num[vars::settings::misc::dpi_scale];
    ImGui::CheckboxSized(label, v, ImVec2(width - vars::styles::margin * 3 - check_size, check_size));
    ImGui::SameLine();
    ImGui::SetCursorPos(ImVec2(width - vars::styles::margin * 1.5f - check_size, ImGui::GetCursorPosY() - check_size * 0.25f));

    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
    ImGui::ColorEdit4(label, col, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
    ImGui::PopStyleVar();
}

std::unordered_map<std::string, IDirect3DTexture9*> textures;

void imgui_cacheImage(IDirect3DDevice9* device, std::string textureID, const unsigned char* image_data, size_t image_size) {
    textures[textureID] = LoadTextureFromMemory(device, image_data, image_size);
}

void imgui_drawimage(std::string textureID, ImVec2 size) {
    if (!textures[textureID]) {
        ImGui::Text(xorstr("No texture"));
    }
    ImGui::Image((ImTextureID)(intptr_t)textures[textureID], size);
}

ImVec4 col_to_imvec4(float* col) {
    return ImVec4(col[0], col[1], col[2], col[3]);
}

void display_vis_preview(ImVec2 pos, ImVec2 size) {
    float check_size = checkboxSizeEx * dpi_scales_num[vars::settings::misc::dpi_scale];
    if (vars::settings::visuals::name) {
        ImGui::PushStyleColor(ImGuiCol_Text, col_to_imvec4(vars::settings::visuals::name_col));
        ImGui::GetWindowDrawList()->AddText(ImVec2(pos.x+size.x/2-ImGui::CalcTextSize("Exec")[0]/2, pos.y), ImGui::GetColorU32(ImGuiCol_Text), "Exec");
        ImGui::PopStyleColor();
    }

    if (vars::settings::visuals::box) {
        ImGui::PushStyleColor(ImGuiCol_Text, col_to_imvec4(vars::settings::visuals::box_col));
        ImGui::GetWindowDrawList()->AddRect(ImVec2(pos.x, pos.y+check_size), ImVec2(pos.x+size.x, pos.y+size.y), ImGui::GetColorU32(ImGuiCol_Text) ,0.f,0.f,1.f);
        ImGui::PopStyleColor();
    }
    if (vars::settings::visuals::health) {
        if (vars::settings::visuals::health_bar) {
            ImGui::PushStyleColor(ImGuiCol_Text, col_to_imvec4(vars::colors::window_bg));
            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(pos.x - vars::styles::margin * 0.75f, pos.y + check_size), ImVec2(pos.x - vars::styles::margin * 0.25f, pos.y + size.y), ImGui::GetColorU32(ImGuiCol_Text));
            ImGui::PopStyleColor();
            ImGui::PushStyleColor(ImGuiCol_Text, col_to_imvec4(vars::settings::visuals::health_col));
            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(pos.x - vars::styles::margin * 0.75f, pos.y + check_size + size.y * 0.41f), ImVec2(pos.x - vars::styles::margin * 0.25f, pos.y + size.y), ImGui::GetColorU32(ImGuiCol_Text));
            ImGui::PopStyleColor();
        }
        if (vars::settings::visuals::health_text) {
            if (vars::settings::visuals::health_bar) {
                ImGui::PushStyleColor(ImGuiCol_Text, col_to_imvec4(vars::settings::visuals::health_col));
                ImGui::GetWindowDrawList()->AddText(ImVec2(pos.x - ImGui::CalcTextSize("59").x - vars::styles::margin, pos.y + ImGui::CalcTextSize("59").y), ImGui::GetColorU32(ImGuiCol_Text), "59");
                ImGui::PopStyleColor();
            }
            else {
                ImGui::PushStyleColor(ImGuiCol_Text, col_to_imvec4(vars::settings::visuals::health_col));
                ImGui::GetWindowDrawList()->AddText(ImVec2(pos.x - ImGui::CalcTextSize("59").x - vars::styles::margin * 0.25f, pos.y + ImGui::CalcTextSize("59").y), ImGui::GetColorU32(ImGuiCol_Text), "59");
                ImGui::PopStyleColor();
            }
        }
    }
    if (vars::settings::visuals::weapon_name) {
        ImGui::PushStyleColor(ImGuiCol_Text, col_to_imvec4(vars::settings::visuals::weapon_col));
        ImGui::GetWindowDrawList()->AddText(ImVec2(pos.x + size.x / 2 - ImGui::CalcTextSize("pickaxe")[0] / 2, pos.y + size.y), ImGui::GetColorU32(ImGuiCol_Text), "pickaxe");
        ImGui::PopStyleColor();
    }
    if (vars::settings::visuals::armor) {
        if (vars::settings::visuals::health) {
            ImGui::PushStyleColor(ImGuiCol_Text, col_to_imvec4(vars::settings::visuals::armor_col));
            if (vars::settings::visuals::health_text) {
                if (!vars::settings::visuals::health_bar) {
                    ImGui::GetWindowDrawList()->AddText(ImVec2(pos.x - ImGui::CalcTextSize("100").x - vars::styles::margin * 0.25f, pos.y + ImGui::CalcTextSize("100").y * 2.f), ImGui::GetColorU32(ImGuiCol_Text), "100");
                }
                else {
                    ImGui::GetWindowDrawList()->AddText(ImVec2(pos.x - ImGui::CalcTextSize("100").x - vars::styles::margin, pos.y + ImGui::CalcTextSize("100").y * 2.f), ImGui::GetColorU32(ImGuiCol_Text), "100");
                }
            }
            else {
                if (!vars::settings::visuals::health_bar) {
                    ImGui::GetWindowDrawList()->AddText(ImVec2(pos.x - ImGui::CalcTextSize("100").x - vars::styles::margin * 0.25f, pos.y + ImGui::CalcTextSize("100").y), ImGui::GetColorU32(ImGuiCol_Text), "100");
                }
                else {
                    ImGui::GetWindowDrawList()->AddText(ImVec2(pos.x - ImGui::CalcTextSize("100").x - vars::styles::margin, pos.y + ImGui::CalcTextSize("100").y), ImGui::GetColorU32(ImGuiCol_Text), "100");
                }
            }
            ImGui::PopStyleColor();
        }
        else {
            ImGui::PushStyleColor(ImGuiCol_Text, col_to_imvec4(vars::settings::visuals::armor_col));
            ImGui::GetWindowDrawList()->AddText(ImVec2(pos.x - ImGui::CalcTextSize("100").x - vars::styles::margin * 0.25f, pos.y + ImGui::CalcTextSize("100").y), ImGui::GetColorU32(ImGuiCol_Text), "100");
            ImGui::PopStyleColor();
        }
    }
    if (vars::settings::visuals::skeleton) {
        ImGui::PushStyleColor(ImGuiCol_Text, col_to_imvec4(vars::settings::visuals::skeleton_col));
        ImGui::GetWindowDrawList()->AddLine(ImVec2(pos.x+size.x*0.5f,pos.y+size.y*0.3f),ImVec2(pos.x+size.x*0.55f, pos.y + size.y * 0.45f), ImGui::GetColorU32(ImGuiCol_Text), 1.f);
        
        ImGui::GetWindowDrawList()->AddLine(ImVec2(pos.x + size.x * 0.3f, pos.y + size.y * 0.425f), ImVec2(pos.x + size.x * 0.55f, pos.y + size.y * 0.45f), ImGui::GetColorU32(ImGuiCol_Text), 1.f);
        ImGui::GetWindowDrawList()->AddLine(ImVec2(pos.x + size.x * 0.3f, pos.y + size.y * 0.425f), ImVec2(pos.x + size.x * 0.025f, pos.y + size.y * 0.95f), ImGui::GetColorU32(ImGuiCol_Text), 1.f);

        ImGui::GetWindowDrawList()->AddLine(ImVec2(pos.x + size.x * 0.55f, pos.y + size.y * 0.45f), ImVec2(pos.x + size.x * 0.82f, pos.y + size.y * 0.5f), ImGui::GetColorU32(ImGuiCol_Text), 1.f);
        ImGui::GetWindowDrawList()->AddLine(ImVec2(pos.x + size.x * 0.82f, pos.y + size.y * 0.5f), ImVec2(pos.x + size.x, pos.y + size.y * 0.8f), ImGui::GetColorU32(ImGuiCol_Text), 1.f);
        
        ImGui::GetWindowDrawList()->AddLine(ImVec2(pos.x + size.x * 0.55f, pos.y + size.y * 0.45f), ImVec2(pos.x + size.x * 0.5f, pos.y + size.y * 0.8f), ImGui::GetColorU32(ImGuiCol_Text), 1.f);

        ImGui::GetWindowDrawList()->AddLine(ImVec2(pos.x + size.x * 0.5f, pos.y + size.y * 0.8f), ImVec2(pos.x + size.x * 0.4f, pos.y + size.y), ImGui::GetColorU32(ImGuiCol_Text), 1.f);
        ImGui::GetWindowDrawList()->AddLine(ImVec2(pos.x + size.x * 0.5f, pos.y + size.y * 0.8f), ImVec2(pos.x + size.x * 0.7f, pos.y + size.y), ImGui::GetColorU32(ImGuiCol_Text), 1.f);

        ImGui::PopStyleColor();
    }
}

void display_vis_preview_ent(ImVec2 pos, ImVec2 size) {
    float check_size = checkboxSizeEx * dpi_scales_num[vars::settings::misc::dpi_scale];
    if (vars::settings::visuals::ent_name) {
        ImGui::PushStyleColor(ImGuiCol_Text, col_to_imvec4(vars::settings::visuals::ent_name_col));
        ImGui::GetWindowDrawList()->AddText(ImVec2(pos.x + size.x / 2 - ImGui::CalcTextSize("Printer")[0] / 2, pos.y), ImGui::GetColorU32(ImGuiCol_Text), "Printer");
        ImGui::PopStyleColor();
    }

    if (vars::settings::visuals::ent_box) {
        ImGui::PushStyleColor(ImGuiCol_Text, col_to_imvec4(vars::settings::visuals::ent_box_col));
        ImGui::GetWindowDrawList()->AddRect(ImVec2(pos.x, pos.y + check_size), ImVec2(pos.x + size.x, pos.y + size.y), ImGui::GetColorU32(ImGuiCol_Text), 0.f, 0.f, 1.f);
        ImGui::PopStyleColor();
    }
    if (vars::settings::visuals::ent_health) {
        if (vars::settings::visuals::ent_health_bar) {
            ImGui::PushStyleColor(ImGuiCol_Text, col_to_imvec4(vars::colors::window_bg));
            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(pos.x - vars::styles::margin * 0.75f, pos.y + check_size), ImVec2(pos.x - vars::styles::margin * 0.25f, pos.y + size.y), ImGui::GetColorU32(ImGuiCol_Text));
            ImGui::PopStyleColor();
            ImGui::PushStyleColor(ImGuiCol_Text, col_to_imvec4(vars::settings::visuals::ent_health_col));
            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(pos.x - vars::styles::margin * 0.75f, pos.y + check_size + size.y * 0.2f), ImVec2(pos.x - vars::styles::margin * 0.25f, pos.y + size.y), ImGui::GetColorU32(ImGuiCol_Text));
            ImGui::PopStyleColor();
        }
        if (vars::settings::visuals::ent_health_text) {
            if (vars::settings::visuals::ent_health_bar) {
                ImGui::PushStyleColor(ImGuiCol_Text, col_to_imvec4(vars::settings::visuals::ent_health_col));
                ImGui::GetWindowDrawList()->AddText(ImVec2(pos.x - ImGui::CalcTextSize("80").x - vars::styles::margin, pos.y + ImGui::CalcTextSize("80").y), ImGui::GetColorU32(ImGuiCol_Text), "80");
                ImGui::PopStyleColor();
            }
            else {
                ImGui::PushStyleColor(ImGuiCol_Text, col_to_imvec4(vars::settings::visuals::ent_health_col));
                ImGui::GetWindowDrawList()->AddText(ImVec2(pos.x - ImGui::CalcTextSize("80").x - vars::styles::margin * 0.25f, pos.y + ImGui::CalcTextSize("80").y), ImGui::GetColorU32(ImGuiCol_Text), "80");
                ImGui::PopStyleColor();
            }
        }
    }
}

int selected_script = 0; // From LUA tab
const char* chams_types[] = { "Flat", "Metal", "Glass", "Wireframe", "Glow" };

const char* skyboxes[] = { "Morning", "Day", "Evening", "Night", "Cloud" };

bool dpi_cached = false;
bool img_cached = false;



void draw_menu(IDirect3DDevice9* device) {
	ImGuiStyle& style = ImGui::GetStyle();
    ImGuiIO& io = ImGui::GetIO();
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, vars::styles::rounding);
    ImGui::PushStyleVar(ImGuiStyleVar_TabBarBorderSize, 0.f);
    ImGui::PushStyleVar(ImGuiStyleVar_TabRounding, 0.f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, vars::styles::rounding);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, vars::styles::rounding);
    ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, vars::styles::rounding);

    ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.f);
    ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, vars::styles::scroll_width);
    ImGui::PushStyleVar(ImGuiStyleVar_PlaySfx, vars::styles::play_sfx);
    ImGui::PushStyleVar(ImGuiStyleVar_AnimSpeed, vars::styles::anim_speed);

    ImGuiStyle* pstyle = &ImGui::GetStyle();

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

    ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, ImVec4(vars::colors::window_bg[0], vars::colors::window_bg[1], vars::colors::window_bg[2], vars::colors::window_bg[3]));
    ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, ImVec4(vars::colors::button[0], vars::colors::button[1], vars::colors::button[2], vars::colors::button[3]));
    ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, ImVec4(vars::colors::button_hover[0], vars::colors::button_hover[1], vars::colors::button_hover[2], vars::colors::button_hover[3]));
    ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, ImVec4(vars::colors::button_clicked[0], vars::colors::button_clicked[1], vars::colors::button_clicked[2], vars::colors::button_clicked[3]));

    if (!dpi_cached) {
        pstyle->CacheScales();
        dpi_cached = true;
        float new_dpi = dpi_scales_num[vars::settings::misc::dpi_scale];
        pstyle->SetDPIScale(new_dpi);
        vars::styles::win_size_ex = vars::styles::win_size;
        vars::styles::win_size.x = vars::styles::win_size.x * new_dpi;
        vars::styles::win_size.y = vars::styles::win_size.y * new_dpi;
        io.FontGlobalScale = new_dpi;
    }

    if (!img_cached) {
        imgui_cacheImage(device, "shaxter_image", visPreview, visPreviewSize);
        imgui_cacheImage(device, "printer_image", visPreviewEnt, visPreviewEntSize);

        img_cached = true;
    }
    

    ImGui::SetNextWindowSize(vars::styles::win_size); // Don't use ImGuiCond_Once due to DPI scale

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
        
        if (i == 1) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(vars::colors::text_inactive[0], vars::colors::text_inactive[1], vars::colors::text_inactive[2], vars::colors::text_inactive[3]));
        }
        ImGui::SetCursorPos(ImVec2(header_size + vars::styles::margin * 2 + prev_b_w, maintab_h * 0.5f - buttonSize[1] * 0.5f));
        if (ImGui::Button(txt, buttonSize, curtab == i ? ImGuiCol_Text : ImGuiCol_TextDisabled))
            if (i != 1) {
                curtab = i;
            }
        if (i == 1) {
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
            {
                ImGui::SetTooltip("Coming soon");
            }
            ImGui::PopStyleColor();
        }
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
        ImGui::Text(xorstr("Fatal Menu paste by pavetr\npavetr.ru/ds\ngithub.com/pavetr1337\ngithub.com/ocornut/imgui"));
        ImGui::EndPopup();
    }

    ImGui::PopFont();
    ImGui::EndChild();
    ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(winpos[0], winpos[1] + vars::styles::win_size[1] - endtab_h - 1.f), ImVec2(winpos[0] + vars::styles::win_size[0], winpos[1] + vars::styles::win_size[1] - endtab_h + 1.f), ImGui::GetColorU32(ImGuiCol_Separator));
    
    ImGui::PopStyleColor();

    // Main block
    ImGui::SetCursorPos(ImVec2(vars::styles::margin, maintab_h+1.f+vars::styles::margin));
    ImGui::BeginChild(xorstr("fatal_main"), ImVec2(vars::styles::win_size[0] - vars::styles::margin * 2, vars::styles::win_size[1] - endtab_h - maintab_h - 2.f - vars::styles::margin * 2), ImGuiChildFlags_None, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    
    float row_width = (vars::styles::win_size[0]) / 3 - vars::styles::margin * 1.3f;
    float row_height = vars::styles::win_size[1] - endtab_h - maintab_h - 2.f - vars::styles::margin * 7;
    float vis_small_h = row_height / 5 - vars::styles::margin * 2;

    ImVec2 checkboxSize = ImVec2(row_width - vars::styles::margin * 2, checkboxSizeEx*dpi_scales_num[vars::settings::misc::dpi_scale]);
    float check_size = checkboxSizeEx * dpi_scales_num[vars::settings::misc::dpi_scale];

    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(vars::colors::child_bg[0], vars::colors::child_bg[1], vars::colors::child_bg[2], vars::colors::child_bg[3]));
    if (curtab == 0) {
        if (ImGui::BeginTabBar(xorstr("aim_tabbar"), ImGuiTabBarFlags_None))
        {
            if (ImGui::BeginTabItem(xorstr("Aimbot")))
            {
                ImGui::SetCursorPosY(maintab_h);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(vars::styles::margin, vars::styles::margin));

                ImGui::BeginChild(xorstr("aim_row_1"), ImVec2(row_width, row_height), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
                add_child_label(ImGui::GetCursorScreenPos(), "WEAPON", curtab);

                ImGui::SliderInt(xorstr("Hit-chance"), &vars::settings::rage::hitchance, 0, 100, "%d%%", ImGuiSliderFlags_FixedWidth | ImGuiSliderFlags_AlwaysClamp, checkboxSize[0]);
                // Also SliderFloat works
                ImGui::SliderInt(xorstr("Pointscale"), &vars::settings::rage::pointscale, 0, 100, "%d%%", ImGuiSliderFlags_FixedWidth | ImGuiSliderFlags_AlwaysClamp, checkboxSize[0]);
                ImGui::SliderInt(xorstr("Min-damage"), &vars::settings::rage::mindamage, 0, 150, "%d hp", ImGuiSliderFlags_FixedWidth | ImGuiSliderFlags_AlwaysClamp, checkboxSize[0]);
                
                const char* hit_bones[] = { "Head","Chest","Stomach","Arms","Legs","Feet"};
                ImGui::Combo(xorstr("Hitboxes"), &vars::settings::rage::hitbox, hit_bones, 6, -1, checkboxSize[0]);
                ImGui::Combo(xorstr("Multipoint"), &vars::settings::rage::multipoint, hit_bones, 6, -1, checkboxSize[0]);
                const char* select_target[] = { "Crosshair","FOV","Health","Distance"};
                ImGui::Combo(xorstr("Target selection"), &vars::settings::rage::targetsel, select_target, 4, -1, checkboxSize[0]);
                
                ImGui::EndChild();

                ImGui::SameLine(row_width+vars::styles::margin);

                ImGui::BeginChild(xorstr("aim_row_2"), ImVec2(row_width, row_height), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
                add_child_label(ImGui::GetCursorScreenPos(), "EXTRA", curtab);
                
                imgui_checkbox_bind(ICON_FA_COG, "Autostop", &vars::settings::rage::autostop, &vars::settings::rage::autostop_bind, row_width);

                ImGui::CheckboxSized(xorstr("Autoscope"), &vars::settings::rage::autoscope, checkboxSize);
                ImGui::CheckboxSized(xorstr("Ignore limbs on moving"), &vars::settings::rage::ignore_limbs, checkboxSize);
                ImGui::CheckboxSized(xorstr("Autorevolver"), &vars::settings::rage::autorevolver, checkboxSize);

                ImGui::EndChild();

                ImGui::SameLine((row_width + vars::styles::margin)*2);

                ImGui::BeginChild(xorstr("aim_row_3"), ImVec2(row_width, row_height), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
                add_child_label(ImGui::GetCursorScreenPos(), "GENERAL", curtab);

                

                ImGui::CheckboxSized(xorstr("Aimbot"), &vars::settings::rage::aimbot, ImVec2(checkboxSize[0], check_size));
                ImGui::CheckboxSized(xorstr("Silent aim"), &vars::settings::rage::silent, ImVec2(checkboxSize[0], check_size));
                ImGui::SliderInt(xorstr("Maximum fov"), &vars::settings::rage::maxfov, 0, 180, "%d deg", ImGuiSliderFlags_FixedWidth | ImGuiSliderFlags_AlwaysClamp, checkboxSize[0]);
                ImGui::CheckboxSized(xorstr("Autofire"), &vars::settings::rage::autofire, ImVec2(checkboxSize[0], check_size));
                ImGui::CheckboxSized(xorstr("Delay shot"), &vars::settings::rage::delay_shot, ImVec2(checkboxSize[0], check_size));
                ImGui::CheckboxSized(xorstr("Duck peek assist"), &vars::settings::rage::duck_peek, ImVec2(checkboxSize[0], check_size));
                ImGui::CheckboxSized(xorstr("Force bodyaim"), &vars::settings::rage::force_bodyaim, ImVec2(checkboxSize[0], check_size));
                ImGui::CheckboxSized(xorstr("Force shoot"), &vars::settings::rage::force_shoot, ImVec2(checkboxSize[0], check_size));
                ImGui::CheckboxSized(xorstr("Headshot only"), &vars::settings::rage::headshot_only, ImVec2(checkboxSize[0], check_size));
                ImGui::CheckboxSized(xorstr("Knife bot"), &vars::settings::rage::knifebot, ImVec2(checkboxSize[0], check_size));
                ImGui::CheckboxSized(xorstr("Zeus bot"), &vars::settings::rage::zeusbot, ImVec2(checkboxSize[0], check_size));


                imgui_checkbox_bind(ICON_FA_COG "#", "Nospread", &vars::settings::rage::nospread, &vars::settings::rage::nospread_bind, row_width);

                imgui_checkbox_bind(ICON_FA_COG "##", "Doubletap", &vars::settings::rage::doubletap, &vars::settings::rage::doubletap_bind, row_width);
                

                /*ImGui::CheckboxSized(xorstr("ColorPicker"), &vars::isJopa123, ImVec2(row_width - vars::styles::margin * 3 - check_size, check_size));
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
                ImGui::PopFont();*/
                ImGui::EndChild();

                ImGui::PopStyleVar();

                ImGui::EndTabItem();

            }
            if (ImGui::BeginTabItem(xorstr("Anti-Aim")))
            {
                ImGui::SetCursorPosY(maintab_h);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(vars::styles::margin, vars::styles::margin));

                ImGui::BeginChild(xorstr("aa_row_1"), ImVec2(row_width, row_height), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
                add_child_label(ImGui::GetCursorScreenPos(), "ANGLES", curtab);

                ImGui::CheckboxSized(xorstr("Anti-aim"), &vars::settings::rage::aa_enable, ImVec2(checkboxSize[0], check_size));
                const char* pitches[] = { "None","Down","Up","Zero","Custom"};
                ImGui::Combo(xorstr("Pitch"), &vars::settings::rage::pitch, pitches, 5, -1, checkboxSize[0]);
                if (vars::settings::rage::pitch == 4) {
                    ImGui::SliderInt(xorstr("Custom Pitch"), &vars::settings::rage::pitch_custom, -180, 180, "%d deg", ImGuiSliderFlags_FixedWidth | ImGuiSliderFlags_AlwaysClamp, checkboxSize[0]);
                }

                const char* yaws[] = { "None","Backwards","Zero","Random","Custom"};
                ImGui::Combo(xorstr("Yaw"), &vars::settings::rage::yaw, yaws, 5, -1, checkboxSize[0]);
                if (vars::settings::rage::yaw == 4) {
                    ImGui::SliderInt(xorstr("Custom Yaw"), &vars::settings::rage::yaw_custom, -180, 180, "%d deg", ImGuiSliderFlags_FixedWidth | ImGuiSliderFlags_AlwaysClamp, checkboxSize[0]);
                }
                else {
                    ImGui::Spacing();
                }
                ImGui::CheckboxSized(xorstr("Hide shot"), &vars::settings::rage::hideshot, ImVec2(checkboxSize[0], check_size));

                ImGui::EndChild();

                ImGui::SameLine(row_width + vars::styles::margin);

                ImGui::BeginChild(xorstr("aa_row_2"), ImVec2(row_width, row_height), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
                add_child_label(ImGui::GetCursorScreenPos(), "FAKELAG", curtab);

                ImGui::CheckboxSized(xorstr("Fakelag"), &vars::settings::rage::fake_enable, ImVec2(checkboxSize[0], check_size));
                const char* fakes[] = { "None","Always on","Adaptive" };
                ImGui::Combo(xorstr("Mode"), &vars::settings::rage::fake_mode, fakes, 3, -1, checkboxSize[0]);
                if (vars::settings::rage::fake_mode != 0) {
                    ImGui::SliderInt(xorstr("Limit"), &vars::settings::rage::fake_limit, 1, 14, "%d", ImGuiSliderFlags_FixedWidth | ImGuiSliderFlags_AlwaysClamp, checkboxSize[0]);
                }

                ImGui::EndChild();

                ImGui::PopStyleVar();

                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        draw_over_items();
    }
    else if (curtab == 2)
    {
        if (ImGui::BeginTabBar(xorstr("vis_tabbar"), ImGuiTabBarFlags_None))
        {
            if (ImGui::BeginTabItem(xorstr("Local"))) {

                ImGui::SetCursorPosY(maintab_h);

                ImGui::BeginChild(xorstr("vis_sect_1"), ImVec2(row_width, vis_small_h * 2), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
                add_child_label(ImGui::GetCursorScreenPos(), "HANDS", curtab);

                imgui_checkbox_col(xorstr("Hands chams"), &vars::settings::visuals::hands_chams, vars::settings::visuals::hands_chams_col, row_width);
                ImGui::Combo(xorstr("Type"), &vars::settings::visuals::hands_chams_type, chams_types, 5, -1, checkboxSize[0]);
                ImGui::Spacing();
                ImGui::CheckboxSized(xorstr("Draw original model"), &vars::settings::visuals::hands_chams_o, ImVec2(checkboxSize[0], check_size));

                ImGui::EndChild();

                ImGui::SetCursorPos(ImVec2(0.f, vis_small_h * 2 + maintab_h + vars::styles::margin*1.5f));

                ImGui::BeginChild(xorstr("vis_sect_2"), ImVec2(row_width, vis_small_h * 2), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
                add_child_label(ImGui::GetCursorScreenPos(), "GENERAL", curtab);

                ImGui::CheckboxSized(xorstr("FOV Changer"), &vars::settings::visuals::enable_fov, ImVec2(checkboxSize[0], check_size));
                
                if (vars::settings::visuals::enable_fov) {
                    ImGui::SliderInt(xorstr("FOV"), &vars::settings::visuals::fov, 80, 240, "%d deg", ImGuiSliderFlags_FixedWidth | ImGuiSliderFlags_AlwaysClamp, checkboxSize[0]);
                }

                ImGui::EndChild();

                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem(xorstr("Player"))) {
                ImGui::SetCursorPosY(maintab_h);

                ImGui::BeginChild(xorstr("vis_sect_1"), ImVec2(row_width, vis_small_h*2), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
                add_child_label(ImGui::GetCursorScreenPos(), "GENERAL", curtab);

                ImGui::CheckboxSized(xorstr("Enabled"), &vars::settings::visuals::enabled, ImVec2(checkboxSize[0], check_size));
                ImGui::SliderInt(xorstr("Draw distance"), &vars::settings::visuals::distance, 0, 100000, "%d m", ImGuiSliderFlags_FixedWidth | ImGuiSliderFlags_AlwaysClamp, checkboxSize[0]);

                ImGui::EndChild();

                ImGui::SetCursorPosY(ImGui::GetCursorPosY()+vars::styles::margin);

                ImGui::BeginChild(xorstr("vis_sect_2"), ImVec2(row_width, row_height-vis_small_h*2-vars::styles::margin*1.3f), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
                add_child_label(ImGui::GetCursorScreenPos(), "ESP", curtab);

                imgui_checkbox_col(xorstr("Name"), &vars::settings::visuals::name, vars::settings::visuals::name_col, row_width);
                imgui_checkbox_col(xorstr("Box"), &vars::settings::visuals::box, vars::settings::visuals::box_col, row_width);
                imgui_checkbox_col(xorstr("Health"), &vars::settings::visuals::health, vars::settings::visuals::health_col, row_width);
                if (vars::settings::visuals::health) {
                    ImGui::CheckboxSized(xorstr("Health Text"), &vars::settings::visuals::health_text, ImVec2(checkboxSize[0], check_size));
                    ImGui::CheckboxSized(xorstr("Health Bar"), &vars::settings::visuals::health_bar, ImVec2(checkboxSize[0], check_size));
                }
                imgui_checkbox_col(xorstr("Weapon"), &vars::settings::visuals::weapon_name, vars::settings::visuals::weapon_col, row_width);
                imgui_checkbox_col(xorstr("Armor"), &vars::settings::visuals::armor, vars::settings::visuals::armor_col, row_width);
                imgui_checkbox_col(xorstr("Skeleton"), &vars::settings::visuals::skeleton, vars::settings::visuals::skeleton_col, row_width);

                ImGui::EndChild();

                ImGui::SetCursorPos(ImVec2(row_width+vars::styles::margin, maintab_h));

                ImGui::BeginChild(xorstr("vis_sect_3"), ImVec2(row_width, row_height), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
                add_child_label(ImGui::GetCursorScreenPos(), "PREVIEW", curtab);
                ImGui::SetCursorPos(ImVec2(vars::styles::margin * 4, vars::styles::margin * 4 + check_size));
                ImVec2 img_pos = ImGui::GetCursorScreenPos();
                ImVec2 img_size = ImVec2(row_width - vars::styles::margin * 8, row_height - vars::styles::margin * 8 - check_size);
                imgui_drawimage("shaxter_image", img_size);
                display_vis_preview(ImVec2(img_pos.x, img_pos.y - check_size), ImVec2(img_size.x, img_size.y + check_size));
                
                ImGui::EndChild();

                ImGui::SetCursorPos(ImVec2(row_width*2 + vars::styles::margin*2, maintab_h));

                ImGui::BeginChild(xorstr("vis_sect_4"), ImVec2(row_width, vis_small_h*2.5f), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
                add_child_label(ImGui::GetCursorScreenPos(), "MODEL", curtab);
                
                imgui_checkbox_col(xorstr("Visible chams"), &vars::settings::visuals::chams_vis, vars::settings::visuals::chams_vis_col, row_width);
                
                ImGui::Combo(xorstr("Type"), &vars::settings::visuals::chams_vis_type, chams_types, 5, -1, checkboxSize[0]);

                ImGui::Spacing();

                imgui_checkbox_col(xorstr("Invisible chams"), &vars::settings::visuals::chams_invis, vars::settings::visuals::chams_invis_col, row_width);
                
                ImGui::Combo(xorstr("Type##"), &vars::settings::visuals::chams_invis_type, chams_types, 5, -1, checkboxSize[0]);
                
                ImGui::Spacing();

                ImGui::CheckboxSized(xorstr("Draw original model"), &vars::settings::visuals::enemy_chams_o, ImVec2(checkboxSize[0], check_size));

                ImGui::EndChild();


                ImGui::SetCursorPos(ImVec2(row_width * 2 + vars::styles::margin * 2, maintab_h + vis_small_h * 2.5f + vars::styles::margin*1.5f));

                ImGui::BeginChild(xorstr("vis_sect_5"), ImVec2(row_width, vis_small_h * 2.f), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
                add_child_label(ImGui::GetCursorScreenPos(), "BACKTRACK", curtab);

                imgui_checkbox_col(xorstr("Visible chams##"), &vars::settings::visuals::chams_backtrack_vis, vars::settings::visuals::chams_backtrack_vis_col, row_width);
                
                ImGui::Combo(xorstr("Type##1"), &vars::settings::visuals::chams_backtrack_vis_type, chams_types, 5, -1, checkboxSize[0]);

                ImGui::Spacing();

                imgui_checkbox_col(xorstr("Invisible chams##"), &vars::settings::visuals::chams_backtrack_invis, vars::settings::visuals::chams_backtrack_invis_col, row_width);

                ImGui::Combo(xorstr("Type##2"), &vars::settings::visuals::chams_backtrack_invis_type, chams_types, 5, -1, checkboxSize[0]);
                
                ImGui::EndChild();

                ImGui::SetCursorPos(ImVec2(row_width * 2 + vars::styles::margin * 2, maintab_h + vis_small_h * 4.5f + vars::styles::margin * 3.f));

                ImGui::BeginChild(xorstr("vis_sect_6"), ImVec2(row_width, vis_small_h * 2.f - vars::styles::margin*3.5f), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
                add_child_label(ImGui::GetCursorScreenPos(), "EXTRA", curtab);

                imgui_checkbox_col(xorstr("Glow"), &vars::settings::visuals::glow, vars::settings::visuals::glow_col, row_width);

                ImGui::EndChild();

                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem(xorstr("Entity"))) {

                ImGui::SetCursorPosY(maintab_h);

                ImGui::BeginChild(xorstr("vis_sect_1"), ImVec2(row_width, vis_small_h * 2), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
                add_child_label(ImGui::GetCursorScreenPos(), "GENERAL", curtab);

                ImGui::CheckboxSized(xorstr("Enabled"), &vars::settings::visuals::ent_enabled, ImVec2(checkboxSize[0], check_size));
                ImGui::SliderInt(xorstr("Draw distance"), &vars::settings::visuals::ent_distance, 0, 100000, "%d m", ImGuiSliderFlags_FixedWidth | ImGuiSliderFlags_AlwaysClamp, checkboxSize[0]);

                ImGui::EndChild();

                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + vars::styles::margin);

                ImGui::BeginChild(xorstr("vis_sect_2"), ImVec2(row_width, row_height - vis_small_h * 2 - vars::styles::margin * 1.3f), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
                add_child_label(ImGui::GetCursorScreenPos(), "ESP", curtab);

                imgui_checkbox_col(xorstr("Name"), &vars::settings::visuals::ent_name, vars::settings::visuals::ent_name_col, row_width);
                imgui_checkbox_col(xorstr("Box"), &vars::settings::visuals::ent_box, vars::settings::visuals::ent_box_col, row_width);
                imgui_checkbox_col(xorstr("Health"), &vars::settings::visuals::ent_health, vars::settings::visuals::ent_health_col, row_width);
                if (vars::settings::visuals::ent_health) {
                    ImGui::CheckboxSized(xorstr("Health Text"), &vars::settings::visuals::ent_health_text, ImVec2(checkboxSize[0], check_size));
                    ImGui::CheckboxSized(xorstr("Health Bar"), &vars::settings::visuals::ent_health_bar, ImVec2(checkboxSize[0], check_size));
                }
                ImGui::EndChild();

                ImGui::SetCursorPos(ImVec2(row_width + vars::styles::margin, maintab_h));

                ImGui::BeginChild(xorstr("vis_sect_3"), ImVec2(row_width, row_height), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
                add_child_label(ImGui::GetCursorScreenPos(), "PREVIEW", curtab);
                ImGui::SetCursorPos(ImVec2(vars::styles::margin * 4, vars::styles::margin * 4 + row_height * 0.25f));
                ImVec2 img_pos = ImGui::GetCursorScreenPos();
                ImVec2 img_size = ImVec2(row_width - vars::styles::margin * 8, row_height*0.5f - vars::styles::margin * 8 - check_size);
                imgui_drawimage("printer_image", img_size);
                display_vis_preview_ent(ImVec2(img_pos.x, img_pos.y - check_size), ImVec2(img_size.x, img_size.y + check_size));

                ImGui::EndChild();

                ImGui::SetCursorPos(ImVec2(row_width * 2 + vars::styles::margin * 2, maintab_h));

                ImGui::BeginChild(xorstr("vis_sect_4"), ImVec2(row_width, vis_small_h * 4.f + vars::styles::margin * 2), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
                add_child_label(ImGui::GetCursorScreenPos(), "MODEL", curtab);

                imgui_checkbox_col(xorstr("Visible chams"), &vars::settings::visuals::ent_chams_vis, vars::settings::visuals::ent_chams_vis_col, row_width);
                const char* chams_types[] = { "Flat", "Metal", "Glass", "Wireframe", "Glow" };
                ImGui::Combo(xorstr("Type"), &vars::settings::visuals::ent_chams_vis_type, chams_types, 5, -1, checkboxSize[0]);

                ImGui::Spacing();

                imgui_checkbox_col(xorstr("Invisible chams"), &vars::settings::visuals::ent_chams_invis, vars::settings::visuals::ent_chams_invis_col, row_width);

                ImGui::Combo(xorstr("Type##"), &vars::settings::visuals::ent_chams_invis_type, chams_types, 5, -1, checkboxSize[0]);
                
                ImGui::Spacing();

                ImGui::CheckboxSized(xorstr("Draw original model"), &vars::settings::visuals::ent_chams_o, ImVec2(checkboxSize[0], check_size));

                ImGui::EndChild();

                ImGui::SetCursorPos(ImVec2(row_width * 2 + vars::styles::margin * 2, maintab_h + vis_small_h * 4.f + vars::styles::margin * 4.f));

                ImGui::BeginChild(xorstr("vis_sect_6"), ImVec2(row_width, vis_small_h * 2.f - vars::styles::margin), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
                add_child_label(ImGui::GetCursorScreenPos(), "EXTRA", curtab);

                imgui_checkbox_col(xorstr("Glow"), &vars::settings::visuals::ent_glow, vars::settings::visuals::ent_glow_col, row_width);

                ImGui::EndChild();

                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem(xorstr("World"))) {

                ImGui::SetCursorPosY(maintab_h);

                ImGui::BeginChild(xorstr("vis_sect_1"), ImVec2(row_width, vis_small_h * 2), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
                add_child_label(ImGui::GetCursorScreenPos(), "GENERAL", curtab);

                imgui_checkbox_col(xorstr("World color"), &vars::settings::visuals::enable_colormod, vars::settings::visuals::colormod_col, row_width);
                
                ImGui::CheckboxSized(xorstr("Custom sky"), &vars::settings::visuals::enable_skybox, ImVec2(checkboxSize[0], check_size));
                
                if (vars::settings::visuals::enable_skybox) {
                    ImGui::Combo(xorstr("Skybox"), &vars::settings::visuals::skybox, skyboxes, 5, -1, checkboxSize[0]);
                    ImGui::Spacing();
                }

                ImGui::CheckboxSized(xorstr("Fullbright"), &vars::settings::visuals::fullbright, ImVec2(checkboxSize[0], check_size));

                ImGui::EndChild();

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
        draw_over_items();

    }
    else if (curtab == 3) {
        
        ImGui::SetCursorPosY(vars::styles::margin);

        ImGui::BeginChild(xorstr("misc_sect_1"), ImVec2(row_width, vis_small_h*2 + maintab_h - vars::styles::margin), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
        add_child_label(ImGui::GetCursorScreenPos(), "GENERAL", curtab);

        imgui_checkbox_bind(ICON_FA_COG "##1", "Third person", &vars::settings::misc::thirdperson, &vars::settings::misc::thirdperson_bind, row_width);
        if (vars::settings::misc::thirdperson) {
            ImGui::SliderInt(xorstr("Distance"), &vars::settings::misc::thirdperson_distance, 0, 1000, "%d m", ImGuiSliderFlags_FixedWidth | ImGuiSliderFlags_AlwaysClamp, checkboxSize[0]);
        }
        imgui_checkbox_bind(ICON_FA_COG "##2", "Auto peek", &vars::settings::misc::autopeek, &vars::settings::misc::autopeek_bind, row_width);
        imgui_checkbox_bind(ICON_FA_COG "##3", "Free camera", &vars::settings::misc::freecam, &vars::settings::misc::freecam_bind, row_width);
        if (vars::settings::misc::freecam) {
            ImGui::SliderInt(xorstr("Speed"), &vars::settings::misc::freecam_distance, 0, 100, "%d m/s", ImGuiSliderFlags_FixedWidth | ImGuiSliderFlags_AlwaysClamp, checkboxSize[0]);
        }
        ImGui::EndChild();

        ImGui::SetCursorPos(ImVec2(0.f,vis_small_h*2 + maintab_h + vars::styles::margin*1.5f));

        ImGui::BeginChild(xorstr("misc_sect_2"), ImVec2(row_width, vis_small_h + maintab_h - vars::styles::margin), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
        add_child_label(ImGui::GetCursorScreenPos(), "HITSOUND", curtab);
        const char* hit_sounds[] = { "Metal", "Bell", "Phonk", "Beep"};
        ImGui::Combo(xorstr("Hit Sound"), &vars::settings::misc::hitsound, hit_sounds, 4, -1, checkboxSize[0]);
        ImGui::Combo(xorstr("Kill Sound"), &vars::settings::misc::killsound, hit_sounds, 4, -1, checkboxSize[0]);

        ImGui::EndChild();

        ImGui::SetCursorPos(ImVec2(0.f, vis_small_h*3 + maintab_h + vars::styles::margin * 6.5f));

        ImGui::BeginChild(xorstr("misc_sect_3"), ImVec2(row_width, vis_small_h*2 + maintab_h - vars::styles::margin), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
        add_child_label(ImGui::GetCursorScreenPos(), "MOVEMENT", curtab);

        ImGui::CheckboxSized(xorstr("Bunny hop"), &vars::settings::misc::bhop, ImVec2(checkboxSize[0], check_size));
        ImGui::CheckboxSized(xorstr("Auto strafe"), &vars::settings::misc::autostrafe, ImVec2(checkboxSize[0], check_size));
        if (vars::settings::misc::autostrafe) {
            const char* strafe_types[] = { "Legit", "Rage", "Multi-Directional"};
            ImGui::Combo(xorstr("Strafe"), &vars::settings::misc::autostrafe_type, strafe_types, 3, -1, checkboxSize[0]);
        }

        ImGui::EndChild();

        ImGui::SetCursorPos(ImVec2(row_width + vars::styles::margin, maintab_h - vars::styles::margin*3.5f));

        ImGui::BeginChild(xorstr("misc_sect_4"), ImVec2(row_width, row_height+maintab_h-vars::styles::margin), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
        add_child_label(ImGui::GetCursorScreenPos(), "EXTRA", curtab);

       

        ImGui::EndChild();

        ImGui::SameLine();
        ImGui::SetCursorPosX(row_width*2 + vars::styles::margin*2);

        ImGui::BeginChild(xorstr("misc_sect_5"), ImVec2(row_width, row_height + maintab_h - vars::styles::margin), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
        // TIP: This trick fixes overlapping text while user scrolls down
        add_child_label(ImVec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y + ImGui::GetScrollY()), "MENU", curtab);

        const char* dpi_scales[] = { "85%", "100%", "125%", "150%", "175%", "200%"};
        if (ImGui::Combo(xorstr("DPI Scale"), &vars::settings::misc::dpi_scale, dpi_scales, 6, -1, checkboxSize[0])) {
            float new_dpi = dpi_scales_num[vars::settings::misc::dpi_scale];
            pstyle->SetDPIScale(new_dpi);
            vars::styles::win_size.x = vars::styles::win_size_ex.x * new_dpi;
            vars::styles::win_size.y = vars::styles::win_size_ex.y * new_dpi;
            io.FontGlobalScale = new_dpi;
        }
        

        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);

        ImGui::ColorEdit4(xorstr("Window background"), vars::colors::window_bg, ImGuiColorEditFlags_NoInputs);
        ImGui::ColorEdit4(xorstr("Child background"), vars::colors::child_bg, ImGuiColorEditFlags_NoInputs);
        ImGui::ColorEdit4(xorstr("Inactive text"), vars::colors::text_inactive, ImGuiColorEditFlags_NoInputs);
        ImGui::ColorEdit4(xorstr("Text"), vars::colors::text, ImGuiColorEditFlags_NoInputs);
        ImGui::ColorEdit4(xorstr("Accent"), vars::colors::selection, ImGuiColorEditFlags_NoInputs);
        ImGui::ColorEdit4(xorstr("Accent highlight"), vars::colors::selection_high, ImGuiColorEditFlags_NoInputs);
        ImGui::ColorEdit4(xorstr("Border"), vars::colors::border, ImGuiColorEditFlags_NoInputs);
        ImGui::ColorEdit4(xorstr("Button"), vars::colors::button, ImGuiColorEditFlags_NoInputs);
        ImGui::ColorEdit4(xorstr("Hovered button"), vars::colors::button_hover, ImGuiColorEditFlags_NoInputs);
        ImGui::ColorEdit4(xorstr("Clicked button"), vars::colors::button_clicked, ImGuiColorEditFlags_NoInputs);

        ImGui::ColorEdit4(xorstr("Hovered background"), vars::colors::frame_bg_hover, ImGuiColorEditFlags_NoInputs);
        ImGui::ColorEdit4(xorstr("Active background"), vars::colors::frame_bg_active, ImGuiColorEditFlags_NoInputs);

        ImGui::SliderFloat(xorstr("Rounding"), &vars::styles::rounding, 0.f, 20.f, "%.2f px", ImGuiSliderFlags_FixedWidth | ImGuiSliderFlags_AlwaysClamp, checkboxSize[0]);
        ImGui::SliderFloat(xorstr("Margin"), &vars::styles::margin, 0.f, 15.f, "%.2f px", ImGuiSliderFlags_FixedWidth | ImGuiSliderFlags_AlwaysClamp, checkboxSize[0]);
        ImGui::SliderFloat(xorstr("Animation time"), &vars::styles::anim_speed, 0.f, 5.f, "%.2f s", ImGuiSliderFlags_FixedWidth | ImGuiSliderFlags_AlwaysClamp, checkboxSize[0]);
        ImGui::SliderFloat(xorstr("Padding X"), &vars::styles::tab_padding.x, 0.f, 15.f, "%.2f px", ImGuiSliderFlags_FixedWidth | ImGuiSliderFlags_AlwaysClamp, checkboxSize[0]);
        ImGui::SliderFloat(xorstr("Padding Y"), &vars::styles::tab_padding.y, 0.f, 15.f, "%.2f px", ImGuiSliderFlags_FixedWidth | ImGuiSliderFlags_AlwaysClamp, checkboxSize[0]);
        
        static bool enable_sfx = true;
        ImGui::CheckboxSized(xorstr("Play SFX"), &enable_sfx, ImVec2(checkboxSize[0], check_size));
        vars::styles::play_sfx = enable_sfx ? 1.f : 0.f;

        ImGui::SliderFloat(xorstr("Scrollbar width"), &vars::styles::scroll_width, 0.f, 25.f, "%.2f px", ImGuiSliderFlags_FixedWidth | ImGuiSliderFlags_AlwaysClamp, checkboxSize[0]);

        
        ImGui::PopStyleVar();

        ImGui::EndChild();

        draw_over_items();
    }
    else if (curtab == 4) {
        ImGui::SetCursorPosY(vars::styles::margin);

        ImGui::BeginChild(xorstr("lua_sect_1"), ImVec2(row_width, row_height + maintab_h - vars::styles::margin), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
        add_child_label(ImGui::GetCursorScreenPos(), "FILES", curtab);

        const char* scripts[] = { "script.lua", "truckachtech.lua", "exploits.lua", "autofarm.lua" };

        ImGui::BeginListBox("##lua_list", ImVec2(-FLT_MIN, row_height + maintab_h - vars::styles::margin * 4));

        for (size_t i = 0; i < sizeof(selected_script); i++)
        {
            if (ImGui::Selectable(scripts[i]))
                selected_script = i;
        }
        ImGui::Button(xorstr("Open folder"));

        ImGui::EndListBox();

        ImGui::EndChild();

        ImGui::SameLine();
        ImGui::SetCursorPosX(row_width+vars::styles::margin);

        ImGui::BeginChild(xorstr("lua_sect_2"), ImVec2(row_width*2, row_height + maintab_h - vars::styles::margin), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
        add_child_label(ImGui::GetCursorScreenPos(), "EDITOR", curtab);

        ImGui::Text(scripts[selected_script]);
        
        // LUA EDITOR HERE

        lua_editor.IsOverwrite() ? "Ovr" : "Ins",
        lua_editor.CanUndo() ? "*" : " ",

        lua_editor.Render("Lua Editor",ImVec2(row_width*2-vars::styles::margin * 2, row_height + maintab_h - vars::styles::margin*6 - 32.f));

        ImGui::Button(xorstr("Run script"), ImVec2(row_width, vars::styles::margin*2+16.f));

        ImGui::EndChild();

        draw_over_items();
    }
    else if (curtab == 5) {
        ImGui::SetCursorPosY(vars::styles::margin);

        ImGui::BeginChild(xorstr("skin_sect_1"), ImVec2(row_width, row_height + maintab_h - vars::styles::margin), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
        add_child_label(ImGui::GetCursorScreenPos(), "SKINS", curtab);

        const char* skins[] = { "Cool skin", "Rare skin", "Common skin", "Worst skin" };

        ImGui::InputText("Search", vars::settings::skins::skin_search, sizeof(vars::settings::skins::skin_search));

        ImGui::BeginListBox("##skin_list", ImVec2(-FLT_MIN, row_height + maintab_h - vars::styles::margin * 4 - check_size*2));

        std::string skin_search_str(vars::settings::skins::skin_search);
        std::transform(skin_search_str.begin(), skin_search_str.end(), skin_search_str.begin(), ::tolower);
        for (size_t i = 0; i < sizeof(vars::settings::skins::selected_skin); i++)
        {
            std::string cur_skin(skins[i]);
            std::transform(cur_skin.begin(), cur_skin.end(), cur_skin.begin(), ::tolower);
            
            if ((skin_search_str.empty() || cur_skin.find(skin_search_str) != std::string::npos) && ImGui::Selectable(skins[i]))
                vars::settings::skins::selected_skin = i;
        }

        ImGui::EndListBox();

        ImGui::EndChild();

        ImGui::SameLine();
        ImGui::SetCursorPosX(row_width + vars::styles::margin);

        ImGui::BeginChild(xorstr("skin_sect_2"), ImVec2(row_width, row_height + maintab_h - vars::styles::margin), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
        add_child_label(ImGui::GetCursorScreenPos(), "PAINT KIT", curtab);

        const char* paints[] = { "Paint 1", "Paint 2", "Paint 3", "Paint 4" };

        ImGui::InputText("Search", vars::settings::skins::paint_search, sizeof(vars::settings::skins::paint_search));

        ImGui::BeginListBox("##paint_list", ImVec2(-FLT_MIN, row_height + maintab_h - vars::styles::margin * 4 - check_size * 2));

        std::string paint_search_str(vars::settings::skins::paint_search);
        std::transform(paint_search_str.begin(), paint_search_str.end(), paint_search_str.begin(), ::tolower);
        for (size_t i = 0; i < sizeof(vars::settings::skins::selected_paint); i++)
        {
            std::string cur_paint(paints[i]);
            std::transform(cur_paint.begin(), cur_paint.end(), cur_paint.begin(), ::tolower);

            if ((paint_search_str.empty() || cur_paint.find(paint_search_str) != std::string::npos) && ImGui::Selectable(paints[i]))
                vars::settings::skins::selected_paint = i;
        }

        ImGui::EndListBox();

        ImGui::EndChild();

        ImGui::SameLine();
        ImGui::SetCursorPosX(row_width*2 + vars::styles::margin*2);

        ImGui::BeginChild(xorstr("skin_sect_3"), ImVec2(row_width, row_height + maintab_h - vars::styles::margin), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysUseWindowPadding);
        add_child_label(ImGui::GetCursorScreenPos(), "QUALITY", curtab);

        const char* wear_types[] = { "StarTrek", "Good", "Well-Worn", "Battle-Hardened" };

        ImGui::InputText("Search", vars::settings::skins::wear_search, sizeof(vars::settings::skins::wear_search));

        ImGui::BeginListBox("##wear_list", ImVec2(-FLT_MIN, row_height + maintab_h - vars::styles::margin * 4 - check_size * 2));

        std::string wear_search_str(vars::settings::skins::wear_search);
        std::transform(wear_search_str.begin(), wear_search_str.end(), wear_search_str.begin(), ::tolower);
        for (size_t i = 0; i < sizeof(vars::settings::skins::selected_paint); i++)
        {
            std::string cur_wear(wear_types[i]);
            std::transform(cur_wear.begin(), cur_wear.end(), cur_wear.begin(), ::tolower);

            if ((wear_search_str.empty() || cur_wear.find(wear_search_str) != std::string::npos) && ImGui::Selectable(wear_types[i]))
                vars::settings::skins::selected_paint = i;
        }

        ImGui::EndListBox();

        ImGui::EndChild();

        draw_over_items();
    }
    ImGui::PopStyleColor();

    
    ImGui::EndChild();

	ImGui::End();

    ImGui::PopFont();

	ImGui::PopStyleColor(27);
	ImGui::PopStyleVar(11);
}