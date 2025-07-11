#ifndef GXRUNTIME_B3D_H
#define GXRUNTIME_B3D_H

#include <string>
#include <vector>
#include <windows.h>

#include "gxfilesystem.h"
#include "gxgraphics.h"
#include "gxinput.h"
#include "gxtimer.h"

#include "../debugger/debugger.h"

class gxRuntime
{
    /***** INTERNAL INTERFACE *****/
public:
    HWND hwnd;
    HINSTANCE hinst;

    gxInput* input;
    gxGraphics* graphics;
    gxFileSystem* fileSystem;

    void flip(bool vwait);
    void moveMouse(int x, int y) const;

    LRESULT windowProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l);

    struct GfxMode;
    struct GfxDriver;

private:
    gxRuntime(HINSTANCE hinst, const std::string& cmd_line, HWND hwnd);
    ~gxRuntime();

    void paint() const;
    void suspend();
    void forceSuspend();
    void resume();
    void forceResume();
    void backupWindowState();
    void restoreWindowState() const;

    RECT t_rect;
    int t_style;
    std::string cmd_line;
    bool pointer_visible;
    std::string app_title;
    std::string app_close;

    bool setDisplayMode(int w, int h, int d, bool d3d, IDirectDraw7* dd) const;
    gxGraphics* openWindowedGraphics(int w, int h, int d, bool d3d);
    gxGraphics* openExclusiveGraphics(int w, int h, int d, bool d3d);

    bool enum_all;
    std::vector<GfxDriver*> drivers;
    GfxDriver* curr_driver;
    int use_di;

    void enumGfx();
    void denumGfx();

    void resetInput() const;
    void pauseAudio();
    void resumeAudio();
    void backupGraphics() const;
    void restoreGraphics() const;
    void acquireInput();
    void unacquireInput() const;

    /***** APP INTERFACE *****/
public:
    static gxRuntime* openRuntime(HINSTANCE hinst, const std::string& cmd_line, Debugger* debugger);
    static void closeRuntime(gxRuntime* runtime);

    void asyncStop() const;
    void asyncRun() const;
    void asyncEnd() const;

    /***** GX INTERFACE *****/
public:
    enum
    {
        GFXMODECAPS_3D = 1
    };

    //return true if program should continue, or false for quit.
    bool idle();
    bool delay(int ms);

    bool execute(const std::string& cmd);
    void setTitle(const std::string& title, const std::string& close);
    int getMilliSecs();
    void setPointerVisible(bool vis);

    std::string commandLine();

    std::string systemProperty(const std::string& t);

    void debugStop();
    void debugStmt(int pos, const char* file);
    void debugEnter(void* frame, void* env, const char* func);
    void debugLeave();
    void debugInfo(const char* t);
    void debugError(const char* t);
    void debugLog(const char* t);

    int numGraphicsDrivers();
    void graphicsDriverInfo(int driver, std::string* name, int* caps) const;

    int numGraphicsModes(int driver) const;
    void graphicsModeInfo(int driver, int mode, int* w, int* h, int* d, int* caps) const;

    void windowedModeInfo(int* caps) const;

    gxInput* openInput(int flags);
    void closeInput(gxInput* input);

    gxGraphics* openGraphics(int w, int h, int d, int driver, int flags);
    void closeGraphics(gxGraphics* graphics);
    bool graphicsLost();

    gxFileSystem* openFileSystem(int flags);
    void closeFileSystem(gxFileSystem* filesys);

    gxTimer* createTimer(int hertz);
    void freeTimer(gxTimer* timer);

    void enableDirectInput(bool use);
    int directInputEnabled() const { return use_di; }

    int callDll(const std::string& dll, const std::string& func, const void* in, int in_sz, void* out, int out_sz);

    OSVERSIONINFO osinfo;
};

#endif
