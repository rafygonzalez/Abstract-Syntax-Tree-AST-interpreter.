#include "ASTNodes.hpp"

class ASTInterpreter
{
private:
    unordered_map<string, float> varMap;

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
            case TokenType::EqualEqual:
                return leftValue == rightValue;
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
        else if (auto varDecl = dynamic_cast<VariableDeclaration *>(&node))
        {
            for (auto &declarator : varDecl->declarations)
            {
                evaluate(*declarator);
            }
            return 0;
        }
        else if (auto varDeclr = dynamic_cast<VariableDeclarator *>(&node))
        {
            float initValue = evaluate(*varDeclr->init);
            varMap[static_cast<Identifier *>(varDeclr->id.get())->name] = initValue;

            return initValue;
        }
        else if (auto identifier = dynamic_cast<Identifier *>(&node))
        {
            return varMap[identifier->name];
        }
        else if (auto updateExpr = dynamic_cast<UpdateExpression *>(&node))
        {
            float value = evaluate(*updateExpr->argument);
            if (updateExpr->op == TokenType::PlusPlus)
            {
                varMap[static_cast<Identifier *>(updateExpr->argument.get())->name] = value + 1;

                return value + 1;
            }
            else if (updateExpr->op == TokenType::MinusMinus)
            {
                varMap[static_cast<Identifier *>(updateExpr->argument.get())->name] = value - 1;
                return value - 1;
            }
        }
        else if (auto ifStmt = dynamic_cast<IfStatement *>(&node))
        {
            bool condition = evaluate(*ifStmt->test);
            if (condition)
            {
                return evaluate(*ifStmt->consequent);
            }
            else if (ifStmt->alternate)
            {
                return evaluate(*ifStmt->alternate);
            }
            return 0;
        }
        else if (auto blockStmt = dynamic_cast<BlockStatement *>(&node))
        {
            float result = 0;
            for (auto &statement : blockStmt->body)
            {
                result = evaluate(*statement);
            }
            return result;
        }
        return 0;
    }
};