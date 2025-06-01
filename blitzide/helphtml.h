#ifndef HELPHTML_H
#define HELPHTML_H

#include "stdafx.h"

class HelpHtml;

class HelpListener
{
public:
    virtual void helpOpen(HelpHtml* help, const std::string& file) = 0;

    virtual void helpTitleChange(HelpHtml* help, const std::string& title) = 0;
};

class HelpHtml : public CHtmlView
{
public:
    HelpHtml(HelpListener* l) : listener(l)
    {
    }

    std::string getTitle();

    DECLARE_DYNAMIC(HelpHtml)

    DECLARE_MESSAGE_MAP()

    afx_msg BOOL OnEraseBkgnd(CDC* dc);

private:
    void OnTitleChange(LPCTSTR t) override;

    void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData,
                           LPCTSTR lpszHeaders, BOOL* pbCancel) override;

    std::string title;
    HelpListener* listener;
};

#endif
