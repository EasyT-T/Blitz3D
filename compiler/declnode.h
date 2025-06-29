#ifndef DECLNODE_H
#define DECLNODE_H

#include "node.h"
#include "std.h"

struct DeclNode : public Node
{
    int pos;
    std::string file;

    DeclNode() : pos(-1)
    {
    }

    virtual void proto(DeclSeq* d, Environ* e)
    {
    }

    virtual void semant(Environ* e)
    {
    }

    virtual void translate(Codegen* g)
    {
    }

    virtual void transdata(Codegen* g)
    {
    }
};

struct DeclSeqNode : public Node
{
    std::vector<DeclNode*> decls;

    DeclSeqNode()
    {
    }

    ~DeclSeqNode() override { for (; decls.size(); decls.pop_back())delete decls.back(); }

    void proto(DeclSeq* d, Environ* e);

    void semant(Environ* e);

    void translate(Codegen* g);

    void transdata(Codegen* g);

    void push_back(DeclNode* d) { decls.push_back(d); }

    int size() { return decls.size(); }
};

#include "exprnode.h"
#include "stmtnode.h"

//'kind' shouldn't really be in Parser...
//should probably be LocalDeclNode,GlobalDeclNode,ParamDeclNode
struct VarDeclNode : public DeclNode
{
    std::string ident, tag;
    int kind;
    bool constant;
    ExprNode* expr;
    DeclVarNode* sem_var;

    VarDeclNode(const std::string& i, const std::string& t, const int k, const bool c, ExprNode* e) : ident(i), tag(t),
        kind(k), constant(c),
        expr(e), sem_var(nullptr)
    {
    }

    ~VarDeclNode() override
    {
        delete expr;
        delete sem_var;
    }

    void proto(DeclSeq* d, Environ* e) override;

    void semant(Environ* e) override;

    void translate(Codegen* g) override;
};

struct FuncDeclNode : public DeclNode
{
    std::string ident, tag;
    DeclSeqNode* params;
    StmtSeqNode* stmts;
    FuncType* sem_type;
    Environ* sem_env;

    FuncDeclNode(const std::string& i, const std::string& t, DeclSeqNode* p, StmtSeqNode* ss) : ident(i), tag(t),
        params(p),
        stmts(ss)
    {
    }

    ~FuncDeclNode() override
    {
        delete params;
        delete stmts;
    }

    void proto(DeclSeq* d, Environ* e) override;

    void semant(Environ* e) override;

    void translate(Codegen* g) override;
};

struct StructDeclNode : public DeclNode
{
    std::string ident;
    DeclSeqNode* fields;
    StructType* sem_type;

    StructDeclNode(const std::string& i, DeclSeqNode* f) : ident(i), fields(f)
    {
    }

    ~StructDeclNode() override { delete fields; }

    void proto(DeclSeq* d, Environ* e) override;

    void semant(Environ* e) override;

    void translate(Codegen* g) override;
};

struct DataDeclNode : public DeclNode
{
    ExprNode* expr;
    std::string str_label;

    DataDeclNode(ExprNode* e) : expr(e)
    {
    }

    ~DataDeclNode() override { delete expr; }

    void proto(DeclSeq* d, Environ* e) override;

    void semant(Environ* e) override;

    void translate(Codegen* g) override;

    void transdata(Codegen* g) override;
};

struct VectorDeclNode : public DeclNode
{
    std::string ident, tag;
    ExprSeqNode* exprs;
    int kind;
    VectorType* sem_type;

    VectorDeclNode(const std::string& i, const std::string& t, ExprSeqNode* e, const int k) : ident(i), tag(t),
        exprs(e), kind(k)
    {
    }

    ~VectorDeclNode() override { delete exprs; }

    void proto(DeclSeq* d, Environ* e) override;

    void translate(Codegen* g) override;
};

#endif
