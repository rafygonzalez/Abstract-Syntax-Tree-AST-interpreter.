#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cctype>
#include <cstdlib>
#include <unordered_map>
using namespace std;
// Token types
enum class TokenType
{
    IDENTIFIER,
    KEYWORD,
    LITERAL,
    OPERATOR,
    PUNCTUATION
};

// Token structure
struct Token
{
    TokenType type;
    std::string value;
};

// Map of JavaScript keywords
unordered_map<std::string, TokenType> keywords = {
    {"if", TokenType::KEYWORD},
    {"else", TokenType::KEYWORD},
    {"while", TokenType::KEYWORD},
    {"for", TokenType::KEYWORD},
    {"function", TokenType::KEYWORD},
    // Add more keywords as needed
};

// Map of JavaScript operators
unordered_map<char, TokenType> operators = {
    {'+', TokenType::OPERATOR},
    {'-', TokenType::OPERATOR},
    {'*', TokenType::OPERATOR},
    {'/', TokenType::OPERATOR},
    {'%', TokenType::OPERATOR},
    {'=', TokenType::OPERATOR},
    {'<', TokenType::OPERATOR},
    {'>', TokenType::OPERATOR},
    {'!', TokenType::OPERATOR},
    // Add more operators as needed
};

// Map of JavaScript punctuation symbols
unordered_map<char, TokenType> punctuation = {
    {',', TokenType::PUNCTUATION},
    {';', TokenType::PUNCTUATION},
    {'(', TokenType::PUNCTUATION},
    {')', TokenType::PUNCTUATION},
    {'{', TokenType::PUNCTUATION},
    {'}', TokenType::PUNCTUATION},
    // Add more punctuation symbols as needed
};

// Check if a character is a digit
bool is_digit(char c)
{
    return c >= '0' && c <= '9';
}

// Check if a character is a letter or underscore
bool is_alpha(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

// Lexer function// Lexer function
std::vector<Token> lex(std::string input)
{
    std::vector<Token> tokens;

    for (size_t i = 0; i < input.size(); i++)
    {
        char c = input[i];

        if (c == ' ' || c == '\t' || c == '\n')
        {
            // Ignore whitespace
            continue;
        }
        else if (is_digit(c))
        {
            // Parse numeric literal
            std::string value;
            while (i < input.size() && is_digit(input[i]))
            {
                value += input[i++];
            }
            i--;
            tokens.push_back({TokenType::LITERAL, value});
        }
        else if (is_alpha(c) || c == '_')
        {
            // Parse identifier, keyword, or string literal
            std::string value;
            bool is_string_literal = false;
            if (c == '"')
            {
                // Parse string literal
                is_string_literal = true;
                value += c;
                while (i + 1 < input.size() && input[i + 1] != '"')
                {
                    value += input[++i];
                }
                if (i + 1 == input.size())
                {
                    // Error: unclosed string literal
                    std::cerr << "Error: unclosed string literal" << std::endl;
                    exit(1);
                }
                value += input[++i];
            }
            else
            {
                // Parse identifier or keyword
                while (i < input.size() && (is_alpha(input[i]) || is_digit(input[i]) || input[i] == '_'))
                {
                    value += input[i++];
                }
                i--;
                if (keywords.count(value))
                {
                    tokens.push_back({keywords[value], value});
                }
                else
                {
                    tokens.push_back({TokenType::IDENTIFIER, value});
                }
            }
            if (is_string_literal)
            {
                tokens.push_back({TokenType::LITERAL, value});
            }
        }
        else if (operators.count(c))
        {
            // Parse operator
            std::string value;
            value += c;
            if (i + 1 < input.size() && operators.count(input[i + 1]))
            {
                value += input[++i];
            }
            tokens.push_back({operators[c], value});
        }
        else if (punctuation.count(c))
        {
            // Parse punctuation symbol
            tokens.push_back({punctuation[c], std::string(1, c)});
        }
        else if (c == '"')
        {
            // Parse string literal
            std::string value;
            i++;
            while (i < input.size() && input[i] != '"')
            {
                value += input[i++];
            }
            if (i < input.size())
            {
                tokens.push_back({TokenType::LITERAL, value});
                i++;
            }
        }
        else
        {
            // Error: unknown character
            std::cerr << "Error: unknown character " << c << std::endl;
            exit(1);
        }
    }

    return tokens;
}

int main()
{
    std::string input = "let test = \"Hi\"; \nif (x < 5)\n {\n return x * 2;\n }";
    std::vector<Token> tokens = lex(input);

    for (const Token &token : tokens)
    {
        std::cout << "Token { type = ";
        switch (token.type)
        {
        case TokenType::IDENTIFIER:
            std::cout << "IDENTIFIER";
            break;
        case TokenType::KEYWORD:
            std::cout << "KEYWORD";
            break;
        case TokenType::LITERAL:
            std::cout << "LITERAL";
            break;
        case TokenType::OPERATOR:
            std::cout << "OPERATOR";
            break;
        case TokenType::PUNCTUATION:
            std::cout << "PUNCTUATION";
            break;
        }
        std::cout << ", value = \"" << token.value << "\" }" << std::endl;
    }

    return 0;
}
