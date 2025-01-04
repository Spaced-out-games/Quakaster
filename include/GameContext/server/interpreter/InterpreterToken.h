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
#include <include/thirdparty/glm/glm.hpp>
#include <include/GameContext/utils/utility_types.h>


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
    TOKEN_TILDE,
    TOKEN_EQUALS,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_BRACKET_CONTAINER, // For []
    TOKEN_PARENTHESES_CONTAINER, // For ()
    TOKEN_ASSIGN,
    TOKEN_NEWLINE,
    TOKEN_EOF,
    TOKEN_CURLY_CONTAINER,
    TOKEN_ACCENT_CHAR = TOKEN_TILDE
};

class Token;

using console_fn = func_ptr_t<void, std::vector<Token>>;

// Define a variant type to hold different token values
using token_value_t = std::variant<
    int,
    float,
    std::string,
    glm::vec3,
    std::vector<Token>,
    console_fn
>;







// Interface for tokens
struct Token {

public:
    Token() = default;

    bool needs_read = 0;
    token_type_t token_type; // Specifies the type of token
    std::string snippet;      // Snippet of the input string corresponding to the token
    token_value_t value;

    bool is_ternary_operator()
    {
        return token_type == TOKEN_PLUS || token_type == TOKEN_MINUS || token_type == TOKEN_DIVIDE || token_type == TOKEN_MULTIPLY;
    }
    
    bool is_container()
    {
        return token_type == TOKEN_BRACKET_CONTAINER || token_type == TOKEN_PARENTHESES_CONTAINER || token_type == TOKEN_CURLY_CONTAINER;
    }

    bool is_container_complex()
    {
        return token_type == TOKEN_BRACKET_CONTAINER || token_type == TOKEN_PARENTHESES_CONTAINER || token_type == TOKEN_ASSIGN;
    }

    bool is_identifier() { return token_type == TOKEN_IDENTIFIER; }

    bool is_EOL() { return token_type == TOKEN_NEWLINE || token_type == TOKEN_SEMICOLON; }


    bool is_literal()
    {
        return token_type == TOKEN_FLOAT || token_type == TOKEN_INTEGER || token_type == TOKEN_STRING;
    }

    // Add comparison operators if necessary
    bool operator==(const Token& other) const {
        return token_type == other.token_type && value == other.value;
    }

    bool operator!=(const Token& other) const {
        return !(*this == other);
    }

    std::string to_string_debug(int depth = 0) {
        std::stringstream ss;

        // Indentation for nested tokens
        for (int i = 0; i < depth; ++i) {
            ss << "    ";
        }
        
        // Print the token snippet and type
        ss << "Token( read: " << needs_read <<" snippet: \"" << snippet << "\", type: ";
        switch (token_type) {
        case TOKEN_INTEGER: ss << "INTEGER"; break;
        case TOKEN_FLOAT: ss << "FLOAT"; break;
        case TOKEN_STRING: ss << "STRING"; break;
        case TOKEN_IDENTIFIER: ss << "IDENTIFIER"; break;
        case TOKEN_EXPRESSION: ss << "EXPRESSION"; break;
        case TOKEN_BRACKET_CONTAINER: ss << "BRACKET_CONTAINER"; break;
        case TOKEN_PARENTHESES_CONTAINER: ss << "PARENTHESES_CONTAINER"; break;
        case TOKEN_CURLY_CONTAINER: ss << "CURLY_CONTAINER"; break;
        case TOKEN_EQUALS: ss << "EQUALS"; break;
        case TOKEN_ASSIGN: ss << "ASSIGN"; break;

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
                ss << nested_token.to_string_debug(depth + 1) << "\n";
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
        : snippet(snippet), token_type(type), value(0)
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
    Token parse_string(std::string& content, size_t& offset, char closer = '"');
    Token tokenize_once(std::string& content, size_t& offset);
    void merge(std::vector<Token>& tokens);


    static std::vector<Token> tokenize(std::string& content) {
        size_t current_position = 0; // Use size_t for consistency
        std::vector<Token> tokens;

        while (current_position < content.size()) {
            //Token token = tokenize_once(content, current_position);
            tokens.emplace_back(tokenize_once(content, current_position));

            // Break if we reached the end of the content
            if (tokens.back().token_type == TOKEN_EOF) {
                break;
            }
        }

        merge(tokens); // Merge tokens if necessary
        return tokens;
    }

    static Token tokenize_once(std::string& content, size_t& offset) {
        skipWhitespace(content, offset);

        while (offset < content.size()) {
            // Handle single-character tokens
            switch (content[offset]) {
            case ';':
                offset++; // Move to the next character
                return Token{ ";", TOKEN_SEMICOLON }; // Return the token
            case ',':
                offset++;
                return Token{ ",", TOKEN_COMMA };
            case ':':
                offset++;
                return Token{ ":", TOKEN_COLON };
            case '=':
                offset++;
                return Token{ "=", TOKEN_EQUALS };
            case '+':
                offset++;
                return Token{ "+", TOKEN_PLUS };
            case '-':
                offset++;
                return Token{ "-", TOKEN_MINUS };
            case '*':
                offset++;
                return Token{ "*", TOKEN_MULTIPLY };
            case '/':
                offset++;
                return Token{ "/", TOKEN_DIVIDE };
            case '"':
                return parse_string(content, offset); // Return the parsed string token
            case '\'':
                return parse_string(content, offset, '\''); // Return the parsed string token
            case '\n':
                offset++;
                return Token{ "NEWLINE", TOKEN_NEWLINE }; // Return newline token
            case '~':
                offset++;
                return Token{ "~", TOKEN_TILDE };
                break;
            default:
                break; // Skip to the next character if not a token
            }

            // Check for digits to parse a number
            if (isdigit(content[offset]) || (content[offset] == '.')) {
                return parse_number(content, offset); // Return the parsed number token
            }
            // Check for identifiers
            else if (isalpha(content[offset]) || content[offset] == '_') {
                return parse_identifier(content, offset); // Return the parsed identifier token
            }
            // Check for containers
            else if (content[offset] == '(' || content[offset] == '[' || content[offset] == '{') {
                return parse_container(content, offset); // Return the parsed container token
            }
            else {
                // Handle unexpected characters (optional)
                std::cerr << "Unexpected character: " << content[offset] << std::endl;
                offset++;
            }

            skipWhitespace(content, offset); // Continue skipping whitespace
        }

        // Return a default token if no valid token was found
        return Token{ "EOF", TOKEN_EOF }; // Assuming you have an EOF token defined
    }

    static void merge(std::vector<Token>& tokens)
    {
        for (size_t i = tokens.size() - 1; i > 0; i--)
        {
            // Check for a valid assignment pattern (identifier = value)
            if (i >= 2) {
                bool token_1_match = (tokens[i - 2].is_identifier());
                bool token_2_match = (tokens[i - 1].token_type == TOKEN_EQUALS);
                bool token_3_match = (tokens[i].is_literal() || tokens[i].is_container() || tokens[i].token_type == TOKEN_IDENTIFIER);

                if (token_1_match && token_2_match && token_3_match)
                {
                    tokens[i - 2].token_type = TOKEN_ASSIGN; // Update the token type
                    std::vector<Token> assign_args;
                    assign_args.push_back(tokens[i]); // Add the right-hand side value
                    tokens[i - 2].value = assign_args; // Set the value of the assignment
                    tokens.erase(tokens.begin() + i); // Remove the value token from the vector
                    tokens.erase(tokens.begin() + i - 1); // Remove the equals token from the vector
                }
            }
            if(i>=1)
            {
                // Accented tokens assimulate
                bool token_1_match = (tokens[i - 1].token_type == TOKEN_ACCENT_CHAR);
                bool token_2_match = (tokens[i - 0].is_identifier());
                if (token_1_match && token_2_match)
                {
                    std::vector<Token> accent_target = { tokens[i] };
                    accent_target[0].needs_read = true;
                    tokens[i - 1].value = accent_target;
                    tokens.erase(tokens.begin() + i);
                }
                
                // token lookahead
                if (i < tokens.size() - 1)
                {
                    if (tokens[i - 1].is_ternary_operator() || tokens[i + 1].is_ternary_operator())
                    {
                        tokens[i].needs_read = 1;
                    }
                }
                

            }
        }

    }


    // This replaces the standard isspace function since it considers newline as a space, which fucks up everything
    static bool is_space(char ch)
    {
        bool result = 0;
        switch (ch)
        {
        case ' ':
            result = 1;
            break;
        case '\t':
            result = 1;
            break;
        default:
            break;
        }
        return result;
    }

    static void skipWhitespace(std::string& content, size_t& offset) {


        while (offset < content.size() && is_space(content[offset])) {
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

        if (token_type == TOKEN_FLOAT)
        {
            Token t = Token(number_snippet ,token_type);
            t.value = std::stof(number_snippet);
            return t;
        }
        else if (token_type == TOKEN_INTEGER)
        {
            Token t = Token(number_snippet, token_type);
            t.value = std::stoi(number_snippet);
            return t;
        }

        // Create and return the Token
        return Token(number_snippet, token_type);
    }

    static Token parse_string(std::string& content, size_t& offset, char closer)
    {
        std::string string_snippet;
        offset++;
        while (content[offset] != closer )
        {
            string_snippet += content[offset];
            offset++;
        }
        Token t = { string_snippet, TOKEN_STRING };
        t.value = string_snippet;
        offset++;
        return t;
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
        Token t = { identifier_snippet, TOKEN_IDENTIFIER };
        t.value = identifier_snippet;
        return t;
    }


    // here's where the 'unexpected token' comes up
    static Token parse_container(std::string& content, size_t& offset) {
        char container_char = content[offset];
        std::string container_snippet(1, container_char);
        offset++; // Move past the opening container character

        // Determine the type of container based on the opening character
        token_type_t container_type = TOKEN_UNKNOWN;
        char closer = 0;
        switch (container_char) {
        case '(':
            container_type = TOKEN_PARENTHESES_CONTAINER;
            closer = ')';
            break;
        case '[':
            container_type = TOKEN_BRACKET_CONTAINER;
            closer = ']';
            break;
        case '{':
            container_type = TOKEN_CURLY_CONTAINER;
            closer = '}';
            break;
        default:
            std::cerr << "Unexpected container character: " << container_char << std::endl;
            return Token(container_snippet, TOKEN_UNKNOWN); // Return a default token for unexpected container
        }

        // Use a vector to manage nested tokens
        std::vector<Token> nested_tokens;

        // Tokenize until the matching closing container
        while (offset < content.size() && content[offset] != closer) {
            skipWhitespace(content, offset);

            if (offset < content.size()) {
                Token token = tokenize_once(content, offset);
                if (token.token_type == TOKEN_EOF) {
                    std::cerr << "Unexpected EOF while parsing container" << std::endl;
                    break;
                }
                nested_tokens.push_back(token);
            }
        }

        // Move past the closing container character
        if (offset < content.size() && content[offset] == closer) {
            container_snippet += content[offset];
            offset++;
        }
        else {
            std::cerr << "Missing closing container for: " << container_char << std::endl;
        }

        // Create a Token representing the container with nested tokens
        return Token(container_snippet, nested_tokens, container_type);
    }


}
