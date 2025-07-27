#pragma once

#include <string>
#include <vector>
#include <memory>

class ASTNode {
    public:
        virtual ~ASTNode() = default;
};

class ASTVariable: public ASTNode {
    public:
        ASTVariable(std::string &n)
            : name(n)
        {}
    private:
        std::string name;
};

class ASTNumber: public ASTNode {
    public:
        ASTNumber(double n)
            : number(n)
        {}
    private:
        double number;
};

class ASTString: public ASTNode {
    public:
        ASTString(std::string &sl)
            : string_literal(sl)
        {}
    private:
        std::string string_literal;
};

class ASTUnaryOp: public ASTNode {
    public:
        ASTUnaryOp(char o, bool s, std::unique_ptr<ASTNode> l, std::unique_ptr<ASTNode> r)
            : op(o), side(s), lhs(std::move(l)), rhs(std::move(r))
        {}
    private:
        char op;
        bool side;
        std::unique_ptr<ASTNode> lhs, rhs;
};

class ASTBinaryOp: public ASTNode {
    public:
        ASTBinaryOp(char o, std::unique_ptr<ASTNode> l, std::unique_ptr<ASTNode> r)
            : op(o), lhs(std::move(l)), rhs(std::move(r))
        {}
    private:
        char op;
        std::unique_ptr<ASTNode> lhs, rhs;
};

class ASTIfStmt: public ASTNode {
    public:
        ASTIfStmt(std::unique_ptr<ASTNode> c, std::vector<std::unique_ptr<ASTNode>> eib, std::unique_ptr<ASTNode> eb)
            : condition(std::move(c)), else_if_branches(eib), else_branch(std::move(eb))
        {}
    private:
        std::unique_ptr<ASTNode> condition;
        std::vector<std::unique_ptr<ASTNode>> else_if_branches;
        std::unique_ptr<ASTNode> else_branch;
};

class ASTWhileLoop: public ASTNode {
    public:
        ASTWhileLoop(std::unique_ptr<ASTNode> c, std::unique_ptr<ASTNode> b)
            : condition(std::move(c)), body(std::move(b))
        {}
    private:
        std::unique_ptr<ASTNode> condition, body;
};