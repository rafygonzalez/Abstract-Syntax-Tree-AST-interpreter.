#include <iostream>

using namespace std;

enum class TokenType
{
    Plus,
    Minus,
    Multiply,
    Divide,
    Number,
    PlusPlus,
    MinusMinus,
    EqualEqual
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

class Identifier : public ASTNode
{
public:
    string name;

    Identifier(string name) : name(name) {}
};

class VariableDeclarator : public ASTNode
{
public:
    unique_ptr<ASTNode> id;
    unique_ptr<ASTNode> init;

    VariableDeclarator(unique_ptr<ASTNode> id, unique_ptr<ASTNode> init)
        : id(move(id)), init(move(init)) {}
};

class VariableDeclaration : public ASTNode
{
public:
    vector<unique_ptr<ASTNode>> declarations;

    void addDeclaration(unique_ptr<ASTNode> declaration)
    {
        declarations.push_back(move(declaration));
    }
};

class UpdateExpression : public ASTNode
{
public:
    TokenType op;
    unique_ptr<ASTNode> argument;

    UpdateExpression(TokenType op, unique_ptr<ASTNode> argument)
        : op(op), argument(move(argument)) {}
};

class IfStatement : public ASTNode
{
public:
    unique_ptr<ASTNode> test;
    unique_ptr<ASTNode> consequent;
    unique_ptr<ASTNode> alternate;
    IfStatement(unique_ptr<ASTNode> test, unique_ptr<ASTNode> consequent)
        : test(move(test)), consequent(move(consequent)) {}

    void setAlternate(unique_ptr<ASTNode> alternate)
    {
        this->alternate = move(alternate);
    }
};

class BlockStatement : public ASTNode
{
public:
    vector<unique_ptr<ASTNode>> body;

    void addStatement(unique_ptr<ASTNode> statement)
    {
        body.push_back(move(statement));
    }
};
