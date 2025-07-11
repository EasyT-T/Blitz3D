#include "tabber.h"

IMPLEMENT_DYNAMIC(Tabber, CTabCtrl)

BEGIN_MESSAGE_MAP(Tabber, CTabCtrl)
    ON_WM_SIZE()
    ON_WM_ERASEBKGND()
    ON_NOTIFY_REFLECT(TCN_SELCHANGE, &Tabber::tcn_selChange)
END_MESSAGE_MAP()

static CRect tabsRect(CTabCtrl& t)
{
    CRect r(0, 0, 0, 0);
    const int n = t.GetItemCount();
    for (int k = 0; k < n; ++k)
    {
        CRect c;
        t.GetItemRect(k, &c);
        if (c.left < r.left) r.left = c.left;
        if (c.right > r.right) r.right = c.right;
        if (c.top < r.top) r.top = c.top;
        if (c.bottom > r.bottom) r.bottom = c.bottom;
    }
    return r;
}

CRect Tabber::getInnerRect()
{
    CRect r;
    GetClientRect(&r);
    int x = 2, y = 2, w = r.Width() - 4, h = r.Height() - 4;

    r = tabsRect(*this);
    h -= r.Height();
    y += r.Height();

    r.left = x;
    r.top = y;
    r.right = x + w;
    r.bottom = y + h;
    return r;
}

Tabber::Tabber() :
    listener(nullptr), curr(-1)
{
}

Tabber::~Tabber()
{
    for (; tabs.size(); tabs.pop_back()) delete tabs.back();
}

void Tabber::OnSize(const UINT type, const int w, const int h)
{
    CTabCtrl::OnSize(type, w, h);
    refresh();
}

BOOL Tabber::OnEraseBkgnd(CDC* dc)
{
    CRect c;
    GetClientRect(&c);

    const HBRUSH hb = (HBRUSH)GetClassLong(m_hWnd, GCL_HBRBACKGROUND);
    CBrush br;
    br.Attach(hb);

    if (curr < 0) dc->FillRect(&c, &br);
    else
    {
        const CRect i = getInnerRect();
        const CRect t(c.left, c.top, i.right, i.top);
        dc->FillRect(&t, &br);
        const CRect r(i.right, c.top, c.right, i.bottom);
        dc->FillRect(&r, &br);
        const CRect b(i.left, i.bottom, c.right, c.bottom);
        dc->FillRect(&b, &br);
        const CRect l(c.left, i.top, i.left, c.bottom);
        dc->FillRect(&l, &br);
    }
    return true;
}

void Tabber::setListener(TabberListener* l)
{
    listener = l;
}

void Tabber::refresh()
{
    if (curr < 0) return;

    const CRect r = getInnerRect();
    CWnd* wnd = getTabWnd(curr);
    wnd->MoveWindow(r.left, r.top, r.Width(), r.Height());
    wnd->ShowWindow(SW_SHOW);
    wnd->SetFocus();
}

Tabber::Tab* Tabber::getTab(int index) const
{
    if (index < 0 || index >= tabs.size()) return nullptr;
    Tabs::const_iterator it = tabs.begin();
    while (index--) ++it;
    return *it;
}

void Tabber::tcn_selChange(NMHDR* p, LRESULT* result)
{
    setCurrent(GetCurSel());
}

void Tabber::insert(const int index, CWnd* w, const std::string& t)
{
    if (index < 0 || index > tabs.size()) return;

    Tabs::iterator it = tabs.begin();
    for (int k = 0; k < index; ++k) ++it;
    Tab* tab = new Tab(w, t);
    tabs.insert(it, tab);

    InsertItem(index, t.c_str());
    if (curr < 0) setCurrent(index);
}

void Tabber::remove(const int index)
{
    if (index < 0 || index >= tabs.size()) return;

    CWnd* w = getTabWnd(index);

    Tabs::iterator it = tabs.begin();
    for (int k = 0; k < index; ++k) ++it;
    delete *it;
    tabs.erase(it);
    DeleteItem(index);

    if (curr >= tabs.size()) curr = tabs.size() - 1;

    refresh();
    if (curr >= 0) SetCurSel(curr);
    if (w) w->ShowWindow(SW_HIDE);
    if (listener) listener->currentSet(this, curr);
}

void Tabber::setCurrent(const int index)
{
    if (index < 0 || index >= tabs.size()) return;

    if (index != curr)
    {
        CWnd* w = getTabWnd(curr);
        curr = index;

        refresh();
        SetCurSel(curr);
        if (w) w->ShowWindow(SW_HIDE);
    }

    if (listener) listener->currentSet(this, curr);
}

void Tabber::setTabText(const int index, const std::string& t)
{
    if (index < 0 || index >= tabs.size()) return;

    std::string s = t + '\0';
    TCITEM tc = {TCIF_TEXT};
    tc.pszText = (char*)s.data();
    SetItem(index, &tc);
}

int Tabber::size() const
{
    return tabs.size();
}

int Tabber::getCurrent() const
{
    return curr;
}

CWnd* Tabber::getTabWnd(const int index) const
{
    const Tab* t = getTab(index);
    return t ? t->wnd : nullptr;
}

std::string Tabber::getTabText(const int index) const
{
    Tab* t = getTab(index);
    return t ? t->text : "";
}
