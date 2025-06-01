
#ifndef MAINFRAME_H
#define MAINFRAME_H

#include "debugger.h"
#include "debugtree.h"
#include "sourcefile.h"
#include "tabber.h"

class MainFrame : public CFrameWnd, public Debugger {

    Tabber tabber;
    Tabber tabber2;
    CToolBar toolBar;
    SourceFile debug_log;
    ConstsTree consts_tree;
    GlobalsTree globals_tree;
    LocalsTree locals_tree;
    std::map<const char *, int> file_tabs;
    std::map<const char *, SourceFile *> files;

    int state, step_level, cur_pos;
    const char *cur_file;

    bool shouldRun() const { return step_level < locals_tree.size(); }

public:
    MainFrame();

    ~MainFrame() override;

    void debugRun() override;

    void debugStop() override;

    void debugStmt(int srcpos, const char *file) override;

    void debugEnter(void *frame, void *env, const char *func) override;

    void debugLeave() override;

    void debugLog(const char *msg) override;

    void debugMsg(const char *msg, bool serious) override;

    void debugSys(void *msg) override;

    void showCurStmt();

    void setState(int n);

    void setRuntime(void *mod, void *env);

    SourceFile *sourceFile(const char *file);

DECLARE_DYNAMIC(MainFrame)

DECLARE_MESSAGE_MAP()

    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

    afx_msg void OnSize(UINT type, int w, int h);

    afx_msg void OnClose();

    afx_msg void cmdStop();

    afx_msg void cmdRun();

    afx_msg void cmdStepOver();

    afx_msg void cmdStepInto();

    afx_msg void cmdStepOut();

    afx_msg void cmdEnd();

    afx_msg void updateCmdUI(CCmdUI *ui);

    afx_msg void OnWindowPosChanging(WINDOWPOS *pos);
};

#endif
