#include "ast.hpp"
using json = nlohmann::json;

unique_ptr<ASTNode> parseAST(const json& ast) {
    string type = ast["type"];
    if (type == "Program") {
        auto program = make_unique<Program>();
        for (auto& statement : ast["body"]) {
            program->addStatement(parseAST(statement));
        }
        return program;
    } else if (type == "ExpressionStatement") {
        auto expression = ast["expression"];
        auto exprStmt = make_unique<ExpressionStatement>(parseAST(expression));
        return exprStmt;
    } else if (type == "BinaryExpression") {
        auto left = parseAST(ast["left"]);
        auto right = parseAST(ast["right"]);
        TokenType op;
        if (ast["operator"] == "+") {
            op = TokenType::Plus;
        } else if (ast["operator"] == "-") {
            op = TokenType::Minus;
        } else if (ast["operator"] == "*") {
            op = TokenType::Multiply;
        } else if (ast["operator"] == "/") {
            op = TokenType::Divide;
        } else {
            cerr << "Error: unsupported operator " << ast["operator"] << endl;
            exit(1);
        }
        auto binaryExpr = make_unique<BinaryExpression>(op, move(left), move(right));
        return binaryExpr;
    } else if (type == "Literal") {
        auto value = ast["value"].get<float>();
        auto literal = make_unique<Literal>(value);
        return literal;
    } else {
        cerr << "Error: unsupported AST node type " << type << endl;
        exit(1);
    }
}
