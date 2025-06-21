#pragma once

#include "lexer.h"
#include "ast.h"

enum OperatorPrecedence {
    op_or,
    op_and,
    op_equality,
    op_relational,
    op_additive,
    op_multiplicative,
    op_not
};

static int cur_token;

int getNextToken();
int getOperatorPrecedence(string);
unique_ptr<ASTNode> logError(string);
unique_ptr<ASTPrototypeNode> logErrorProto(string);
unique_ptr<ASTNode> parseNumber();
unique_ptr<ASTNode> parseString();
unique_ptr<ASTNode> parseParens();
unique_ptr<ASTNode> parseIdentifier();
unique_ptr<ASTNode> parseIf();
unique_ptr<ASTNode> parseFor();
unique_ptr<ASTNode> parsePrimary();
unique_ptr<ASTNode> parseBinaryOp();
unique_ptr<ASTNode> parseExpression();
unique_ptr<ASTPrototypeNode> parsePrototype();
unique_ptr<ASTFunctionNode> parseDefinition();
unique_ptr<ASTFunctionNode> parseTopLevelExpr();
void handleDefinition();
void handleTopLevelExpression();
void mainLoop();