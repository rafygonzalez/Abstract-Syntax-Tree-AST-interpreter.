#include <iostream>
#include "lib/lexer.hpp"

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
