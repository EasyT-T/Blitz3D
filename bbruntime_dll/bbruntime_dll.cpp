#pragma warning( disable:4786 )

#include "bbruntime_dll.h"
#include "../debugger/debugger.h"

#include <eh.h>
#include <float.h>
#include <map>

#include "../bbruntime/bbruntime.h"

class DummyDebugger : public Debugger
{
public:
    void debugRun() override
    {
    }

    void debugStop() override
    {
    } // bbruntime_panic(0); }
    void debugStmt(int srcpos, const char* file) override
    {
    }

    void debugEnter(void* frame, void* env, const char* func) override
    {
    }

    void debugLeave() override
    {
    }

    void debugLog(const char* msg) override
    {
    }

    void debugMsg(const char* e, const bool serious) override
    {
        if (serious)
            MessageBox(nullptr, e, "Error!",MB_OK | MB_TOPMOST | MB_SETFOREGROUND);
    }

    void debugSys(void* msg) override
    {
    }
};

static HINSTANCE hinst;
static std::map<const char*, void*> syms;
std::map<const char*, void*>::iterator sym_it;
static gxRuntime* gx_runtime;

static void rtSym(const char* sym, void* pc)
{
    syms[sym] = pc;
}

static void _cdecl seTranslator(const unsigned int u, EXCEPTION_POINTERS* pExp)
{
    switch (u)
    {
    case EXCEPTION_INT_DIVIDE_BY_ZERO:
        bbruntime_panic("Integer divide by zero");
    case EXCEPTION_ACCESS_VIOLATION:
        bbruntime_panic("Memory access violation");
    case EXCEPTION_ILLEGAL_INSTRUCTION:
        bbruntime_panic("Illegal instruction");
    case EXCEPTION_STACK_OVERFLOW:
        bbruntime_panic("Stack overflow!");
    }
    bbruntime_panic("Unknown runtime exception");
}

int Runtime::version()
{
    return VERSION;
}

const char* Runtime::nextSym()
{
    if (!syms.size())
    {
        bbruntime_link(rtSym);
        sym_it = syms.begin();
    }
    if (sym_it == syms.end())
    {
        syms.clear();
        return nullptr;
    }
    return (sym_it++)->first;
}

int Runtime::symValue(const char* sym)
{
    const std::map<const char*, void*>::iterator it = syms.find(sym);
    if (it != syms.end()) return (int)it->second;
    return -1;
}

void Runtime::startup(const HINSTANCE h)
{
    hinst = h;
}

void Runtime::shutdown()
{
    trackmem(false);
    syms.clear();
}

void Runtime::execute(void (*pc)(), const char* args, Debugger* dbg)
{
    const bool debug = !!dbg;

    static DummyDebugger dummydebug;

    if (!dbg) dbg = &dummydebug;

    trackmem(true);

    const _se_translator_function old_trans = _set_se_translator(seTranslator);
    _control87(
        _RC_NEAR | _PC_24 | _EM_INVALID | _EM_ZERODIVIDE | _EM_OVERFLOW | _EM_UNDERFLOW | _EM_INEXACT | _EM_DENORMAL,
        0xfffff);

    //strip spaces from ends of args...
    std::string params = args;
    while (params.size() && params[0] == ' ') params = params.substr(1);
    while (params.size() && params[params.size() - 1] == ' ') params = params.substr(0, params.size() - 1);

    if (gx_runtime = gxRuntime::openRuntime(hinst, params, dbg))
    {
        bbruntime_run(gx_runtime, pc, debug);

        gxRuntime* t = gx_runtime;
        gx_runtime = nullptr;
        gxRuntime::closeRuntime(t);
    }

    _control87(_CW_DEFAULT, 0xfffff);
    _set_se_translator(old_trans);
}

void Runtime::asyncStop()
{
    if (gx_runtime) gx_runtime->asyncStop();
}

void Runtime::asyncRun()
{
    if (gx_runtime) gx_runtime->asyncRun();
}

void Runtime::asyncEnd()
{
    if (gx_runtime) gx_runtime->asyncEnd();
}

void Runtime::checkmem(std::streambuf* buf)
{
    std::ostream out(buf);
    ::checkmem(out);
}

Runtime*_cdecl runtimeGetRuntime()
{
    static Runtime runtime;
    return &runtime;
}

/********************** BUTT UGLY DLL->EXE HOOK! *************************/

static void* module_pc;
static std::map<std::string, int> module_syms;
static std::map<std::string, int> runtime_syms;
static Runtime* runtime;

static void fail()
{
    MessageBox(nullptr, "Unable to run Blitz Basic module", nullptr, 0);
    ExitProcess(-1);
}

struct Sym
{
    std::string name;
    int value;
};

static Sym getSym(void** p)
{
    Sym sym;
    char* t = static_cast<char*>(*p);
    while (const char c = *t++) sym.name += c;
    sym.value = *(int*)t + (int)module_pc;
    *p = t + 4;
    return sym;
}

static int findSym(const std::string& t)
{
    std::map<std::string, int>::iterator it = module_syms.find(t);
    if (it != module_syms.end()) return it->second;
    it = runtime_syms.find(t);
    if (it != runtime_syms.end()) return it->second;

    const std::string err = "Can't find symbol: " + t;
    MessageBox(nullptr, err.c_str(), nullptr, 0);
    ExitProcess(0);
    return 0;
}

static void link()
{
    while (const char* sc = runtime->nextSym())
    {
        std::string t(sc);

        if (t[0] == '_')
        {
            runtime_syms["_" + t] = runtime->symValue(sc);
            continue;
        }

        if (t[0] == '!') t = t.substr(1);

        if (!isalnum(t[0])) t = t.substr(1);

        for (int k = 0; k < t.size(); ++k)
        {
            if (isalnum(t[k]) || t[k] == '_') continue;
            t = t.substr(0, k);
            break;
        }

        runtime_syms["_f" + tolower(t)] = runtime->symValue(sc);
    }

    const HRSRC hres = FindResource(nullptr,MAKEINTRESOURCE(1111),RT_RCDATA);
    if (!hres) fail();
    const HGLOBAL hglo = LoadResource(nullptr, hres);
    if (!hglo) fail();
    void* p = LockResource(hglo);
    if (!p) fail();

    const int sz = *static_cast<int*>(p);
    p = static_cast<int*>(p) + 1;

    //replace malloc for service pack 2 Data Execution Prevention (DEP).
    module_pc = VirtualAlloc(nullptr, sz,MEM_COMMIT | MEM_RESERVE,PAGE_EXECUTE_READWRITE);

    memcpy(module_pc, p, sz);
    p = static_cast<char*>(p) + sz;

    int k;

    int cnt = *static_cast<int*>(p);
    p = static_cast<int*>(p) + 1;
    for (k = 0; k < cnt; ++k)
    {
        Sym sym = getSym(&p);
        if (sym.value < (int)module_pc || sym.value >= (int)module_pc + sz) fail();
        module_syms[sym.name] = sym.value;
    }

    cnt = *static_cast<int*>(p);
    p = static_cast<int*>(p) + 1;
    for (k = 0; k < cnt; ++k)
    {
        Sym sym = getSym(&p);
        int* pp = (int*)sym.value;
        const int dest = findSym(sym.name);
        *pp += dest - (int)pp;
    }

    cnt = *static_cast<int*>(p);
    p = static_cast<int*>(p) + 1;
    for (k = 0; k < cnt; ++k)
    {
        Sym sym = getSym(&p);
        int* pp = (int*)sym.value;
        const int dest = findSym(sym.name);
        *pp += dest;
    }

    runtime_syms.clear();
    module_syms.clear();
}

extern "C" _declspec(dllexport) int _stdcall bbWinMain();
extern "C" BOOL _stdcall _DllMainCRTStartup(HANDLE, DWORD, LPVOID);

bool WINAPI DllMain(HANDLE module, DWORD reason, void* reserved)
{
    return TRUE;
}

int __stdcall bbWinMain()
{
    const HINSTANCE inst = GetModuleHandle(nullptr);

    _DllMainCRTStartup(inst,DLL_PROCESS_ATTACH, nullptr);

#ifdef BETA
	int ver=VERSION & 0x7fff;
	string t="Created with Blitz3D Beta V"+itoa( ver/100 )+"."+itoa( ver%100 );
	MessageBox( GetDesktopWindow(),t.c_str(),"Blitz3D Message",MB_OK );
#endif

#ifdef SCHOOLS
	MessageBox( GetDesktopWindow(),"Created with the schools version of Blitz Basic","Blitz Basic Message",MB_OK );
#endif

    runtime = runtimeGetRuntime();
    runtime->startup(inst);

    link();

    //get cmd_line and params
    std::string cmd = GetCommandLine(), params;
    while (cmd.size() && cmd[0] == ' ') cmd = cmd.substr(1);
    if (cmd.find('\"') == 0)
    {
        const int n = cmd.find('\"', 1);
        if (n != std::string::npos)
        {
            params = cmd.substr(n + 1);
            cmd = cmd.substr(1, n - 1);
        }
    }
    else
    {
        const int n = cmd.find(' ');
        if (n != std::string::npos)
        {
            params = cmd.substr(n + 1);
            cmd = cmd.substr(0, n);
        }
    }

    runtime->execute(static_cast<void(*)()>(module_pc), params.c_str(), nullptr);
    runtime->shutdown();

    _DllMainCRTStartup(inst,DLL_PROCESS_DETACH, nullptr);

    ExitProcess(0);
    return 0;
}
