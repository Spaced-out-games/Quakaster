#pragma once

struct UIContext;

struct UIBase
{
    bool always_visible = 0;
    bool visible = 1;
    virtual void draw(UIContext*) = 0;
    
};