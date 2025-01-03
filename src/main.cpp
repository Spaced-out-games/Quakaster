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
    std::string input = "test = 4 4";
    ConsoleInterpreter interpreter;

    std::vector<Token> tokens = Tokenizer::tokenize(input);
    tokens = interpreter.expand(tokens);

    for (auto& token: tokens)
    {
        std::cout << token.to_string() << "\n";
    }
    

    return 0;
}