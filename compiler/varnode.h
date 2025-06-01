#ifndef VARNODE_H
#define VARNODE_H

#include "exprnode.h"
#include "node.h"
#include "std.h"

struct VarNode : public Node
{
    Type* sem_type;

    //get set var
    TNode* load(Codegen* g);

    virtual TNode* store(Codegen* g, TNode* n);

    virtual bool isObjParam();

    //addr of var
    virtual void semant(Environ* e) = 0;

    virtual TNode* translate(Codegen* g) = 0;
};

#include "decl.h"

struct DeclVarNode : public VarNode
{
    Decl* sem_decl;

    DeclVarNode(Decl* d = nullptr) : sem_decl(d) { if (d) sem_type = d->type; }

    void semant(Environ* e) override;

    TNode* translate(Codegen* g) override;

    TNode* store(Codegen* g, TNode* n) override;

    bool isObjParam() override;
};

struct IdentVarNode : public DeclVarNode
{
    std::string ident, tag;
    bool mustExist;

    IdentVarNode(const std::string& i, const std::string& t, const bool mustExist) : ident(i), tag(t),
        mustExist(mustExist)
    {
    }

    void semant(Environ* e) override;
};

struct ArrayVarNode : public VarNode
{
    std::string ident, tag;
    ExprSeqNode* exprs;
    Decl* sem_decl;

    ArrayVarNode(const std::string& i, const std::string& t, ExprSeqNode* e) : ident(i), tag(t), exprs(e)
    {
    }

    ~ArrayVarNode() override { delete exprs; }

    void semant(Environ* e) override;

    TNode* translate(Codegen* g) override;
};

struct FieldVarNode : public VarNode
{
    ExprNode* expr;
    std::string ident, tag;
    Decl* sem_field;

    FieldVarNode(ExprNode* e, const std::string& i, const std::string& t) : expr(e), ident(i), tag(t)
    {
    }

    ~FieldVarNode() override { delete expr; }

    void semant(Environ* e) override;

    TNode* translate(Codegen* g) override;
};

struct VectorVarNode : public VarNode
{
    ExprNode* expr;
    ExprSeqNode* exprs;
    VectorType* vec_type;

    VectorVarNode(ExprNode* e, ExprSeqNode* es) : expr(e), exprs(es)
    {
    }

    ~VectorVarNode() override
    {
        delete expr;
        delete exprs;
    }

    void semant(Environ* e) override;

    TNode* translate(Codegen* g) override;
};

#endif
