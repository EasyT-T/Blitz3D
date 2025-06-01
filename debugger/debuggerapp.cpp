
#include "debuggerapp.h"
#include "debugger.h"
#include "resource.h"
#include "stdafx.h"
#include "../blitzide/prefs.h"

DebuggerApp debuggerApp;

DebuggerApp::~DebuggerApp() {
}

BOOL DebuggerApp::InitInstance() {

    AfxInitRichEdit();

    main_frame = new MainFrame();

    m_pMainWnd = main_frame;

    RECT rect;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);

    const int x = rect.left;
    const int w = rect.right - x;
    const int h = 240;
    const int y = rect.bottom - h;

    main_frame->Create(0, "Blitz Debugger",
                       WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
                       CRect(x, y, x + w, y + h));
    main_frame->ShowWindow(SW_SHOW);
    main_frame->UpdateWindow();

    return TRUE;
}

int DebuggerApp::ExitInstance() {
    main_frame->DestroyWindow();
    return 0;
}

MainFrame *DebuggerApp::mainFrame() {
    return debuggerApp.main_frame;
}

Debugger *_cdecl

debuggerGetDebugger(void *mod, void *env) {
    debuggerApp.mainFrame()->setRuntime(mod, env);
    return debuggerApp.mainFrame();
}
