#include "nodes.h"

std::set<std::string> Node::usedfuncs;

///////////////////////////////
// generic exception thrower //
///////////////////////////////
void Node::ex()
{
    ex("INTERNAL COMPILER ERROR");
}

void Node::ex(const std::string& e)
{
    throw Ex(e, -1, "");
}

void Node::ex(const std::string& e, const int pos)
{
    throw Ex(e, pos, "");
}

void Node::ex(const std::string& e, const int pos, const std::string& f)
{
    throw Ex(e, pos, f);
}

///////////////////////////////
// Generate a local variable //
///////////////////////////////
VarNode* Node::genLocal(Environ* e, Type* ty)
{
    const std::string t = genLabel();
    Decl* d = e->decls->insertDecl(t, ty, DECL_LOCAL);
    return d_new DeclVarNode(d);
}

/////////////////////////////////////////////////
// if type is const, return const value else 0 //
/////////////////////////////////////////////////
ConstNode* Node::constValue(Type* ty)
{
    const ConstType* c = ty->constType();
    if (!c) return nullptr;
    ty = c->valueType;
    if (ty == Type::int_type) return d_new IntConstNode(c->intValue);
    if (ty == Type::float_type) return d_new FloatConstNode(c->floatValue);
    return d_new StringConstNode(c->stringValue);
}

///////////////////////////////////////////////////////
// work out var offsets - return size of local frame //
///////////////////////////////////////////////////////
int Node::enumVars(Environ* e)
{
    //calc offsets
    int p_size = 0, l_size = 0;
    for (int k = 0; k < e->decls->size(); ++k)
    {
        Decl* d = e->decls->decls[k];
        if (d->kind & DECL_PARAM)
        {
            d->offset = p_size + 20;
            p_size += 4;
        }
        else if (d->kind & DECL_LOCAL)
        {
            d->offset = -4 - l_size;
            l_size += 4;
        }
    }
    return l_size;
}

//////////////////////////////
// initialize all vars to 0 //
//////////////////////////////
TNode* Node::createVars(Environ* e)
{
    int k;
    TNode* t = nullptr;
    //initialize locals
    for (k = 0; k < e->decls->size(); ++k)
    {
        const Decl* d = e->decls->decls[k];
        if (d->kind != DECL_LOCAL) continue;
        if (d->type->vectorType()) continue;
        if (!t) t = d_new TNode(IR_CONST, nullptr, nullptr, 0);
        TNode* p = d_new TNode(IR_LOCAL, nullptr, nullptr, d->offset);
        p = d_new TNode(IR_MEM, p, nullptr);
        t = d_new TNode(IR_MOVE, t, p);
    }
    //initialize vectors
    for (k = 0; k < e->decls->size(); ++k)
    {
        const Decl* d = e->decls->decls[k];
        if (d->kind == DECL_PARAM) continue;
        const VectorType* v = d->type->vectorType();
        if (!v) continue;
        TNode* p = call("__bbVecAlloc", global(v->label));
        TNode* m = d->kind == DECL_GLOBAL ? global("_v" + d->name) : local(d->offset);
        p = move(p, mem(m));
        if (t) t = seq(t, p);
        else t = p;
    }
    return t;
}

////////////////////////
// release local vars //
////////////////////////
TNode* Node::deleteVars(Environ* e)
{
    TNode* t = nullptr,* l = nullptr,* p1,* p2;
    for (int k = 0; k < e->decls->size(); ++k)
    {
        const Decl* d = e->decls->decls[k];
        Type* type = d->type;
        std::string func;
        if (type == Type::string_type)
        {
            if (d->kind == DECL_LOCAL || d->kind == DECL_PARAM)
            {
                func = "__bbStrRelease";
                p1 = mem(local(d->offset));
                p2 = nullptr;
            }
        }
        else if (type->structType())
        {
            if (d->kind == DECL_LOCAL)
            {
                func = "__bbObjRelease";
                p1 = mem(local(d->offset));
                p2 = nullptr;
            }
        }
        else if (const VectorType* v = type->vectorType())
        {
            if (d->kind == DECL_LOCAL)
            {
                func = "__bbVecFree";
                p1 = mem(local(d->offset));
                p2 = global(v->label);
            }
        }
        if (!func.size()) continue;
        TNode* p = d_new TNode(IR_SEQ, call(func, p1, p2), nullptr);
        (l ? l->r : t) = p;
        l = p;
    }
    return t;
}

//////////////////////////////////////////////////////////////
// compare 2 translated operands - return 1 if true, else 0 //
//////////////////////////////////////////////////////////////
TNode* Node::compare(const int op, TNode* l, TNode* r, Type* ty)
{
    int n = 0;
    if (ty == Type::float_type)
    {
        switch (op)
        {
        case '=':
            n = IR_FSETEQ;
            break;
        case NE:
            n = IR_FSETNE;
            break;
        case '<':
            n = IR_FSETLT;
            break;
        case '>':
            n = IR_FSETGT;
            break;
        case LE:
            n = IR_FSETLE;
            break;
        case GE:
            n = IR_FSETGE;
            break;
        }
    }
    else
    {
        switch (op)
        {
        case '=':
            n = IR_SETEQ;
            break;
        case NE:
            n = IR_SETNE;
            break;
        case '<':
            n = IR_SETLT;
            break;
        case '>':
            n = IR_SETGT;
            break;
        case LE:
            n = IR_SETLE;
            break;
        case GE:
            n = IR_SETGE;
            break;
        }
    }
    if (ty == Type::string_type)
    {
        l = call("__bbStrCompare", l, r);
        r = d_new TNode(IR_CONST, nullptr, nullptr, 0);
    }
    else if (ty->structType())
    {
        l = call("__bbObjCompare", l, r);
        r = d_new TNode(IR_CONST, nullptr, nullptr, 0);
    }
    return d_new TNode(n, l, r);
}

/////////////////////////////////
// calculate the type of a tag //
/////////////////////////////////
Type* Node::tagType(const std::string& tag, Environ* e)
{
    Type* t;
    if (tag.size())
    {
        t = e->findType(tag);
        if (!t) ex("Type \"" + tag + "\" not found");
    }
    else t = nullptr;
    return t;
}

////////////////////////////////
// Generate a fresh ASM label //
////////////////////////////////
std::string Node::genLabel()
{
    static int cnt;
    return "_" + itoa(++cnt & 0x7fffffff);
}

//////////////////////////////////////////////////////
// create a stmt-type function call with int result //
//////////////////////////////////////////////////////
TNode* Node::call(const std::string& func, TNode* a0, TNode* a1, TNode* a2)
{
    int size = 0;
    TNode* t = nullptr;
    if (a0)
    {
        t = move(a0, mem(arg(0)));
        size += 4;
        if (a1)
        {
            t = seq(t, move(a1, mem(arg(4))));
            size += 4;
            if (a2)
            {
                t = seq(t, move(a2, mem(arg(8))));
                size += 4;
            }
        }
    }
    TNode* l = d_new TNode(IR_GLOBAL, 0, 0, func);
    return d_new TNode(IR_CALL, l, t, size);
}

////////////////////////////////////////////////////////
// create a stmt-type function call with float result //
////////////////////////////////////////////////////////
TNode* Node::fcall(const std::string& func, TNode* a0, TNode* a1, TNode* a2)
{
    int size = 0;
    TNode* t = nullptr;
    if (a0)
    {
        t = move(a0, mem(arg(0)));
        size += 4;
        if (a1)
        {
            t = seq(t, move(a1, mem(arg(4))));
            size += 4;
            if (a2)
            {
                t = seq(t, move(a2, mem(arg(8))));
                size += 4;
            }
        }
    }
    TNode* l = d_new TNode(IR_GLOBAL, 0, 0, func);
    return d_new TNode(IR_FCALL, l, t, size);
}

TNode* Node::seq(TNode* l, TNode* r)
{
    return d_new TNode(IR_SEQ, l, r);
}

TNode* Node::move(TNode* src, TNode* dest)
{
    return d_new TNode(IR_MOVE, src, dest);
}

TNode* Node::global(const std::string& s)
{
    return d_new TNode(IR_GLOBAL, 0, 0, s);
}

TNode* Node::local(const int offset)
{
    return d_new TNode(IR_LOCAL, nullptr, nullptr, offset);
}

TNode* Node::arg(const int offset)
{
    return d_new TNode(IR_ARG, nullptr, nullptr, offset);
}

TNode* Node::mem(TNode* ref)
{
    return d_new TNode(IR_MEM, ref, nullptr);
}

TNode* Node::add(TNode* l, TNode* r)
{
    return d_new TNode(IR_ADD, l, r);
}

TNode* Node::mul(TNode* l, TNode* r)
{
    return d_new TNode(IR_MUL, l, r);
}

TNode* Node::iconst(const int n)
{
    return d_new TNode(IR_CONST, nullptr, nullptr, n);
}

TNode* Node::ret()
{
    return d_new TNode(IR_RET, nullptr, nullptr);
}

TNode* Node::jsr(const std::string& s)
{
    return d_new TNode(IR_JSR, 0, 0, s);
}

TNode* Node::jump(const std::string& s)
{
    return d_new TNode(IR_JUMP, 0, 0, s);
}

TNode* Node::jumpt(TNode* expr, const std::string& s)
{
    return d_new TNode(IR_JUMPT, expr, 0, s);
}

TNode* Node::jumpf(TNode* expr, const std::string& s)
{
    return d_new TNode(IR_JUMPF, expr, 0, s);
}

TNode* Node::jumpge(TNode* l, TNode* r, const std::string& s)
{
    return d_new TNode(IR_JUMPGE, l, r, s);
}
