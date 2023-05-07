#include <fstream>
#include <nlohmann/json.hpp>

#include "lib/parserAST.hpp"

using namespace std;
using json = nlohmann::json;

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        cerr << "Usage: " << argv[0] << " <json-file>" << endl;
        return 1;
    }

    string filename = argv[1];
    ifstream input(filename);
    if (!input.good())
    {
        cerr << "Error: failed to open " << filename << endl;
        return 1;
    }

    json ast;
    input >> ast;

    unique_ptr<ASTNode> node = parseAST(ast);
    ASTInterpreter interpreter;
    auto result = interpreter.evaluate(*node);
    cout << "Result: " << result << endl;

    return 0;
}
