#pragma once
namespace vars {
    inline bool isMenuOpen = true;

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

        inline float frame_bg_hover[4] = { window_bg[0] * 1.1f, window_bg[1] * 1.1f, window_bg[2] * 1.1f, 1.f };
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
        inline ImVec2 win_size_ex = { 848.f,588.f }; // Equals to win_size in code, needs for DPI scaling
        inline float scroll_width = 12.5f;
        inline float play_sfx = 1.f;
        inline float anim_speed = 0.1f;
    }

    namespace settings {
        namespace rage {
            // Aimbot
            inline int hitchance = 100;
            inline int pointscale = 34;
            inline int mindamage = 100;

            inline int hitbox = 0;
            inline int multipoint = 0;
            inline int targetsel = 0;

            inline bool autostop = false;
            inline ImGuiKey autostop_bind = ImGuiKey_None;

            inline bool autoscope = false;
            inline bool ignore_limbs = false;
            inline bool autorevolver = false;

            inline bool aimbot = false;
            inline bool silent = false;
            inline int maxfov = 0;

            inline bool autofire = false;
            inline bool delay_shot = false;
            inline bool duck_peek = false;
            inline bool force_bodyaim = false;
            inline bool force_shoot = false;
            inline bool headshot_only = false;

            inline bool knifebot = false;
            inline bool zeusbot = false;

            inline bool nospread = false;
            inline ImGuiKey nospread_bind = ImGuiKey_None;

            inline bool doubletap = false;
            inline ImGuiKey doubletap_bind = ImGuiKey_None;

            // Anti-aim
            inline bool aa_enable = false;
            inline int pitch = 0;
            inline int pitch_custom = 0;
            inline int yaw = 0;
            inline int yaw_custom = 0;
            inline bool hideshot = false;

            inline bool fake_enable = false;
            inline int fake_mode = 0;
            inline int fake_limit = 0;
        }
        namespace visuals {
            // LOCAL
            inline bool hands_chams = false;
            inline int hands_chams_type = 0;
            inline float hands_chams_col[4] = { 1.f, 1.f, 1.f, 1.f };
            inline bool hands_chams_o = false;

            inline bool enable_fov = false;
            inline int fov = 100;

            // PLAYER VIS
            inline bool enabled = false;

            inline bool name = false;
            inline float name_col[4] = { 1.f, 1.f, 1.f, 1.f };

            inline bool box = false;
            inline float box_col[4] = { 1.f, 1.f, 1.f, 1.f };

            inline bool health = false;
            inline bool health_text = true;
            inline bool health_bar = false;
            inline float health_col[4] = { 1.f, 1.f, 1.f, 1.f };

            inline bool weapon_name = false;
            inline float weapon_col[4] = { 1.f, 1.f, 1.f, 1.f };

            inline bool armor = false;
            inline float armor_col[4] = { 1.f, 1.f, 1.f, 1.f };

            inline bool skeleton = false;
            inline float skeleton_col[4] = { 1.f, 1.f, 1.f, 1.f };

            inline bool enemy_chams_o = false;

            inline bool chams_vis = false;
            inline int chams_vis_type = 0;
            inline float chams_vis_col[4] = { 1.f, 1.f, 1.f, 1.f };

            inline bool chams_invis = false;
            inline int chams_invis_type = 0;
            inline float chams_invis_col[4] = { 1.f, 1.f, 1.f, 1.f };

            inline bool chams_backtrack_vis = false;
            inline int chams_backtrack_vis_type = 0;
            inline float chams_backtrack_vis_col[4] = { 1.f, 1.f, 1.f, 1.f };

            inline bool chams_backtrack_invis = false;
            inline int chams_backtrack_invis_type = 0;
            inline float chams_backtrack_invis_col[4] = { 1.f, 1.f, 1.f, 1.f };

            inline bool glow = false;
            inline float glow_col[4] = { 1.f, 1.f, 1.f, 1.f };

            inline int distance = 0;

            // ENT VIS
            inline bool ent_enabled = false;

            inline bool ent_chams_o = false;

            inline bool ent_name = false;
            inline float ent_name_col[4] = { 1.f, 1.f, 1.f, 1.f };

            inline bool ent_box = false;
            inline float ent_box_col[4] = { 1.f, 1.f, 1.f, 1.f };

            inline bool ent_health = false;
            inline bool ent_health_text = true;
            inline bool ent_health_bar = false;
            inline float ent_health_col[4] = { 1.f, 1.f, 1.f, 1.f };

            inline bool ent_chams_vis = false;
            inline int ent_chams_vis_type = 0;
            inline float ent_chams_vis_col[4] = { 1.f, 1.f, 1.f, 1.f };

            inline bool ent_chams_invis = false;
            inline int ent_chams_invis_type = 0;
            inline float ent_chams_invis_col[4] = { 1.f, 1.f, 1.f, 1.f };

            inline bool ent_glow = false;
            inline float ent_glow_col[4] = { 1.f, 1.f, 1.f, 1.f };

            inline int ent_distance = 0;

            // WORLD
            inline bool enable_colormod = false;
            inline float colormod_col[4] = { 1.f, 1.f, 1.f, 1.f };

            inline bool enable_skybox = false;
            inline int skybox = 0;
            inline bool fullbright = false;
        }
        namespace misc {
            inline bool autopeek = false;
            inline ImGuiKey autopeek_bind = ImGuiKey_None;

            inline bool thirdperson = false;
            inline ImGuiKey thirdperson_bind = ImGuiKey_None;
            inline int thirdperson_distance = 0;

            inline bool freecam = false;
            inline ImGuiKey freecam_bind = ImGuiKey_None;
            inline int freecam_distance = 0;
            inline int hitsound = 0;
            inline int killsound = 0;

            inline bool bhop = false;
            inline bool autostrafe = false;
            inline int autostrafe_type = 0;

            inline int dpi_scale = 1;
        }
        namespace skins {
            inline int selected_skin = 0;
            inline char skin_search[128];

            inline int selected_paint = 0;
            inline char paint_search[128];

            inline int selected_wear = 0;
            inline char wear_search[128];
        }
    }
}