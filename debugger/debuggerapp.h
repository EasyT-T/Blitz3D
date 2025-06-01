#ifndef DEBUGGERAPP_H
#define DEBUGGERAPP_H

#include "mainframe.h"

class DebuggerApp final : public CWinApp
{
    MainFrame* main_frame = nullptr;

public:
    ~DebuggerApp() override;

    BOOL InitInstance() override;

    int ExitInstance() override;

    MainFrame* mainFrame();
};

extern DebuggerApp debuggerApp;

#endif
