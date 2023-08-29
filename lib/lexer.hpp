#include <string>
#include <vector>
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
    {"return", TokenType::KEYWORD},
    {"let", TokenType::KEYWORD},
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
    {'<=', TokenType::OPERATOR},
    {'>=', TokenType::OPERATOR},
    {'==', TokenType::OPERATOR},
    {'!=', TokenType::OPERATOR},
    {'&&', TokenType::OPERATOR},
    {'||', TokenType::OPERATOR},
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
            value += c;
            while (i + 1 < input.size() && input[i + 1] != '"')
            {
                // Handle escape sequences
                if (input[i + 1] == '\\')
                {
                    value += input[i + 1];
                    if (i + 2 < input.size())
                    {
                        value += input[i + 2];
                        i += 2;
                    }
                    else
                    {
                        // Error: invalid escape sequence
                        std::cerr << "Error: invalid escape sequence" << std::endl;
                        exit(1);
                    }
                }
                else
                {
                    value += input[++i];
                }
            }
            if (i + 1 == input.size())
            {
                // Error: unclosed string literal
                std::cerr << "Error: unclosed string literal" << std::endl;
                exit(1);
            }
            value += input[++i];
            tokens.push_back({TokenType::LITERAL, value});
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

class AstNode
{
public:
    virtual ~AstNode() {}
};

using AstNodePtr = std::shared_ptr<AstNode>;

// Expressions
class Expression : public AstNode
{
};

class Identifier : public Expression
{
public:
    std::string name;
    Identifier(const std::string &name) : name(name) {}
};

class Literal : public Expression
{
public:
    std::string value;
    Literal(const std::string &value) : value(value) {}
};

class BinaryOperation : public Expression
{
public:
    Token op;
    AstNodePtr left;
    AstNodePtr right;
    BinaryOperation(const Token &op, AstNodePtr left, AstNodePtr right) : op(op), left(left), right(right) {}
};

// Statements
class Statement : public AstNode
{
};

class ExpressionStatement : public Statement
{
public:
    AstNodePtr expression;
    ExpressionStatement(AstNodePtr expression) : expression(expression) {}
};

class BlockStatement : public Statement
{
public:
    std::vector<AstNodePtr> statements;
};

class IfStatement : public Statement
{
public:
    AstNodePtr condition;
    AstNodePtr thenStatement;
    AstNodePtr elseStatement;
    IfStatement(AstNodePtr condition, AstNodePtr thenStatement, AstNodePtr elseStatement)
        : condition(condition), thenStatement(thenStatement), elseStatement(elseStatement) {}
};

class WhileStatement : public Statement
{
public:
    AstNodePtr condition;
    AstNodePtr body;
    WhileStatement(AstNodePtr condition, AstNodePtr body) : condition(condition), body(body) {}
};

class ForStatement : public Statement
{
public:
    AstNodePtr init;
    AstNodePtr condition;
    AstNodePtr update;
    AstNodePtr body;
    ForStatement(AstNodePtr init, AstNodePtr condition, AstNodePtr update, AstNodePtr body)
        : init(init), condition(condition), update(update), body(body) {}
};

class FunctionDeclaration : public Statement
{
public:
    std::string name;
    std::vector<std::string> params;
    AstNodePtr body;
    FunctionDeclaration(const std::string &name, const std::vector<std::string> &params, AstNodePtr body)
        : name(name), params(params), body(body) {}
};

class ReturnStatement : public Statement
{
public:
    AstNodePtr expression;
    ReturnStatement(AstNodePtr expression) : expression(expression) {}
};

class VariableDeclaration : public Statement
{
public:
    std::string name;
    AstNodePtr init;
    VariableDeclaration(const std::string &name, AstNodePtr init) : name(name), init(init) {}
};

class Parser
{
public:
    Parser(const std::vector<Token> &tokens) : tokens(tokens), position(0) {}

    AstNodePtr parse()
    {
        std::vector<AstNodePtr> statements;
        while (!eof())
        {
            statements.push_back(parseStatement());
        }
        auto block = std::make_shared<BlockStatement>();
        block->statements = statements;
        return block;
    }

private:
    std::vector<Token> tokens;
    size_t position;

    bool eof() const
    {
        return position >= tokens.size();
    }

    const Token &peek() const
    {
        return tokens[position];
    }

    const Token &next()
    {
        return tokens[++position];
    }

    void advance()
    {
        position++;
    }

    bool match(TokenType type)
    {
        if (peek().type == type)
        {
            advance();
            return true;
        }
        return false;
    }

    AstNodePtr parseStatement()
    {
        AstNodePtr statement;
        if (peek().type == TokenType::KEYWORD && peek().value == "if")
        {
            advance();
            statement = parseIfStatement();
        }
        else if (peek().type == TokenType::KEYWORD && peek().value == "while")
        {
            advance();
            statement = parseWhileStatement();
        }
        else if (peek().type == TokenType::KEYWORD && peek().value == "for")
        {
            advance();
            statement = parseForStatement();
        }
        else if (peek().type == TokenType::KEYWORD && peek().value == "function")
        {
            advance();
            statement = parseFunctionDeclaration();
        }
        else if (peek().type == TokenType::KEYWORD && peek().value == "return")
        {
            advance();
            statement = parseReturnStatement();
        }
        else if (peek().type == TokenType::KEYWORD && peek().value == "let")
        {
            advance();
            statement = parseVariableDeclaration();
        }
        else
        {
            statement = parseExpressionStatement();
        }
        return statement;
    }

    AstNodePtr parseExpression()
    {
        AstNodePtr left = parsePrimary();

        while (peek().type == TokenType::OPERATOR)
        {
            Token op = peek();
            advance();
            AstNodePtr right = parsePrimary();
            left = std::make_shared<BinaryOperation>(op, left, right);
        }

        return left;
    }

    AstNodePtr parsePrimary()
    {
        if (peek().type == TokenType::IDENTIFIER)
        {
            return parseIdentifier();
        }
        else if (peek().type == TokenType::LITERAL)
        {
            return parseLiteral();
        }
        else if (match(TokenType::PUNCTUATION) && peek().value == "(")
        {
            AstNodePtr expr = parseExpression();
            if (!match(TokenType::PUNCTUATION) || peek().value != ")")
            {
                // Error: unmatched parenthesis
                std::cerr << "Error: unmatched parenthesis" << std::endl;
                exit(1);
            }
            return expr;
        }
        // Error: unexpected token
        std::cerr << "Error: unexpected token " << peek().value << std::endl;
        exit(1);
    }

    AstNodePtr parseIdentifier()
    {
        auto identifier = std::make_shared<Identifier>(peek().value);
        advance();
        return identifier;
    }

    AstNodePtr parseLiteral()
    {
        auto literal = std::make_shared<Literal>(peek().value);
        advance();
        return literal;
    }

    AstNodePtr parseExpressionStatement()
    {
        AstNodePtr expr = parseExpression();
        if (match(TokenType::PUNCTUATION) && peek().value == ";")
        {
            advance();
        }
        return std::make_shared<ExpressionStatement>(expr);
    }

    AstNodePtr parseIfStatement()
    {
        if (!match(TokenType::PUNCTUATION) || peek().value != "(")
        {
            // Error: expected '(' after 'if'
            std::cerr << "Error: expected '(' after 'if'" << std::endl;
            exit(1);
        }
        advance();
        AstNodePtr condition = parseExpression();
        if (!match(TokenType::PUNCTUATION) || peek().value != ")")
        {
            // Error: expected ')' after if condition
            std::cerr << "Error: expected ')' after if condition" << std::endl;
            exit(1);
        }
        advance();
        AstNodePtr thenStatement = parseStatement();
        AstNodePtr elseStatement;
        if (match(TokenType::KEYWORD) && peek().value == "else")
        {
            advance();
            elseStatement = parseStatement();
        }
        return std::make_shared<IfStatement>(condition, thenStatement, elseStatement);
    }

    AstNodePtr parseWhileStatement()
    {
        if (!match(TokenType::PUNCTUATION) || peek().value != "(")
        {
            // Error: expected '(' after 'while'
            std::cerr << "Error: expected '(' after 'while'" << std::endl;
            exit(1);
        }
        advance();
        AstNodePtr condition = parseExpression();
        if (!match(TokenType::PUNCTUATION) || peek().value != ")")
        {
            // Error: expected ')' after while condition
            std::cerr << "Error: expected ')' after while condition" << std::endl;
            exit(1);
        }
        advance();
        AstNodePtr body = parseStatement();
        return std::make_shared<WhileStatement>(condition, body);
    }

    AstNodePtr parseForStatement()
    {
        if (!match(TokenType::PUNCTUATION) || peek().value != "(")

        {
            // Error: expected '(' after 'for'
            std::cerr << "Error: expected '(' after 'for'" << std::endl;
            exit(1);
        }
        advance();
        AstNodePtr init = parseStatement();
        AstNodePtr condition = parseExpression();
        if (!match(TokenType::PUNCTUATION) || peek().value != ";")
        {
            // Error: expected ';' after for condition
            std::cerr << "Error: expected ';' after for condition" << std::endl;
            exit(1);
        }
        advance();
        AstNodePtr update = parseExpression();
        if (!match(TokenType::PUNCTUATION) || peek().value != ")")
        {
            // Error: expected ')' after for update
            std::cerr << "Error: expected ')' after for update" << std::endl;
            exit(1);
        }
        advance();
        AstNodePtr body = parseStatement();
        return std::make_shared<ForStatement>(init, condition, update, body);
    }

    AstNodePtr parseFunctionDeclaration()
    {
        AstNodePtr name = parseIdentifier();
        if (!match(TokenType::PUNCTUATION) || peek().value != "(")

        {
            // Error: expected '(' after function name
            std::cerr << "Error: expected '(' after function name" << std::endl;
            exit(1);
        }
        advance();
        std::vector<std::string> params;
        while (peek().type == TokenType::IDENTIFIER)
        {
            params.push_back(peek().value);
            advance();
            if (!match(TokenType::PUNCTUATION) || peek().value != ",")
            {
                break;
            }
            advance();
        }
        if (!match(TokenType::PUNCTUATION) || peek().value != ")")
        {
            // Error: expected ')' after function parameters
            std::cerr << "Error: expected ')' after function parameters" << std::endl;
            exit(1);
        }
        advance();
        AstNodePtr body = parseStatement();
        return std::make_shared<FunctionDeclaration>(static_cast<Identifier *>(name.get())->name, params, body);
    }

    AstNodePtr parseReturnStatement()
    {
        AstNodePtr expression = parseExpression();
        if (match(TokenType::PUNCTUATION) && peek().value == ";")
        {
            advance();
        }
        return std::make_shared<ReturnStatement>(expression);
    }

    AstNodePtr parseVariableDeclaration()
    {
        AstNodePtr name = parseIdentifier();
        AstNodePtr init;
        if (match(TokenType::OPERATOR) && peek().value == "=")
        {
            advance();
            init = parseExpression();
        }
        if (match(TokenType::PUNCTUATION) && peek().value == ";")
        {
            advance();
        }
        return std::make_shared<VariableDeclaration>(static_cast<Identifier *>(name.get())->name, init);
    }
};
