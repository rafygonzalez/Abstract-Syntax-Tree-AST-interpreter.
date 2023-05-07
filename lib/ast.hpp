#include <iostream>

using namespace std;

enum class TokenType
{
    Plus,
    Minus,
    Multiply,
    Divide,
    Number,
};

class Token
{
public:
    TokenType type;
    string value;

    Token(TokenType type, string value) : type(type), value(value) {}
};

class ASTNode
{
public:
    virtual ~ASTNode() = default;
};

class BinaryExpression : public ASTNode
{
public:
    TokenType op;
    unique_ptr<ASTNode> left;
    unique_ptr<ASTNode> right;

    BinaryExpression(TokenType op, unique_ptr<ASTNode> left, unique_ptr<ASTNode> right)
        : op(op), left(move(left)), right(move(right)) {}
};

class Literal : public ASTNode
{
public:
    float value;

    Literal(float value) : value(value) {}
};

class ExpressionStatement : public ASTNode
{
public:
    unique_ptr<ASTNode> expression;

    ExpressionStatement(unique_ptr<ASTNode> expression) : expression(move(expression)) {}
};

class Program : public ASTNode
{
public:
    vector<unique_ptr<ASTNode>> statements;

    void addStatement(unique_ptr<ASTNode> statement)
    {
        statements.push_back(move(statement));
    }
};

class ASTInterpreter
{
public:
    float evaluate(ASTNode &node)
    {
        if (auto binaryExpr = dynamic_cast<BinaryExpression *>(&node))
        {
            float leftValue = evaluate(*binaryExpr->left);
            float rightValue = evaluate(*binaryExpr->right);
            switch (binaryExpr->op)
            {
            case TokenType::Plus:
                return leftValue + rightValue;
            case TokenType::Minus:
                return leftValue - rightValue;
            case TokenType::Multiply:
                return leftValue * rightValue;
            case TokenType::Divide:
                return leftValue / rightValue;
            }
        }
        else if (auto literal = dynamic_cast<Literal *>(&node))
        {
            return literal->value;
        }
        else if (auto exprStmt = dynamic_cast<ExpressionStatement *>(&node))
        {
            return evaluate(*exprStmt->expression);
        }
        else if (auto program = dynamic_cast<Program *>(&node))
        {
            float result = 0;
            for (auto &statement : program->statements)
            {
                result = evaluate(*statement);
            }
            return result;
        }
        return 0;
    }
};