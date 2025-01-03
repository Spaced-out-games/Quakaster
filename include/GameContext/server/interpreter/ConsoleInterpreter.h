#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <queue>
#include <functional>
#include <iostream>
#include "InterpreterToken.h" // Ensure this path is correct

struct ConsoleUI;

using convar = Token;
using console_fn = std::function<void(std::queue<Token>)>;

class ConsoleInterpreter {
public:
    // Constructor that initializes the convar map
    ConsoleInterpreter() {
        convars = {
            {"max_health", "100"},
            {"player_name", "Hero"},
            {"spawn_point", "(0, 0, 0)"}
        };
    }

    // Method to expand a vector of tokens
    std::vector<Token> expand(const std::vector<Token>& tokens) {
        std::vector<Token> expanded_tokens;

        for (const auto& token : tokens) {
            if (token.token_type == TOKEN_IDENTIFIER) {
                // If the token is an identifier, look up its value in the convar map
                auto it = convars.find(token.snippet); // Correctly use convars
                if (it != convars.end()) {
                    // If found, create a new token with the corresponding value
                    expanded_tokens.push_back(Token(it->second)); // Keep the same type or adjust as needed
                }
                else {
                    // If not found, keep the original token
                    expanded_tokens.push_back(token);
                }
            }
            else if (token.token_type == TOKEN_BRACKET_CONTAINER || token.token_type == TOKEN_PARENTHESES_CONTAINER) {
                // If the token is a container, expand its nested tokens
                auto nested_tokens = std::get<std::vector<Token>>(token.value); // Get the nested tokens
                auto expanded_nested_tokens = expand(nested_tokens); // Recursively expand them

                // Create a new token for the container with the expanded nested tokens
                expanded_tokens.push_back(Token(token.snippet, expanded_nested_tokens, token.token_type));
            }
            else {
                // For non-identifier tokens, add them directly to the output
                expanded_tokens.push_back(token);
            }
        }

        return expanded_tokens;
    }


private:
    std::unordered_map<std::string, std::string> convars; // Map of convars
};