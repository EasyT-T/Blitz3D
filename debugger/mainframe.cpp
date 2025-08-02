#include "mainframe.h"
#include "debuggerapp.h"
#include "resource.h"
#include "stdafx.h"
#include "../blitzide/prefs.h"

#include <fstream>

#define WM_IDLEUPDATECMDUI  0x0363  // wParam == bDisableIfNoHandler

enum
{
    WM_STOP = WM_APP + 1, WM_RUN, WM_END
};

enum
{
    STARTING, RUNNING, STOPPED, ENDING
};

IMPLEMENT_DYNAMIC(MainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(MainFrame, CFrameWnd)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_CLOSE()
    ON_WM_WINDOWPOSCHANGING()

    ON_COMMAND(ID_STOP, &MainFrame::cmdStop)
    ON_COMMAND(ID_RUN, &MainFrame::cmdRun)
    ON_COMMAND(ID_STEPOVER, &MainFrame::cmdStepOver)
    ON_COMMAND(ID_STEPINTO, &MainFrame::cmdStepInto)
    ON_COMMAND(ID_STEPOUT, &MainFrame::cmdStepOut)
    ON_COMMAND(ID_END, &MainFrame::cmdEnd)

    ON_UPDATE_COMMAND_UI(ID_STOP, &MainFrame::updateCmdUI)
    ON_UPDATE_COMMAND_UI(ID_RUN, &MainFrame::updateCmdUI)
    ON_UPDATE_COMMAND_UI(ID_STEPOVER, &MainFrame::updateCmdUI)
    ON_UPDATE_COMMAND_UI(ID_STEPINTO, &MainFrame::updateCmdUI)
    ON_UPDATE_COMMAND_UI(ID_STEPOUT, &MainFrame::updateCmdUI)
    ON_UPDATE_COMMAND_UI(ID_END, &MainFrame::updateCmdUI)

END_MESSAGE_MAP()

MainFrame::MainFrame() : state(STARTING), step_level(-1), cur_pos(0), cur_file(nullptr)
{
}

MainFrame::~MainFrame()
{
    for (std::map<const char*, SourceFile*>::iterator it = files.begin(); it != files.end(); ++it) delete it->second;
}

int MainFrame::OnCreate(const LPCREATESTRUCT lpCreateStruct)
{
    CFrameWnd::OnCreate(lpCreateStruct);

    prefs.open();

    const std::string tb = prefs.homeDir + "/cfg/dbg_toolbar.bmp";

    //Toolbar
    const HBITMAP toolbmp = static_cast<HBITMAP>(LoadImage(
        nullptr, tb.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_LOADMAP3DCOLORS));

    BITMAP bm;
    GetObject(toolbmp, sizeof(bm), &bm);

    int n = 0;
    const UINT toolbuts[] = {ID_STOP, ID_RUN, ID_STEPOVER, ID_STEPINTO, ID_STEPOUT, ID_END};
    const int toolcnt = sizeof(toolbuts) / sizeof(UINT);
    for (int k = 0; k < toolcnt; ++k) if (toolbuts[k] != ID_SEPARATOR) ++n;

    SIZE imgsz, butsz;
    imgsz.cx = bm.bmWidth / n;
    imgsz.cy = bm.bmHeight;
    butsz.cx = imgsz.cx + 7;
    butsz.cy = imgsz.cy + 6;

    toolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS);
    toolBar.SetBitmap(toolbmp);
    toolBar.SetSizes(butsz, imgsz);
    toolBar.SetButtons(toolbuts, toolcnt);

    //Tabber
    tabber.Create(
        WS_VISIBLE | WS_CHILD |
        TCS_HOTTRACK,
        CRect(0, 0, 0, 0), this, 1);
    tabber.SetFont(&prefs.tabsFont);

    //Second tabber
    tabber2.Create(
        WS_VISIBLE | WS_CHILD |
        TCS_HOTTRACK,
        CRect(0, 0, 0, 0), this, 2);
    tabber2.SetFont(&prefs.tabsFont);

    //Debug Log
    debug_log.Create(
        WS_CHILD | WS_HSCROLL | WS_VSCROLL |
        ES_NOHIDESEL | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
        CRect(0, 0, 0, 0), &tabber, 1);
    tabber.insert(0, &debug_log, "Debug log");

    //Debug trees
    locals_tree.Create(
        WS_VISIBLE | WS_CHILD |
        TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS,
        CRect(0, 0, 0, 0), &tabber2, 3);

    globals_tree.Create(
        WS_VISIBLE | WS_CHILD |
        TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS,
        CRect(0, 0, 0, 0), &tabber2, 3);

    consts_tree.Create(
        WS_VISIBLE | WS_CHILD |
        TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS,
        CRect(0, 0, 0, 0), &tabber2, 3);

    tabber2.insert(0, &locals_tree, "Locals");
    tabber2.insert(1, &globals_tree, "Globals");
    tabber2.insert(2, &consts_tree, "Consts");
    tabber2.setCurrent(0);

    setState(STARTING);

    return 0;
}

void MainFrame::setState(const int n)
{
    state = n;
    SendMessageToDescendants(WM_IDLEUPDATECMDUI, (WPARAM)TRUE, 0, TRUE, TRUE);
    if (shouldRun())
    {
        if (const HWND app = ::FindWindow("Blitz Runtime Class", nullptr))
        {
            ::SetActiveWindow(app);
        }
    }
    else
    {
        SetActiveWindow();
    }
}

void MainFrame::OnClose()
{
    cmdEnd();
}

void MainFrame::OnSize(const UINT type, const int sw, const int sh)
{
    CFrameWnd::OnSize(type, sw, sh);

    CRect r, t;
    GetClientRect(&r);
    int x = r.left, y = r.top, w = r.Width(), h = r.Height();

    toolBar.GetWindowRect(&t);
    y += t.Height();
    h -= t.Height();

    tabber.MoveWindow(x, y, w - 240, h);

    tabber2.MoveWindow(x + w - 240, y, 240, h);
}

void MainFrame::setRuntime(void* mod, void* env)
{
    consts_tree.reset(static_cast<Environ*>(env));
    globals_tree.reset(static_cast<Module*>(mod), static_cast<Environ*>(env));
    locals_tree.reset(static_cast<Environ*>(env));
}

void MainFrame::showCurStmt()
{
    if (!cur_file) return;

    SourceFile* t = sourceFile(cur_file);

    const int row = (cur_pos >> 16) & 0xffff, col = cur_pos & 0xffff;
    t->highLight(row, col);

    globals_tree.refresh();
    locals_tree.refresh();
}

void MainFrame::debugRun()
{
    setState(RUNNING);
}

void MainFrame::debugStop()
{
    step_level = locals_tree.size();
    setState(STOPPED);
    showCurStmt();
}

void MainFrame::debugStmt(const int pos, const char* file)
{
    cur_pos = pos;
    cur_file = file;

    if (shouldRun()) return;

    cmdStop();
}

void MainFrame::debugEnter(void* frame, void* env, const char* func)
{
    locals_tree.pushFrame(frame, env, func);

    if (locals_tree.size() > 1) return;

    globals_tree.refresh();
    locals_tree.refresh();

    setState(RUNNING);
}

void MainFrame::debugLeave()
{
    locals_tree.popFrame();
}

void MainFrame::debugMsg(const char* msg, const bool serious)
{
    if (serious)
    {
        ::MessageBox(nullptr, msg, "Runtime Error", MB_OK | MB_ICONWARNING | MB_TOPMOST | MB_SETFOREGROUND);
    }
    else
    {
        ::MessageBox(nullptr, msg, "Runtime Message", MB_OK | MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
    }
}

void MainFrame::debugLog(const char* msg)
{
    debug_log.ReplaceSel(msg);
    debug_log.ReplaceSel("\n");
    tabber.setCurrent(0);
    setState(state);
}

void MainFrame::debugSys(void* m)
{
}

void MainFrame::cmdStop()
{
    if (const HWND hwnd = ::FindWindow("Blitz Runtime Class", nullptr))
    {
        ::PostMessage(hwnd, WM_STOP, 0, 0);
    }
}

void MainFrame::cmdRun()
{
    if (const HWND hwnd = ::FindWindow("Blitz Runtime Class", nullptr))
    {
        step_level = -1;
        ::PostMessage(hwnd, WM_RUN, 0, 0);
    }
}

void MainFrame::cmdEnd()
{
    if (const HWND hwnd = ::FindWindow("Blitz Runtime Class", nullptr))
    {
        ::PostMessage(hwnd, WM_END, 0, 0);
        setState(ENDING);
    }
}

void MainFrame::cmdStepOver()
{
    if (const HWND hwnd = ::FindWindow("Blitz Runtime Class", nullptr))
    {
        ::PostMessage(hwnd, WM_RUN, 0, 0);
    }
}

void MainFrame::cmdStepInto()
{
    if (const HWND hwnd = ::FindWindow("Blitz Runtime Class", nullptr))
    {
        step_level = locals_tree.size() + 1;
        ::PostMessage(hwnd, WM_RUN, 0, 0);
    }
}

void MainFrame::cmdStepOut()
{
    if (HWND hwnd = ::FindWindow("Blitz Runtime Class", nullptr))
    {
        step_level = locals_tree.size() - 1;
        ::PostMessage(nullptr, WM_RUN, 0, 0);
    }
}

SourceFile* MainFrame::sourceFile(const char* file)
{
    if (!file) file = "<unknown>";

    std::map<const char*, SourceFile*>::const_iterator it = files.find(file);

    if (it != files.end())
    {
        tabber.setCurrent(file_tabs[file]);
        return it->second;
    }

    //crete new source file
    SourceFile* t = new SourceFile();

    it = files.insert(std::make_pair(file, t)).first;

    int tab = files.size();

    t->Create(
        WS_CHILD | WS_HSCROLL | WS_VSCROLL |
        ES_NOHIDESEL | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
        CRect(0, 0, 0, 0), &tabber, 1);

    {
        std::ifstream in(file, std::ios::binary);
        if (in.is_open()) t->setText(in);
    }

    file_tabs.insert(std::make_pair(file, tab));

    if (const char* p = strrchr(file, '/')) file = p + 1;
    if (const char* p = strrchr(file, '\\')) file = p + 1;
    tabber.insert(tab, t, file);

    tabber.setCurrent(tab);

    return t;
}

void MainFrame::updateCmdUI(CCmdUI* ui)
{
    if (state != RUNNING && state != STOPPED)
    {
        ui->Enable(false);
        return;
    }
    switch (ui->m_nID)
    {
    case ID_STOP:
        ui->Enable(shouldRun());
        break;
    case ID_RUN:
    case ID_STEPOVER:
    case ID_STEPINTO:
    case ID_STEPOUT:
        ui->Enable(!shouldRun());
        break;
    case ID_END:
        ui->Enable(true);
        break;
    }
}

void MainFrame::OnWindowPosChanging(WINDOWPOS* pos)
{
    RECT rect;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);

    pos->x = rect.left;
    pos->cx = rect.right - pos->x;
    pos->cy = rect.bottom - pos->y;
}
