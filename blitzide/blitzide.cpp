#include "blitzide.h"
#include "about.h"
#include "libs.h"
#include "mainframe.h"
#include "prefs.h"
#include "resource.h"
#include "stdafx.h"

BlitzIDE blitzIDE;

BOOL BlitzIDE::InitInstance()
{
#ifdef _DEBUG
    AfxEnableMemoryTracking(true);
#endif

    AfxInitRichEdit();

    prefs.open();

    initLibs();

    mainFrame = new MainFrame();
    m_pMainWnd = mainFrame;

    mainFrame->LoadFrame(IDR_MAINFRAME);
    mainFrame->MoveWindow(CRect(prefs.win_rect));
    mainFrame->ShowWindow(m_nCmdShow);
    mainFrame->UpdateWindow();

    if (prefs.win_maximized) mainFrame->ShowWindow(SW_SHOWMAXIMIZED);

    return TRUE;
}

int BlitzIDE::ExitInstance()
{
    prefs.close();

    return 0;
}
