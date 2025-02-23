#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include "vars.h"
#include <iostream>
#include "xorstr.hpp"
#include <unordered_map>

#include "fonts/JosefinSansRegular.h"
#include "fonts/JosefinSansBold.h"
#include "fonts/FontIcon.h"
#include "text_editor/TextEditor.h"

void draw_menu(IDirect3DDevice9* device);
void load_fonts();
static TextEditor lua_editor;