#include "funclist.h"

IMPLEMENT_DYNAMIC(FuncList, CListCtrl)

BEGIN_MESSAGE_MAP(FuncList, CListCtrl)
    ON_WM_CREATE()
    ON_NOTIFY_REFLECT(NM_CLICK, &FuncList::nm_click)
END_MESSAGE_MAP()

FuncList::FuncList() :
    listener(nullptr)
{
}

void FuncList::setListener(FuncListListener* l)
{
    listener = l;
}

int FuncList::OnCreate(const LPCREATESTRUCT cs)
{
    CListCtrl::OnCreate(cs);

    return 0;
}

void FuncList::nm_click(NMHDR* nmhdr, LRESULT* result)
{
    const NMLISTVIEW* lv = (NMLISTVIEW*)nmhdr;

    int k = 0;
    Funcs::iterator it;
    for (it = funcs.begin(); it != funcs.end() && k < lv->iItem; ++k, ++it)
    {
    }
    if (it != funcs.end())
    {
        listener->funcSelected(*it);
    }
    *result = 0;
}

void FuncList::clear()
{
    funcs.clear();
    DeleteAllItems();
}

void FuncList::insert(const int line, const std::string& func)
{
    int n = 0;
    Funcs::iterator it;
    for (it = funcs.begin(); it != funcs.end() && line > *it; ++n, ++it)
    {
    }
    if (it != funcs.end() && line == *it)
    {
        SetItemText(n, 0, func.c_str());
        return;
    }
    it = funcs.insert(it, line);
    InsertItem(n, func.c_str());
}

void FuncList::remove(const int begin, const int end)
{
    int n = 0;
    Funcs::iterator it;
    for (it = funcs.begin(); it != funcs.end() && *it < begin; ++n, ++it)
    {
    }
    while (it != funcs.end() && *it < end)
    {
        funcs.erase(it++);
        DeleteItem(n);
    }
}

void FuncList::relocate(const int begin, const int offset)
{
    for (Funcs::iterator it = funcs.begin(); it != funcs.end(); ++it)
    {
        if (*it >= begin) *it += offset;
    }
}
