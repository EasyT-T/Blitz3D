#include "libs.h"
#include "prefs.h"
#include "resource.h"
#include "stdafx.h"

#include <mmsystem.h>

char _credits[] =
    "\r\n"
    "Programming and design: Mark Sibly\r\n\r\n"
    "Documentation: Mark Sibly, Simon Harrison, Paul Gerfen, Shane Monroe and the Blitz Doc Team\r\n\r\n"
    "Testing and support: James Boyd, Simon Armstrong and the Blitz Dev Team\r\n\r\n"
    "FreeImage Image loader courtesy of Floris van den berg\r\n\r\n";

class Dialog : public CDialog
{
    bool _quit;

public:
    Dialog() : _quit(false)
    {
    }

    afx_msg void OnOK() override
    {
        _quit = true;
    }

    void wait()
    {
        _quit = false;
        MSG msg;
        while (!_quit && GetMessage(&msg, nullptr, 0, 0))
        {
            if (!AfxGetApp()->PreTranslateMessage(&msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        EndDialog(0);
    }

    void wait(const int n)
    {
        const int _expire = (int)timeGetTime() + n;
        for (;;)
        {
            int tm = _expire - (int)timeGetTime();
            if (tm < 0) tm = 0;
            MsgWaitForMultipleObjects(0, nullptr, false, tm, QS_ALLEVENTS);

            MSG msg;
            if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
            {
                if (!AfxGetApp()->PreTranslateMessage(&msg))
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
            }
            if (!tm) return;
        }
    }
};

void aboutBlitz(bool delay)
{
    AfxGetMainWnd()->EnableWindow(0);

    Dialog about;

    about.Create(IDD_ABOUT);

    int bcc_ver = compiler_ver & 0xffff;
    int ide_ver = VERSION & 0xffff;
    int lnk_ver = linker_ver & 0xffff;
    int run_ver = runtime_ver & 0xffff;
    std::string bcc_v = itoa(ide_ver / 1000) + "." + itoa(ide_ver % 1000);
    std::string ide_v = itoa(ide_ver / 1000) + "." + itoa(ide_ver % 1000);
    std::string lnk_v = itoa(lnk_ver / 1000) + "." + itoa(lnk_ver % 1000);
    std::string run_v = itoa(run_ver / 1000) + "." + itoa(run_ver % 1000);

    std::string credits = _credits;

    about.GetDlgItem(IDC_CREDITS)->SetWindowText(credits.c_str());

    std::string t = "Blitz3D IDE V" + ide_v;
    about.GetDlgItem(IDC_PRODUCT)->SetWindowText(t.c_str());

    t = "Compiler V" + bcc_v + " Linker V" + lnk_v;
    about.GetDlgItem(IDC_PRODUCT2)->SetWindowText(t.c_str());

    t = "Runtime V" + run_v;
    about.GetDlgItem(IDC_VERSION)->SetWindowText(t.c_str());

    about.wait();
    about.EndDialog(0);
    AfxGetMainWnd()->EnableWindow(1);
}
