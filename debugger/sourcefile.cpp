
#include "sourcefile.h"
#include "../blitzide/prefs.h"

#include <istream>

IMPLEMENT_DYNAMIC(SourceFile, CRichEditCtrl)

BEGIN_MESSAGE_MAP(SourceFile, CRichEditCtrl)
                    ON_WM_CREATE()
END_MESSAGE_MAP()

SourceFile::SourceFile() {
}

SourceFile::~SourceFile() {
}

int SourceFile::OnCreate(const LPCREATESTRUCT lpCreateStruct) {
    CRichEditCtrl::OnCreate(lpCreateStruct);

    SetReadOnly(true);
    SetFont(&prefs.editFont);
    SetBackgroundColor(false, prefs.rgb_bkgrnd);

    CHARFORMAT fmt = {sizeof(fmt)};
    fmt.dwMask = CFM_COLOR;
    fmt.crTextColor = prefs.rgb_default;

    SetSel(0, -1);
    SetDefaultCharFormat(fmt);
    SetSelectionCharFormat(fmt);
    SetSel(0, 0);

    return 0;
}

void SourceFile::highLight(const int row, const int col) {
    const int pos = LineIndex(row) + col;
    HideSelection(true, false);
    bool quote = false;
    int end = pos, len = GetTextLength();
    while (end < len) {
        char temp[8];
        SetSel(end, end + 1);
        GetSelText(temp);
        if (temp[0] == '\"') quote = !quote;
        if (!quote && (temp[0] == ':' || !isprint(temp[0]))) break;
        ++end;
    }
    HideSelection(false, false);
    SetSel(pos, end);
}

void SourceFile::formatStreamLine() {
    is_line += "\\line ";
}

DWORD SourceFile::streamIn(const LPBYTE buff, const LONG cnt, LONG *done) {
    int n = 0;
    while (n < cnt) {
        if (is_curs == is_line.size()) {
            if (is_stream->peek() == EOF) break;
            is_curs = 0;
            is_line = "";
            int c = 0;
            for (;;) {
                c = is_stream->get();
                if (c == '\r' || c == '\n' || c == EOF) break;
                if (c == '\\' || c == '{' || c == '}') is_line += '\\';
                is_line += (char) c;
            }
            formatStreamLine();
            ++is_linenum;
            if (c == '\r' && is_stream->peek() == '\n') is_stream->get();
            if (is_stream->peek() == EOF) is_line += '}';
        }
        int sz = is_line.size() - is_curs;
        if (n + sz > cnt) sz = cnt - n;
        memcpy(buff + n, is_line.data() + is_curs, sz);
        is_curs += sz;
        n += sz;
    }
    *done = n;
    return 0;
}

static std::string rtfbgr(const int bgr) {
    return "\\red" + itoa(bgr & 0xff) + "\\green" + itoa((bgr >> 8) & 0xff) + "\\blue" + itoa((bgr >> 16) & 0xff) + ';';
}

void SourceFile::setText(std::istream &in) {
    EDITSTREAM es;
    es.dwCookie = (DWORD) this;
    es.dwError = 0;
    es.pfnCallback = [](const DWORD_PTR cookie, const LPBYTE buff, const LONG cnt, LONG *done) {
        return reinterpret_cast<SourceFile*>(cookie)->streamIn(buff, cnt, done);
    };
    is_line = "{\\rtf1{\\colortbl;" + rtfbgr(prefs.rgb_string) + rtfbgr(prefs.rgb_ident) +
              rtfbgr(prefs.rgb_keyword) + rtfbgr(prefs.rgb_comment) + rtfbgr(prefs.rgb_digit) +
              rtfbgr(prefs.rgb_default) + "}";
    const int tabTwips = 1440 * 8 / GetDeviceCaps(::GetDC(nullptr), LOGPIXELSX) * prefs.edit_tabs;
    for (int k = 0; k < MAX_TAB_STOPS; ++k) is_line += "\\tx" + itoa(k * tabTwips) + ' ';
    is_stream = &in;
    is_curs = is_linenum = 0;
    this->StreamIn((CP_UTF8 << 16) | SF_USECODEPAGE | SF_RTF, es);
}
