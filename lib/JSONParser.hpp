#include "ASTInterpreter.hpp"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

unique_ptr<ASTNode> parseAST(const json &ast)
{
    string type = ast["type"];
    if (type == "Program")
    {
        auto program = make_unique<Program>();
        for (auto &statement : ast["body"])
        {
            program->addStatement(parseAST(statement));
        }
        return program;
    }
    else if (type == "ExpressionStatement")
    {
        auto expression = ast["expression"];
        auto exprStmt = make_unique<ExpressionStatement>(parseAST(expression));
        return exprStmt;
    }
    else if (type == "BinaryExpression")
    {
        auto left = parseAST(ast["left"]);
        auto right = parseAST(ast["right"]);
        TokenType op;
        if (ast["operator"] == "+")
        {
            op = TokenType::Plus;
        }
        else if (ast["operator"] == "-")
        {
            op = TokenType::Minus;
        }
        else if (ast["operator"] == "*")
        {
            op = TokenType::Multiply;
        }
        else if (ast["operator"] == "/")
        {
            op = TokenType::Divide;
        }
        else if (ast["operator"] == "==")
        {
            op = TokenType::EqualEqual;
        }
        else
        {
            cerr << "Error: unsupported operator " << ast["operator"] << endl;
            exit(1);
        }
        auto binaryExpr = make_unique<BinaryExpression>(op, move(left), move(right));
        return binaryExpr;
    }
    else if (type == "Literal")
    {
        auto value = ast["value"].get<float>();
        auto literal = make_unique<Literal>(value);
        return literal;
    }
    else if (type == "Identifier")
    {
        auto name = ast["name"];
        auto identifier = make_unique<Identifier>(name);
        return identifier;
    }
    else if (type == "VariableDeclarator")
    {
        auto id = parseAST(ast["id"]);
        auto init = parseAST(ast["init"]);
        auto varDecl = make_unique<VariableDeclarator>(move(id), move(init));
        return varDecl;
    }
    else if (type == "VariableDeclaration")
    {
        auto varDecl = make_unique<VariableDeclaration>();
        for (auto &declarator : ast["declarations"])
        {
            varDecl->addDeclaration(parseAST(declarator));
        }
        return varDecl;
    }
    else if (type == "UpdateExpression")
    {
        auto argument = parseAST(ast["argument"]);
        TokenType op;
        if (ast["operator"] == "++")
        {
            op = TokenType::PlusPlus;
        }
        else if (ast["operator"] == "--")
        {
            op = TokenType::MinusMinus;
        }
        else
        {
            cerr << "Error: unsupported operator " << ast["operator"] << endl;
            exit(1);
        }
        auto updateExpr = make_unique<UpdateExpression>(op, move(argument));
        return updateExpr;
    }
    else if (type == "BlockStatement")
    {
        auto blockStmt = make_unique<BlockStatement>();
        for (auto &statement : ast["body"])
        {
            blockStmt->addStatement(parseAST(statement));
        }
        return blockStmt;
    }
    else if (type == "IfStatement")
    {
        auto test = parseAST(ast["test"]);
        auto consequent = parseAST(ast["consequent"]);
        auto ifStmt = make_unique<IfStatement>(move(test), move(consequent));

        if (ast.find("alternate") != ast.end() && !ast["alternate"].is_null())
        {
            auto alternate = parseAST(ast["alternate"]);
            ifStmt->setAlternate(move(alternate));
        }

        return ifStmt;
    }
    else
    {
        cerr << "Error: unsupported AST node type " << type << endl;
        exit(1);
    }
}