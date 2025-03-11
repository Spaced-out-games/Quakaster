#pragma once
//#include <imgui.h>
//#include <backends/imgui_impl_sdl2.h>
//#include <backends/imgui_impl_opengl3.h>

struct UIContext;

struct UIBase
{
    bool always_visible = false;
    bool visible = 1;
    virtual void draw(UIContext*) = 0;
    
};