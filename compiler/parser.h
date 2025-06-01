/*

  The parser builds an abstact syntax tree from input tokens.

*/

#ifndef PARSER_H
#define PARSER_H

#include "nodes.h"
#include "toker.h"

class Parser
{
public:
    Parser(Toker& t);

    ProgNode* parse(const std::string& main);

private:
    enum Dialect
    {
        DIALECT_CLASSIC,
        DIALECT_SECURE,
        DIALECT_MODERN,
    };

    Dialect dialect = DIALECT_CLASSIC;

    std::string incfile;
    std::set<std::string> included;
    Toker* toker,* main_toker;
    std::map<std::string, DimNode*> arrayDecls;

    DeclSeqNode* consts;
    DeclSeqNode* structs;
    DeclSeqNode* funcs;
    DeclSeqNode* datas;

    StmtSeqNode* parseStmtSeq(int scope);

    void parseStmtSeq(StmtSeqNode* stmts, int scope);

    void ex(const std::string& s);

    void exp(const std::string& s);

    std::string parseIdent();

    void parseChar(int c);

    std::string parseTypeTag();

    VarNode* parseVar(bool mustExist = false);

    VarNode* parseVar(const std::string& ident, const std::string& tag, bool mustExist = false);

    CallNode* parseCall(const std::string& ident, const std::string& tag);

    IfNode* parseIf();

    DeclNode* parseVarDecl(int kind, bool constant);

    DimNode* parseArrayDecl();

    DeclNode* parseFuncDecl();

    DeclNode* parseStructDecl();

    ExprSeqNode* parseExprSeq();

    ExprNode* parseExpr(bool opt);

    ExprNode* parseExpr1(bool opt); //And, Or, Eor
    ExprNode* parseExpr2(bool opt); //<,=,>,<=,<>,>=
    ExprNode* parseExpr3(bool opt); //+,-
    ExprNode* parseExpr4(bool opt); //Lsr,Lsr,Asr
    ExprNode* parseExpr5(bool opt); //*,/,Mod
    ExprNode* parseExpr6(bool opt); //^
    ExprNode* parseUniExpr(bool opt); //+,-,Not,~
    ExprNode* parsePrimary(bool opt);
};

#endif
