/*
#pragma once
#include <SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <include/thirdparty/entt.hpp>
#include <iostream>
#include <include/GameContext/GameContext.h>
#include <glm/glm.hpp>

#include <include/GameContext/IO/controller.h>

#include <include/GameContext/client/Client.h>

int main() {
    GameContext context;
	context.run();

}
*/


#include <include/GameContext/server/interpreter/ConsoleInterpreter.h>
#include <include/GameContext/server/interpreter/InterpreterToken.h>
// Example usage
int main() {
    std::string input = "sv_cheats = ~foo";


    ConsoleInterpreter interpreter;

    std::vector<Token> tokens = Tokenizer::tokenize(input);
    //tokens = interpreter.expand(tokens);

    for (auto& token: tokens)
    {
        token = interpreter.expand(token);
        std::cout << token.to_string_debug() << "\n";
    }
    

    return 0;
}