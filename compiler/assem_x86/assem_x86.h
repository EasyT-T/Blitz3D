#ifndef ASSEM_X86_H
#define ASSEM_X86_H

#include "../assem.h"

#include <iostream>
#include <string>
#include <vector>

#include "insts.h"
#include "operand.h"

class Assem_x86 : public Assem
{
public:
    Assem_x86(std::istream& in, Module* mod);

    void assemble() override;

private:
    void align(int n) const;

    void emit(int n) const;

    void emitw(int n);

    void emitd(int n);

    void emitImm(const std::string& s, int size);

    void emitImm(const Operand& o, int size);

    void r_reloc(const std::string& dest);

    void a_reloc(const std::string& dest);

    void assemDir(const std::string& name, const std::string& op);

    void assemInst(const std::string& name, const std::string& lhs, const std::string& rhs);

    void assemLine(const std::string& line);
};

#endif
