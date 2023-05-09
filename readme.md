This is C++ code that defines a simple interpreter for an abstract syntax tree (AST) representing a programming language. The interpreter can evaluate expressions and statements in the AST and update variables.

The code defines a few different classes that represent nodes in the AST. Each node class inherits from the ASTNode base class, which has a virtual destructor.

There are several classes for different kinds of expressions and statements, including:

- BinaryExpression: Represents an expression with a binary operator (e.g. 1 + 2)
- Literal: Represents a literal value (e.g. 42)
- ExpressionStatement: Wraps an expression in a statement (e.g. 2 * 3;)
- Program: Represents a whole program, made up of statements
- Identifier: Represents a variable name
- VariableDeclarator: Defines a variable and its initial value
- VariableDeclaration: Groups together variable declarators
- UpdateExpression: Updates a variable by incrementing or decrementing its value

# Compile With: 

g++ -I/Users/rafaelgonzalez/vcpkg/installed/arm64-osx/include -o interpreter main.cpp -std=c++17

# Run with:

./interpreter ast_files/binary_expressions.json

./interpreter ast_files/update_expressions.json

./interpreter ast_files/if_statement.json 