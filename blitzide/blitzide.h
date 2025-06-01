
#ifndef BLITZIDE_H
#define BLITZIDE_H

#include "mainframe.h"
#include "prefs.h"

class BlitzIDE : public CWinApp {
public:
    MainFrame *mainFrame;

    BOOL InitInstance() override;

    int ExitInstance() override;
};

extern BlitzIDE blitzIDE;

#endif