
#include "libs.h"
#include "blitzide.h"
#include "editor.h"

static std::map<std::string, std::string> keyhelps;

int compiler_ver, linker_ver, runtime_ver;

static std::string execProc(const std::string &proc) {
    HANDLE rd, wr;

    SECURITY_ATTRIBUTES sa = {sizeof(sa), nullptr, true};

    if (CreatePipe(&rd, &wr, &sa, 0)) {
        STARTUPINFO si = {sizeof(si)};
        si.dwFlags = STARTF_USESTDHANDLES;
        si.hStdOutput = si.hStdError = wr;
        PROCESS_INFORMATION pi = {nullptr};
        if (CreateProcess(nullptr, (char *) proc.c_str(), nullptr, nullptr, true, DETACHED_PROCESS, nullptr, nullptr, &si, &pi)) {
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
            CloseHandle(wr);

            std::string t;
            char *buf = new char[1024];
            for (;;) {
                unsigned long sz;
                const int n = ReadFile(rd, buf, 1024, &sz, nullptr);
                if (!n && GetLastError() == ERROR_BROKEN_PIPE) break;
                if (!n) {
                    t = "";
                    break;
                }
                if (!sz) break;
                t += std::string(buf, sz);
            }
            delete[] buf;
            CloseHandle(rd);
            return t;
        }
        CloseHandle(rd);
        CloseHandle(wr);
    }
    AfxMessageBox((proc + " failed").c_str());
    ExitProcess(0);
    return "";
}

int version(std::string vers, std::string t) {
    t += " version:";
    int n = vers.find(t);
    n += t.size();
    const int maj = atoi(vers.substr(n));
    n = vers.find('.', n) + 1;
    const int min = atoi(vers.substr(n));
    const int v = maj * 100 + min;
    return v;
}

void initLibs() {

    const std::string valid = execProc(prefs.homeDir + "/bin/blitzcc -q");
    if (valid.size()) {
        AfxMessageBox(("Compiler environment error: " + valid).c_str());
        ExitProcess(0);
    }

    const std::string vers = tolower(execProc(prefs.homeDir + "/bin/blitzcc -v"));
    compiler_ver = version(vers, "compiler");
    linker_ver = version(vers, "linker");
    runtime_ver = version(vers, "runtime");

    //generate keywords!
    std::string kws = execProc(prefs.homeDir + "/bin/blitzcc +k");

    if (!kws.size()) {
        AfxMessageBox("Error generating keywords");
        ExitProcess(0);
    }

    int pos = 0, n;
    while ((n = kws.find('\n', pos)) != std::string::npos) {
        std::string t = kws.substr(pos, n - pos - 1);
        for (int q = 0; (q = t.find('\r', q)) != std::string::npos;) t = t.replace(q, 1, "");

        const std::string help = t;
        const int i = t.find(' ');
        if (i != std::string::npos) {
            t = t.substr(0, i);
            if (!t.size()) {
                AfxMessageBox("Error in keywords");
                ExitProcess(0);
            }
            if (!isalnum(t[t.size() - 1])) t = t.substr(0, t.size() - 1);
        }

        Editor::addKeyword(t);
        keyhelps[t] = help;
        pos = n + 1;
    }
}

std::string quickHelp(const std::string &kw) {
    const std::map<std::string, std::string>::const_iterator it = keyhelps.find(kw);
    return it == keyhelps.end() ? "" : it->second;
}

bool isMediaFile(const std::string &f) {
    static const char* exts[] = {
            "bmp", "jpg", "png", "tga", "iff", "pcx",
            "wav", "mid", "mp3", "mod", "s3m", "xm", "it", "rmi", "sgt",
            "x", "3ds", nullptr
    };

    const int i = f.rfind('.');
    if (i == std::string::npos || i + 1 == f.size()) return false;
    const std::string ext = f.substr(i + 1);
    const char **p = exts;
    while (const char *e = *p++) {
        std::string t(e);
        if (i + t.size() + 1 != f.size()) continue;
        if (ext == t) return true;
    }
    return false;
}
