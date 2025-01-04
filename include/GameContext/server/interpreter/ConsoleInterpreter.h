#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <queue>
#include <functional>
#include <iostream>
#include "InterpreterToken.h" // Ensure this path is correct
#include <include/thirdparty/glm/glm.hpp>

struct ConsoleUI;

using convar = Token;

class ConsoleInterpreter {
public:
    // Constructor that initializes the convar map
    ConsoleInterpreter() {
        convars = {
            {"max_health", 100},
            {"player_name", "Hero"},
            {"spawn_point", glm::vec3(0, 0, 0)},
            {"foo", "test succsessful, bitch"}
        };
    }
    bool is_command(const Token& token) { return true; }

    Token expand(Token& token) {
        if (token.needs_read) {
            auto it = convars.find(token.snippet); // Use the snippet to look up the value
            if (it != convars.end()) {
                // If found, create a new token with the corresponding value
                return Token{ std::get<std::string>(it->second), TOKEN_IDENTIFIER };
            }
        }
        if (token.is_container() || token.token_type == TOKEN_ACCENT_CHAR) {
            std::vector<Token>& container = std::get<std::vector<Token>>(token.value);
            for (size_t i = 0; i < container.size(); ++i) {
                container[i] = expand(container[i]); // Expand each token inside the container
            }
            return Token{ token.snippet, container, token.token_type }; // Return updated container
        }
        return token;
    }

    void execute(std::string& input)
    {
        // tokenize
        bool set = 0;
        size_t index = 0;
        size_t end_index;
        std::vector<Token> tokens = Tokenizer::tokenize(input);
        for (Token& token : tokens)
        {
            token = expand(token);
            if (token.is_EOL() && !set)
            {
                end_index = index;
                set = true;
            }
            index++;
        }
        Token& command = tokens[0];

        // find the console command and pipe

        auto& it = convars.find(command.snippet);
        if (it != convars.end()) {
            token_value_t& variant = it->second;

            // pipe in tokens 1 through end_index
            
        }

    }

    // Method to expand a single token
    /*Token expand(const Token& token) {


        //if(token.token_type == TOKEN_ASSIGN) {return token;}

        // If the token is an identifier, look up its value in the convar map
        if (token.token_type == TOKEN_IDENTIFIER) {
            auto it = convars.find(token.snippet); // Use the snippet to look up the value
            if (it != convars.end()) {
                // If found, create a new token with the corresponding value
                size_t offset = 0;
                return Tokenizer::tokenize_once(it->second, offset); // Create a new token with the expanded value
            }

            // If not found, return the original token
            if (is_command(token))
            {
                
                return token;
            }

            
        }
        else if (token.token_type == TOKEN_BRACKET_CONTAINER || token.token_type == TOKEN_PARENTHESES_CONTAINER || token.token_type == TOKEN_ASSIGN) {
            // If the token is a container, expand its nested tokens
            auto nested_tokens = std::get<std::vector<Token>>(token.value); // Get the nested tokens
            std::vector<Token> expanded_nested_tokens;

            for (const auto& nested_token : nested_tokens) {
                expanded_nested_tokens.push_back(expand(nested_token)); // Recursively expand each nested token
            }

            // Create a new token for the container with the expanded nested tokens
            return Token(token.snippet, expanded_nested_tokens, token.token_type);
        }

        // For non-identifier tokens, return them directly
        return token;
    }*/

private:
    std::unordered_map<std::string, token_value_t> convars; // Map of convars
};