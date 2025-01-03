#pragma once
#include <variant>
#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <cctype>
#include <optional>

enum token_type_t : uint8_t
{
    TOKEN_UNKNOWN,
    TOKEN_INTEGER,
    TOKEN_FLOAT,
    TOKEN_STRING,
    TOKEN_IDENTIFIER,
    TOKEN_EXPRESSION,
    TOKEN_SEMICOLON,
    TOKEN_COMMA,
    TOKEN_COLON,
    TOKEN_ASSIGN, // for a = b
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_BRACKET_CONTAINER, // For []
    TOKEN_PARENTHESES_CONTAINER, // For ()
    TOKEN_EQUALS
};



// Define a variant type to hold different token values
using token_value_t = std::variant<int, float, std::string, std::vector<class Token>>;







// Interface for tokens
struct Token {
    token_type_t token_type; // Specifies the type of token
    std::string snippet;      // Snippet of the input string corresponding to the token
    token_value_t value;

    // Add comparison operators if necessary
    bool operator==(const Token& other) const {
        return token_type == other.token_type && value == other.value;
    }

    bool operator!=(const Token& other) const {
        return !(*this == other);
    }

    std::string to_string(int depth = 0) {
        std::stringstream ss;

        // Indentation for nested tokens
        for (int i = 0; i < depth; ++i) {
            ss << "    ";
        }

        // Print the token snippet and type
        ss << "Token(snippet: \"" << snippet << "\", type: ";
        switch (token_type) {
        case TOKEN_INTEGER: ss << "INTEGER"; break;
        case TOKEN_FLOAT: ss << "FLOAT"; break;
        case TOKEN_STRING: ss << "STRING"; break;
        case TOKEN_IDENTIFIER: ss << "IDENTIFIER"; break;
        case TOKEN_EXPRESSION: ss << "EXPRESSION"; break;
        case TOKEN_BRACKET_CONTAINER: ss << "BRACKET_CONTAINER"; break;
        case TOKEN_PARENTHESES_CONTAINER: ss << "BRACKET_CONTAINER"; break;

        default: ss << "UNKNOWN";
        }
        ss << ", value: ";

        // Print the token value
        if (std::holds_alternative<int>(value)) {
            ss << std::get<int>(value);
        }
        else if (std::holds_alternative<float>(value)) {
            ss << std::get<float>(value);
        }
        else if (std::holds_alternative<std::string>(value)) {
            ss << "\"" << std::get<std::string>(value) << "\"";
        }
        else if (std::holds_alternative<std::vector<Token>>(value)) {
            ss << "[\n"; // Begin nested tokens
            for (auto& nested_token : std::get<std::vector<Token>>(value)) {
                ss << nested_token.to_string(depth + 1) << "\n";
            }
            for (int i = 0; i < depth; ++i) {
                ss << "    "; // Indentation for closing bracket
            }
            ss << "]";
        }
        else {
            ss << "null"; // Handle empty or uninitialized variant
        }

        ss << ")";
        return ss.str();
    }


    


    // Constructor for initializing the snippet and token type
    Token(const std::string& snippet, token_type_t type = TOKEN_UNKNOWN)
        : snippet(snippet), token_type(type), value(from_string())
    {

    }

    Token(const std::string& snippet, std::vector<Token> tokens, token_type_t type = TOKEN_UNKNOWN): snippet(snippet), token_type(type), value(tokens)
    {

    }

    // Method to get the token value
    token_value_t from_string() {
        // Clear the stringstream for reuse
        static std::stringstream ss; // Stream for parsing
        ss.str(""); // Clear the previous content
        ss.clear(); // Clear any error flags

        ss << snippet; // Pipe the snippet into the stringstream

        // Try to read it as a float first
        float floatValue;
        if (ss >> floatValue) {
            // If it's a whole number, return as an int
            if (floatValue == static_cast<int>(floatValue)) {
                token_type = TOKEN_INTEGER; // Set the token type
                return static_cast<int>(floatValue); // Return as an int
            }
            token_type = TOKEN_FLOAT; // Set the token type
            return floatValue; // Return as a float
        }

        // Check if the snippet is a valid identifier (alphanumeric or underscore)
        if (isIdentifier(snippet)) {
            token_type = TOKEN_IDENTIFIER; // Set the token type
            return snippet; // Return as a string
        }

        // If no valid number or identifier, return a default constructed variant (empty)
        token_type = TOKEN_UNKNOWN; // Set the token type to unknown
        return token_value_t(); // Return an empty variant
    }

private:
    // Helper function to check if the snippet is a valid identifier
    bool isIdentifier(const std::string& str) const {
        if (str.empty() || !(isalpha(str[0]) || str[0] == '_')) {
            return false; // First character must be a letter or underscore
        }
        for (char c : str) {
            if (!isalnum(c) && c != '_') {
                return false; // Must be alphanumeric or underscore
            }
        }
        return true; // Valid identifier
    }
};










namespace Tokenizer
{

    void skipWhitespace(std::string& content, size_t& offset);
    Token parse_number(std::string& content, size_t& offset);
    Token parse_identifier(std::string& content, size_t& offset);
    Token parse_container(std::string& content, size_t& offset);

    static std::vector<Token> tokenize(std::string& content, size_t& offset) {
        std::vector<Token> tokens;
        bool last_token_was_identifier = 0;

        skipWhitespace(content, offset);

        while (offset < content.size()) {
            // Handle single-character tokens
            switch (content[offset]) {
            case ';':
                last_token_was_identifier = false;
                tokens.push_back(Token{ ";", TOKEN_SEMICOLON });
                break;
            case ',':
                last_token_was_identifier = false;
                tokens.push_back(Token{ ",", TOKEN_COMMA });
                break;
            case ':':
                last_token_was_identifier = false;
                tokens.push_back(Token{ ":", TOKEN_COLON });
                break;
            case '=':
                if (last_token_was_identifier)
                {
                    tokens.back().token_type = TOKEN_ASSIGN;
                    break;
                }



                tokens.push_back(Token{ "=", TOKEN_EQUALS });
                last_token_was_identifier = false;
                break;
            case '+':
                last_token_was_identifier = false;
                tokens.push_back(Token{ "+", TOKEN_PLUS });
                break;
            case '-':
                last_token_was_identifier = false;
                tokens.push_back(Token{ "-", TOKEN_MINUS });
                break;
            case '*':
                last_token_was_identifier = false;
                tokens.push_back(Token{ "*", TOKEN_MULTIPLY });
                break;
            case '/':
                last_token_was_identifier = false;
                tokens.push_back(Token{ "/", TOKEN_DIVIDE });
                break;
            default:
                break; // Skip to the next character if not a token
            }

            // Skip to the next character if a token was added
            if (!tokens.empty() && tokens.back().snippet == std::string(1, content[offset])) {
                offset++;
                skipWhitespace(content, offset);
                continue;
            }

            // Check for digits to parse a number
            if (isdigit(content[offset]) || (content[offset] == '.')) {
                last_token_was_identifier = false;
                tokens.push_back(parse_number(content, offset)); // Parse and add the number token
            }
            // Check for identifiers
            else if (isalpha(content[offset]) || content[offset] == '_') {
                if (last_token_was_identifier && tokens.back().token_type == TOKEN_ASSIGN)
                {
                    std::vector<Token> assignment_tokens;
                    assignment_tokens.push_back(Token{ tokens.back().snippet, TOKEN_IDENTIFIER });
                    assignment_tokens.push_back(parse_identifier(content, offset));

                    auto& last_token = tokens.back();
                    last_token.token_type = TOKEN_ASSIGN;
                    last_token.value = assignment_tokens;
                    break;
                }

                
                tokens.push_back(parse_identifier(content, offset)); // Parse and add the identifier token

                last_token_was_identifier = true;
            }
            // Check for containers
            else if (content[offset] == '(' || content[offset] == '[') {
                last_token_was_identifier = false;
                tokens.push_back(parse_container(content, offset)); // Parse and add the container token
            }
            else {
                // Handle unexpected characters (optional)
                last_token_was_identifier = false;
                std::cerr << "Unexpected character: " << content[offset] << std::endl;
                offset++;
            }

            skipWhitespace(content, offset); // Continue skipping whitespace
        }

        return tokens;
    }

    static std::vector<Token> tokenize(std::string& content)
    {
        size_t current_position = 0; // Use size_t for consistency
        return tokenize(content, current_position);
    }


    static void skipWhitespace(std::string& content, size_t& offset) {
        while (offset < content.size() && isspace(content[offset])) {
            offset++; // Use offset consistently
        }
    }

    static Token parse_number(std::string& content, size_t& offset) {
        std::string number_snippet;
        bool encountered_decimal = false;

        // Check if the current character is a digit or a decimal point
        while (offset < content.size() && (isdigit(content[offset]) || (content[offset] == '.' && !encountered_decimal))) {
            // If we encounter a decimal point, set the flag and continue
            if (content[offset] == '.') {
                encountered_decimal = true;
            }
            number_snippet += content[offset]; // Append the character to the number snippet
            offset++; // Move to the next character
        }

        // Determine the token type based on whether a decimal point was encountered
        token_type_t token_type = encountered_decimal ? TOKEN_FLOAT : TOKEN_INTEGER;

        // Create and return the Token
        return Token(number_snippet, token_type);
    }

    static Token parse_identifier(std::string& content, size_t& offset) {
        std::string identifier_snippet;

        // Identifiers can start with a letter or underscore and can be followed by letters, digits, or underscores
        if (isalpha(content[offset]) || content[offset] == '_') {
            identifier_snippet += content[offset];
            offset++;

            while (offset < content.size() && (isalnum(content[offset]) || content[offset] == '_')) {
                identifier_snippet += content[offset];
                offset++;
            }
        }

        // Return the identifier token
        return Token(identifier_snippet, TOKEN_IDENTIFIER);
    }

    static Token parse_container(std::string& content, size_t& offset) {
        char container_char = content[offset];
        std::string container_snippet(1, container_char);
        offset++; // Move past the opening container character

        // Use a vector to manage nested tokens
        std::vector<Token> nested_tokens;

        // Determine the type of container based on the opening character
        token_type_t container_type = (container_char == '[') ? TOKEN_BRACKET_CONTAINER : TOKEN_PARENTHESES_CONTAINER;

        // Recursively tokenize until we find the matching closing container
        while (offset < content.size() && content[offset] != (container_char == '(' ? ')' : ']')) {
            skipWhitespace(content, offset);

            if (offset < content.size()) {
                // Check for nested containers
                if (content[offset] == '(' || content[offset] == '[') {
                    nested_tokens.push_back(parse_container(content, offset));
                }
                // Check for identifiers
                else if (isalpha(content[offset]) || content[offset] == '_') {
                    nested_tokens.push_back(parse_identifier(content, offset));
                }
                // Check for numbers
                else if (isdigit(content[offset]) || content[offset] == '.') {
                    nested_tokens.push_back(parse_number(content, offset));
                }
                else {
                    // Handle unexpected characters (optional)
                    std::cerr << "Unexpected character: " << content[offset] << std::endl;
                    offset++;
                }
            }
        }

        // Move past the closing container character
        if (offset < content.size()) {
            container_snippet += content[offset];
            offset++;
        }

        // Create a Token representing the container with nested tokens
        return Token(container_snippet, nested_tokens, container_type);
    }


}
