#pragma once

struct UIContext;

struct UIBase
{
    bool always_visible = false;
    bool visible = 1;
    virtual void draw(UIContext*) = 0;
    
};