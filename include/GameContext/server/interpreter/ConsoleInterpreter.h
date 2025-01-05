#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <queue>
#include <functional>
#include <iostream>
#include "InterpreterToken.h" // Ensure this path is correct
#include "Convar.h"
#include <span>
#include <include/GameContext/GameContext.h>

//struct Convar;

//#include <include/thirdparty/glm/glm.hpp> 

struct ConsoleUI;



class ConsoleInterpreter {
public:
    // Constructor that initializes the convar map
    ConsoleInterpreter() {

    }

    std::string exit(std::span<Token> tokens)
    {
        set_convar("exit_status", 1);
    }
   
    bool is_command(const Token& token) { return convars[token.snippet].deduce_token_type<console_fn>() == CONSOLE_FUNC; }

    Token expand(Token& token) {

        std::cout << token.to_string_debug() << '\n';

        if (token.needs_read) {
            auto it = convars.find(token.snippet); // Use the snippet to look up the value
            if (it != convars.end()) {
                // If found, create a new token with the corresponding value
                return Token{ it->second.get<std::string>(), TOKEN_IDENTIFIER };
            }
        }
        if (token.is_container()) {
            std::vector<Token>& container = std::get<std::vector<Token>>(token.value);
            for (size_t i = 0; i < container.size(); ++i) {
                container[i] = expand(container[i]); // Expand each token inside the container
            }
            return Token{ token.snippet, container, token.token_type }; // Return updated container
        }
        return token;
    }

    std::string execute(std::string& input)
    {
        // Tokenize the input
        std::vector<Token> tokens = Tokenizer::tokenize(input);

        // Expand tokens and find command ranges
        std::vector<std::span<Token>> arguments;
        std::vector<std::string> command_names;
        size_t start = 0;
        for (size_t i = 0; i < tokens.size(); ++i)
        {
            tokens[i] = expand(tokens[i]); // Expand the token
        }

        for (size_t i = 0; i < tokens.size(); ++i)
        {
            

            if (tokens[i].is_EOL())
            {
                // Only proceed if there's a command to process
                if (start < i) // Check that there are tokens in the range
                {
                    // Store command name (the first token in the current command range)
                    if (tokens[start].token_type == TOKEN_IDENTIFIER) // Check if it's a command token
                    {
                        command_names.push_back(tokens[start].snippet); // Store the command name
                    }
                    else
                    {
                        command_names.push_back(""); // Fallback if not a valid command token
                    }

                    // Add the range [start + 1, i) to arguments
                    arguments.emplace_back(std::span<Token>(tokens.data() + start + 1, i - start - 1));
                }

                start = i + 1; // Move start to the next token after the delimiter
            }
        }

        // Handle the final range if there's any leftover tokens
        if (start < tokens.size())
        {
            if (tokens[start].token_type == TOKEN_IDENTIFIER)
            {
                command_names.push_back(tokens[start].snippet);
            }
            else
            {
                command_names.push_back("");
            }

            // Add the arguments span, excluding the command name
            if (tokens.size() > start + 1)
            {
                arguments.emplace_back(std::span<Token>(tokens.data() + start + 1, tokens.size() - start - 1));
            }
            else
            {
                arguments.emplace_back(std::span<Token>()); // Empty span if no arguments
            }
        }

        // Execute each command with corresponding arguments
        for (size_t i = 0; i < command_names.size(); ++i)
        {
            if (i < arguments.size()) // Ensure index is valid for arguments
            {
                return execute_command(command_names[i], arguments[i]);
            }
            else
            {
                return "Warning: Mismatched command and argument sizes.\n";
            }
        }
    }


    std::string execute_command(std::string& command, std::span<Token> args)
    {
        std::string result;

        // runs a function, piping in the rest of the tokens
        auto it = convars.find(command);
        if (it != convars.end() && it->second.type == CONSOLE_FUNC)
        {
            console_fn& func = it->second.get<console_fn>();
            func(args);
            return "";
        }

        // sets a convar
        if (it != convars.end())
        {
            size_t offset = 0;
            result += "command does not exist; attempting to assign convar";

            // Check for TOKEN_ASSIGN
            if (args.size() >= 2 && ( args[0].token_type == TOKEN_ASSIGN || args[0].token_type == TOKEN_EQUALS))
            {
                offset = 1; // Move to the next token
            }

            // Ensure offset is within bounds
            if (offset < args.size())
            {
                Convar& variable = it->second;
                //Convar& value = args[offset].value; // Safe to access now

                switch (variable.type)
                {
                case TOKEN_FLOAT:
                    if (args[offset].token_type == TOKEN_FLOAT) { variable.set<float>(std::get<float>(args[offset].value)); }
                    else { return "Type mismatch"; }
                    break;
                case TOKEN_INTEGER:
                    if (args[offset].token_type == TOKEN_INTEGER) { variable.set<int>(std::get<int>(args[offset].value)); }
                    else { return "Type mismatch"; }
                    break;
                case TOKEN_STRING:
                    if (args[offset].token_type == TOKEN_STRING) { variable.set<std::string>(std::get<std::string>(args[offset].value)); }
                    else { return "Type mismatch"; }
                    break;


                default:
                    break;
                }

                return result;
            }
            else
            {
                result += "Error: Offset is out of range.\n";
                return result; // Handle the error appropriately
            }
        }

        return "SYNTAX ERROR";
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
    // Add a new convar to the map
    template <typename T>
    void add_convar(const std::string& name, T& target) {
        assert(convars.find(name) == convars.end() && "Convar already exists!");
        convars[name].target = (void*)(&target);
        convars[name].type = convars[name].deduce_token_type<T>();
        //convars[name].set(value);
    }

    void add_command(const std::string& name, void* target)
    {
        convars[name].target = target;
        convars[name].type = CONSOLE_FUNC;
    }

    // Get a convar's value
    template <typename T>
    T get_convar(const std::string& name) const {
        auto it = convars.find(name);
        assert(it != convars.end() && "Convar not found!");
        return it->second.get<T>();
    }

    // Set a convar's value
    template <typename T>
    void set_convar(const std::string& name, const T& value) {
        auto it = convars.find(name);
        assert(it != convars.end() && "Convar not found!");
        it->second.set(value);
    }

    // Check if a convar exists
    bool has_convar(const std::string& name) const {
        return convars.find(name) != convars.end();
    }
//private:
    //std::unordered_map<std::string, token_value_t> convars; // Map of convars
    std::unordered_map<std::string, Convar> convars; // Map of convars
};