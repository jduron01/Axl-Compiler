#pragma once

static int type;

void parse();
void parseProgram();
void parsePrototype();
void parseBody();
void parseStatement();
void parseIf();
void parseWhile();
void parseFor();
void parseReturn();
void parseDeclaration();
void parseCall();
void parseExpression();
void parseAssignment();
void parseOr();
void parseAnd();
void parseEquality();
void parseRelational();
void parseAdditive();
void parseMultiplicative();
void parseUnary();
void parsePrimary();
void expect(int);
void next();