#pragma once


struct UIBase
{
    bool visible = 1;
    virtual void draw() = 0;
    
};