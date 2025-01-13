#pragma once

struct UIContext;

struct UIBase
{
    bool visible = 1;
    virtual void draw(UIContext*) = 0;
    
};