#pragma once
#include <SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <include/entt.hpp>
#include <iostream>
#include "include/window.h"

#include "include/entity.h"
#include "include/consoleComponent.h"
#include "include/textComponent.h"
#include "include/res_shader.h"

//#include "include/gameApp.h"
#include "include/gameApp.h"
//#include <assimp/Importer.hpp>
int main() {
    
    gameApp app;
    app.run();
}