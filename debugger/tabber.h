#ifndef TABBER_H
#define TABBER_H

#include "stdafx.h"

class Tabber;

class TabberListener
{
public:
    virtual void currentSet(Tabber* tabber, int index) = 0;
};

class Tabber : public CTabCtrl
{
public:
    Tabber();

    ~Tabber() override;

    void setListener(TabberListener* l);

    void insert(int index, CWnd* wnd, const std::string& text);

    void remove(int index);

    void setCurrent(int index);

    void setTabText(int index, const std::string& t);

    int size() const;

    int getCurrent() const;

    CWnd* getTabWnd(int index) const;

    std::string getTabText(int index) const;

    DECLARE_DYNAMIC(Tabber)

    DECLARE_MESSAGE_MAP()

    afx_msg void OnSize(UINT type, int w, int h);

    afx_msg BOOL OnEraseBkgnd(CDC* dc);

    afx_msg void tcn_selChange(NMHDR* p, LRESULT* result);

private:
    TabberListener* listener;

    struct Tab
    {
        CWnd* wnd;
        std::string text;

        Tab(CWnd* w, const std::string& t) : wnd(w), text(t)
        {
        }
    };

    typedef std::list<Tab*> Tabs;

    Tabs tabs;
    int curr;

    void refresh();

    CRect getInnerRect();

    Tab* getTab(int index) const;
};

#endif
