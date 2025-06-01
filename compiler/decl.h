#ifndef DECL_H
#define DECL_H

#include "std.h"

enum
{
    DECL_FUNC = 1, DECL_ARRAY = 2, DECL_STRUCT = 4, //NOT vars
    DECL_GLOBAL = 8, DECL_LOCAL = 16, DECL_PARAM = 32, DECL_FIELD = 64 //ARE vars
};

struct Type;
struct ConstType;

struct Decl
{
    std::string name;
    Type* type; //type
    int kind, offset;
    ConstType* defType; //default value
    Decl(const std::string& s, Type* t, const int k, ConstType* d = nullptr) : name(s), type(t), kind(k), defType(d)
    {
    }

    ~Decl();

    virtual void getName(char* buff);
};

struct DeclSeq
{
    std::vector<Decl*> decls;

    DeclSeq();

    ~DeclSeq();

    Decl* findDecl(const std::string& s);

    Decl* insertDecl(const std::string& s, Type* t, int kind, ConstType* d = nullptr);

    int size() { return decls.size(); }
};

#endif
