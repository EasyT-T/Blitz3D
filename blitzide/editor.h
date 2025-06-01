
#ifndef EDITOR_H
#define EDITOR_H

#include "funclist.h"
#include "stdafx.h"
#include "tabber.h"

#include <tom.h>

class Editor;

class EditorListener {
public:
    virtual void cursorMoved(Editor *e) {}
};

class Editor : public CWnd, FuncListListener, TabberListener {
public:
    Editor(EditorListener *l);

    ~Editor() override;

    void setName(const std::string &f);

    void setModified(bool n);

    bool setText(std::istream &in);

    void setCursor(int pos);

    void cut();

    void copy();

    void paste();

    void find();

    void replace();

    bool canFind() const;

    bool findNext(bool wrap);

    void print();

    void hilight(int pos);

    void selectAll();

    static void lock();

    static void unlock();

    int getCursor() const;

    std::string getName() const;

    std::string getKeyword();

    bool getModified() const;

    bool getText(std::ostream &out);

    void getCursor(int *row, int *col) const;

    bool canCutCopy();

    bool canPaste() const;

    static void addKeyword(const std::string &s);

DECLARE_DYNAMIC(Editor)

DECLARE_MESSAGE_MAP()

    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

    afx_msg void OnSize(UINT type, int w, int h);

    afx_msg void OnSetFocus(CWnd *wnd);

    afx_msg void OnKillFocus(CWnd *wnd);

    afx_msg void OnPaint();

    afx_msg void OnMouseMove(UINT flags, CPoint p);

    afx_msg void OnLButtonDown(UINT flags, CPoint p);

    afx_msg void OnLButtonUp(UINT flags, CPoint p);

    afx_msg void en_change();

    afx_msg void en_update();

    afx_msg void en_selchange(NMHDR *nmhdr, LRESULT *result);

    afx_msg void en_protected(NMHDR *nmhdr, LRESULT *result);

    afx_msg void en_msgfilter(NMHDR *nmhdr, LRESULT *result);

    afx_msg LRESULT onFind(WPARAM w, LPARAM l);

private:
    EditorListener *listener;

    std::string name;
    bool findOnly, found;
    CFindReplaceDialog *finder;
    char findBuff[256], replaceBuff[256];
    long selStart, selEnd;
    int findFlags, d_c;

    //formatting
    bool fmtBusy;
    int lineToFmt, fmtLineCount;

    //streaming
    std::string is_line;
    std::istream *is_stream;
    int is_curs, is_linenum;

    //sizing
    bool sizing;
    CPoint point;
    int tabber_width;

    //tabber
    Tabber tabber;
    FuncList funcList, typeList, labsList;
    CRichEditCtrl editCtrl;
    ITextDocument *tomDoc;

    void funcSelected(int line) override;

    void currentSet(Tabber *tabber, int index) override;

    void resized();

    void getSel();

    void setSel();

    void suspendUndo() const;

    void resumeUndo() const;

    void endFind();

    void fixFmt(bool fmt);

    void setFormat(int from, int to, int color, const std::string &s = "");

    void formatLine(int line);

    void caret();

    void cursorMoved();

    std::string getLine(int line) const;

    void formatStreamLine();

    DWORD streamIn(LPBYTE buff, LONG cnt, LONG *done);

    static DWORD CALLBACK streamIn(DWORD cookie, LPBYTE buff, LONG cnt, LONG *done);

    static DWORD CALLBACK streamOut(DWORD cookie, LPBYTE buff, LONG cnt, LONG *done);
};

#endif
