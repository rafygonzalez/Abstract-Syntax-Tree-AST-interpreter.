#include <iostream>
#include "lib/lexer.hpp"
#include <iostream>

int main() {
    std::string input = "let x = 5;\n"
                        "let y = x * 2;\n"
                        "function add(a, b) {\n"
                        "  return a + b;\n"
                        "}\n"
                        "let z = add(x, y);";

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
    
    Parser parser(tokens);
    AstNodePtr ast = parser.parse();


    return 0;
}
