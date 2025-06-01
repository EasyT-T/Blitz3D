#ifndef TYPE_H
#define TYPE_H

#include "decl.h"

struct FuncType;
struct ArrayType;
struct StructType;
struct ConstType;
struct VectorType;

struct Type
{
    virtual ~Type()
    {
    }

    virtual bool intType() { return 0; }

    virtual bool floatType() { return 0; }

    virtual bool stringType() { return 0; }

    //casts to inherited types
    virtual FuncType* funcType() { return nullptr; }

    virtual ArrayType* arrayType() { return nullptr; }

    virtual StructType* structType() { return nullptr; }

    virtual ConstType* constType() { return nullptr; }

    virtual VectorType* vectorType() { return nullptr; }

    //operators
    virtual bool canCastTo(Type* t) { return this == t; }

    //built in types
    static Type* void_type,* int_type,* float_type,* string_type,* null_type;
};

struct FuncType : public Type
{
    Type* returnType;
    DeclSeq* params;
    bool userlib, cfunc;

    FuncType(Type* t, DeclSeq* p, const bool ulib, const bool cfn) : returnType(t), params(p), userlib(ulib), cfunc(cfn)
    {
    }

    ~FuncType() override { delete params; }

    FuncType* funcType() override { return this; }
};

struct ArrayType : public Type
{
    Type* elementType;
    int dims;

    ArrayType(Type* t, const int n) : elementType(t), dims(n)
    {
    }

    ArrayType* arrayType() override { return this; }
};

struct StructType : public Type
{
    std::string ident;
    DeclSeq* fields;

    StructType(const std::string& i) : ident(i), fields(nullptr)
    {
    }

    StructType(const std::string& i, DeclSeq* f) : ident(i), fields(f)
    {
    }

    ~StructType() override { delete fields; }

    StructType* structType() override { return this; }

    bool canCastTo(Type* t) override;
};

struct ConstType : public Type
{
    Type* valueType;
    int intValue;
    float floatValue;
    std::string stringValue;

    ConstType(const int n) : intValue(n), valueType(Type::int_type)
    {
    }

    ConstType(const float n) : floatValue(n), valueType(Type::float_type)
    {
    }

    ConstType(const std::string& n) : stringValue(n), valueType(Type::string_type)
    {
    }

    ConstType* constType() override { return this; }
};

struct VectorType : public Type
{
    std::string label;
    Type* elementType;
    std::vector<int> sizes;

    VectorType(const std::string& l, Type* t, const std::vector<int>& szs) : label(l), elementType(t), sizes(szs)
    {
    }

    VectorType* vectorType() override { return this; }

    bool canCastTo(Type* t) override;
};

#endif
