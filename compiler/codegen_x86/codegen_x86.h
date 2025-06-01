
#include "../codegen.h"

struct Tile;

class Codegen_x86 : public Codegen {
public:
    Codegen_x86(std::ostream &out, bool debug);

    void enter(const std::string &l, int frameSize) override;

    void code(TNode *code) override;

    void leave(TNode *cleanup, int pop_sz) override;

    void label(const std::string &l) override;

    void i_data(int i, const std::string &l) override;

    void s_data(const std::string &s, const std::string &l) override;

    void p_data(const std::string &p, const std::string &l) override;

    void align_data(int n) override;

    void flush() override;

private:
    bool inCode;

    Tile *genCompare(TNode *t, std::string &func, bool negate);

    Tile *munch(TNode *t);        //munch and discard result
    Tile *munchReg(TNode *t);        //munch and put result in a CPU reg
    Tile *munchFP(TNode *t);        //munch and put result on FP stack

    Tile *munchCall(TNode *t);

    Tile *munchUnary(TNode *t);

    Tile *munchLogical(TNode *t);

    Tile *munchArith(TNode *t);

    Tile *munchShift(TNode *t);

    Tile *munchRelop(TNode *t);

    Tile *munchFPUnary(TNode *t);

    Tile *munchFPArith(TNode *t);

    Tile *munchFPRelop(TNode *t);
};