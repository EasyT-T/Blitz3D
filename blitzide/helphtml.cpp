#include "helphtml.h"
#include "libs.h"
#include "mainframe.h"

IMPLEMENT_DYNAMIC(HelpHtml, CHtmlView)

BEGIN_MESSAGE_MAP(HelpHtml, CHtmlView)
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

std::string HelpHtml::getTitle()
{
    return title;
}

void HelpHtml::OnTitleChange(const LPCTSTR t)
{
    listener->helpTitleChange(this, title = t);
}

void HelpHtml::OnBeforeNavigate2(const LPCTSTR url, DWORD flags, LPCTSTR target, CByteArray& posted, LPCTSTR headers,
                                 BOOL* cancel)
{
    const std::string t(url);
    int attr = GetFileAttributes(url);
    if (attr == -1) attr = 0;
    if ((attr & FILE_ATTRIBUTE_DIRECTORY) ||
        (t.rfind(".bb") + 3 == t.size()) ||
        (isMediaFile(t)))
    {
        listener->helpOpen(this, t);
        *cancel = true;
        return;
    }
    *cancel = false;
}

BOOL HelpHtml::OnEraseBkgnd(CDC* dc)
{
    return true;
}
