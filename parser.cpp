#include <iostream>
#include "parser.h"

using std::cout;
using std::cerr;
using std::endl;
using std::make_unique;

int main() {
    return 0;
}

int getNextToken() {
    return cur_token = getToken();
}

int getOperatorPrecedence(string s) {
    for (char c : s) {
        if (!isascii(c)) {
            return -1;
        }
    }

    if (s == "!") {
       return op_not;
    }

    if (s == "*" || s == "/" || s == "%") {
        return op_multiplicative;
    }

    if (s == "+" || s == "-") {
        return op_additive;
    }

    if (s == "<" || s == ">" || s == "<=" || s == ">=") {
        return op_relational;
    }

    if (s == "==" || s == "!=") {
        return op_equality;
    }
    
    if (s == "&&") {
        return op_and;
    }

    if (s == "||") {
        return op_or;
    }

    return -1;
}

unique_ptr<ASTNode> logError(string e) {
    cerr << "Error: " << e << endl;
    return nullptr;
}

unique_ptr<ASTPrototypeNode> logErrorProto(string e) {
    logError(e);
    return nullptr;
}

unique_ptr<ASTNode> parseNumber() {
    unique_ptr<ASTNumberNode> result = make_unique<ASTNumberNode>(number);
    getNextToken();

    return result;
}

unique_ptr<ASTNode> parseParens() {
    getNextToken();
    unique_ptr<ASTNode> expr = parseExpression();

    if (!expr) {
        return nullptr;
    }

    if (cur_token != ')') {
        return logError("expected ')'");
    }

    getNextToken();
    return expr;
}

unique_ptr<ASTNode> parseIdentifier() {
    string name = input;
    getNextToken();

    if (cur_token != '(') {
        return make_unique<ASTVariableNode>(name);
    }

    getNextToken();
    vector<unique_ptr<ASTNode>> args;

    if (cur_token != ')') {
        while (true) {
            if (unique_ptr<ASTNode> arg = parseExpression()) {
                args.push_back(arg);
            } else {
                return nullptr;
            }

            if (cur_token == ')') {
                break;
            }

            if (cur_token != ',') {
                return logError("expected ')' or ',' in argument list");
            }

            getNextToken();
        }
    }

    getNextToken();

    return make_unique<ASTCallNode>(name, args);
}

unique_ptr<ASTNode> parseIf() {
    getNextToken();
    unique_ptr<ASTNode> condition = parseExpression();

    if (!condition) {
        return nullptr;
    }

    if (cur_token != '{') {
        return logError("expected '{'");
    }

    getNextToken();
    unique_ptr<ASTNode> body = parseExpression();

    if (!body) {
        return nullptr;
    }

    if (cur_token == tok_sino) {
        getNextToken();
        unique_ptr<ASTNode> else_branch = parseExpression();

        if (!else_branch) {
            return nullptr;
        }

        if (cur_token != '}') {
            return logError("expected '}'");
        }

        getNextToken();

        return make_unique<ASTIfNode>(condition, body, else_branch);
    }

    if (cur_token != '}') {
        return logError("expected '}'");
    }

    getNextToken();

    return make_unique<ASTIfNode>(condition, body, nullptr);
}

// TODO: finish the rest of the parser functions.