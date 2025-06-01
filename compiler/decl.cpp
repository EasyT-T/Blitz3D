#include "decl.h"
#include "type.h"

Decl::~Decl()
{
}

DeclSeq::DeclSeq()
{
}

void Decl::getName(char* buff)
{
    const int sz = name.size();
    memcpy(buff, name.data(), sz);
    buff[sz] = 0;
}

DeclSeq::~DeclSeq()
{
    for (; decls.size(); decls.pop_back()) delete decls.back();
}

Decl* DeclSeq::findDecl(const std::string& s)
{
    for (std::vector<Decl*>::iterator it = decls.begin(); it != decls.end(); ++it)
    {
        if ((*it)->name == s) return *it;
    }
    return nullptr;
}

Decl* DeclSeq::insertDecl(const std::string& s, Type* t, const int kind, ConstType* d)
{
    if (findDecl(s)) return nullptr;
    decls.push_back(d_new Decl(s, t, kind, d));
    return decls.back();
}
