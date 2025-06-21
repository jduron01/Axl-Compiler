#pragma once

#include <memory>
#include <string>
#include <vector>

using std::unique_ptr;
using std::string;
using std::vector;
using std::move;

class ASTNode {
    public:
        virtual ~ASTNode() = default;
};

class ASTNumberNode : public ASTNode {
    public:
        ASTNumberNode(double v)
            : value(v)
        {}
    private:
        double value;
};

class ASTStringNode : public ASTNode {
    public:
        ASTStringNode(string v)
            : value(v)
        {}
    private:
        string value;
};

class ASTVariableNode : public ASTNode {
    public:
        ASTVariableNode(const string &n)
            : name(n)
        {}
    private:
        string name;
};

class ASTAssignNode : public ASTNode {
    public:
        ASTAssignNode(unique_ptr<ASTVariableNode> var, unique_ptr<ASTNode> val)
            : var(move(var)), value(move(val))
        {}
    private:
        unique_ptr<ASTVariableNode> var;
        unique_ptr<ASTNode> value;
};

class ASTBinaryNode : public ASTNode {
    public:
        ASTBinaryNode(char o, unique_ptr<ASTNode> l, unique_ptr<ASTNode> r)
            : op(o), lhs(move(l)), rhs(move(r))
        {}
    private:
        char op;
        unique_ptr<ASTNode> lhs;
        unique_ptr<ASTNode> rhs;
};

class ASTCallNode : public ASTNode {
    public:
        ASTCallNode(const string &c, vector<unique_ptr<ASTNode>> a)
            : callee(c), args(a)
        {}
    private:
        string callee;
        vector<unique_ptr<ASTNode>> args;
};

class ASTIfNode : public ASTNode {
    public:
        ASTIfNode(unique_ptr<ASTNode> c, unique_ptr<ASTNode> b,
                  unique_ptr<ASTNode> e)
            : condition(move(c)), body(move(b)), else_branch(move(e))
        {}
    private:
        unique_ptr<ASTNode> condition, body, else_branch;
};

class ASTElseIfNode : public ASTNode {
    public:
        ASTElseIfNode(unique_ptr<ASTNode> c)
            : condition(move(c))
        {}
    private:
        unique_ptr<ASTNode> condition;
};

class ASTForNode : public ASTNode {
    public:
        ASTForNode(const string &v, unique_ptr<ASTNode> l, unique_ptr<ASTNode> u,
                   unique_ptr<ASTNode> s, unique_ptr<ASTNode> b)
            : var(v), lower(move(l)), upper(move(u)), step(move(s)), body(move(b))
        {}
    private:
        string var;
        unique_ptr<ASTNode> lower, upper, step, body;
};

class ASTWhileNode : public ASTNode {
    public:
        ASTWhileNode(unique_ptr<ASTNode> c, unique_ptr<ASTNode> b)
            : condition(move(c)), body(move(b))
        {}
    private:
        unique_ptr<ASTNode> condition, body;
};

class ASTPrototypeNode : public ASTNode {
    public:
        ASTPrototypeNode(const string &n, vector<string> a)
            : name(n), args(a)
        {}
    private:
        string name;
        vector<string> args;
};

class ASTFunctionNode : public ASTNode {
    public:
        ASTFunctionNode(unique_ptr<ASTPrototypeNode> p, unique_ptr<ASTNode> b,
                        unique_ptr<ASTNode> r)
            : prototype(move(p)), body(move(b)), ret(move(r))
        {}
    private:
        unique_ptr<ASTPrototypeNode> prototype;
        unique_ptr<ASTNode> body, ret;
};