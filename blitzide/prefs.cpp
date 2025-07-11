#include "prefs.h"

#include <fstream>
#include <iomanip>

#define SWAPRB(x) ( (((x)>>16)&0xff) | ((x)&0xff00) | (((x)&0xff)<<16) )

Prefs prefs;

void Prefs::open()
{
    const char* p = getenv("blitzpath");
    if (!p)
    {
        AfxMessageBox("blitzpath environment variable not found!", MB_TOPMOST | MB_SETFOREGROUND | MB_ICONINFORMATION);
        ExitProcess(0);
    }

    homeDir = p;

    AddFontResource((homeDir + "/cfg/blitz.fon").c_str());

    setDefault();

    bool prg_windowed;

    std::ifstream in((homeDir + "/cfg/blitzide.prefs").c_str());
    if (!in.good()) return;

    while (!in.eof())
    {
        std::string t;
        in >> t;
        if (!t.size()) continue;
        while (in.peek() == '\t') in.ignore();
        if (t == "prg_debug") in >> prg_debug;
        else if (t == "prg_lastbuild") getline(in, prg_lastbuild);
        else if (t == "prg_windowed") in >> prg_windowed;
        else if (t == "win_maximized") in >> win_maximized;
        else if (t == "win_notoolbar") in >> win_notoolbar;
        else if (t == "win_rect")
        {
            in >> win_rect.left;
            in >> win_rect.top;
            in >> win_rect.right;
            in >> win_rect.bottom;
        }
        else if (t.substr(0, 5) == "font_")
        {
            std::string s;
            if (in.peek() == '\"')
            {
                in.ignore();
                while (in.peek() != '\"') s += (char)in.get();
                in.ignore();
            }
            else
            {
                in >> s;
            }
            int h;
            in >> h;
            //AfxMessageBox((s+" "+std::to_string(h)).c_str());
            t = t.substr(5);
            if (t == "editor")
            {
                font_editor = s;
                font_editor_height = h;
            }
            else if (t == "tabs")
            {
                font_tabs = s;
                font_tabs_height = h;
            }
            else if (t == "debug")
            {
                font_debug = s;
                font_debug_height = h;
            }
            else if (t == "window")
            {
                font_window = s;
                font_window_height = h;
            }
        }
        else if (t.substr(0, 4) == "rgb_")
        {
            t = t.substr(4);
            std::string s;
            in >> s;
            int rgb = 0;
            for (int k = 0; k < s.size(); ++k)
            {
                const int n = s[k];
                rgb = (rgb << 4) | (n <= '9' ? n - '0' : (n & 31) + 9);
            }
            rgb = SWAPRB(rgb);

            if (t == "bkgrnd") rgb_bkgrnd = rgb;
            else if (t == "string") rgb_string = rgb;
            else if (t == "ident") rgb_ident = rgb;
            else if (t == "keyword") rgb_keyword = rgb;
            else if (t == "comment") rgb_comment = rgb;
            else if (t == "digit") rgb_digit = rgb;
            else if (t == "default") rgb_default = rgb;
        }
        else if (t == "edit_tabs")
        {
            in >> edit_tabs;
        }
        else if (t == "edit_blkcursor")
        {
            in >> edit_blkcursor;
        }
        else if (t == "edit_backup")
        {
            in >> edit_backup;
        }
        else if (t == "img_toolbar")
        {
            getline(in, img_toolbar);
        }
        else if (t == "cmd_line")
        {
            getline(in, cmd_line);
        }
        else if (t == "file_recent")
        {
            std::string l;
            getline(in, l);
            if (recentFiles.size() < 10) recentFiles.push_back(l);
        }
        else
        {
            std::string s = "Unrecognized option '" + t + "' in blitzide.prefs";
            AfxMessageBox("Error in preferences file");
            setDefault();
            return;
        }
    }
    createFonts();
}

void Prefs::close() const
{
    std::ofstream out((homeDir + "/cfg/blitzide.prefs").c_str());
    if (!out.good()) return;

    out << "prg_debug\t" << prg_debug << std::endl;
    out << "prg_lastbuild\t" << prg_lastbuild << std::endl;
    out << "win_maximized\t" << win_maximized << std::endl;
    out << "win_notoolbar\t" << win_notoolbar << std::endl;
    out << "win_rect\t" << win_rect.left << ' ' << win_rect.top << ' ' << win_rect.right << ' ' << win_rect.bottom
        << std::endl;
    out << "font_editor\t\"" << font_editor << "\" " << font_editor_height << std::endl;
    out << "font_tabs\t\"" << font_tabs << "\" " << font_tabs_height << std::endl;
    out << "font_debug\t\"" << font_debug << "\" " << font_debug_height << std::endl;
    out << "font_window\t\"" << font_window << "\" " << font_window_height << std::endl;
    out << std::hex;
    out << "rgb_bkgrnd\t" << SWAPRB(rgb_bkgrnd) << std::endl;
    out << "rgb_string\t" << SWAPRB(rgb_string) << std::endl;
    out << "rgb_ident\t" << SWAPRB(rgb_ident) << std::endl;
    out << "rgb_keyword\t" << SWAPRB(rgb_keyword) << std::endl;
    out << "rgb_comment\t" << SWAPRB(rgb_comment) << std::endl;
    out << "rgb_digit\t" << SWAPRB(rgb_digit) << std::endl;
    out << "rgb_default\t" << SWAPRB(rgb_default) << std::endl;
    out << std::dec;
    out << "edit_tabs\t" << edit_tabs << std::endl;
    out << "edit_blkcursor\t" << edit_blkcursor << std::endl;
    out << "edit_backup\t" << edit_backup << std::endl;
    out << "img_toolbar\t" << img_toolbar << std::endl;
    out << "cmd_line\t" << cmd_line << std::endl;
    for (int k = 0; k < recentFiles.size(); ++k)
    {
        out << "file_recent\t" << recentFiles[k] << std::endl;
    }

    RemoveFontResource((homeDir + "/cfg/blitz.fon").c_str());
}

void Prefs::setDefault()
{
    RECT r;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &r, 0);
    const auto dw = r.right - r.left;
    const auto dh = r.bottom - r.top;
    const auto w = dw / 2;
    const auto h = dh * 3 / 4;
    win_rect.left = r.left + (dw - w) / 2;
    win_rect.top = r.top + (dh - h) / 2;
    win_rect.right = win_rect.left + w;
    win_rect.bottom = win_rect.top + h;

    win_maximized = false;
    win_notoolbar = false;
    font_editor = "Lucida Console";
    font_editor_height = 12;
    font_tabs = "Verdana";
    font_tabs_height = 10;
    font_debug = "Verdana";
    font_debug_height = 10;
    font_window = "Verdana";
    font_window_height = 10;

    rgb_bkgrnd = RGB(0x22, 0x55, 0x88);
    rgb_string = RGB(0x00, 0xff, 0x66);
    rgb_ident = RGB(0xff, 0xff, 0xff);
    rgb_keyword = RGB(0xaa, 0xff, 0xff);
    rgb_comment = RGB(0xff, 0xee, 0x00);
    rgb_digit = RGB(0x33, 0xff, 0xdd);
    rgb_default = RGB(0xee, 0xee, 0xee);

    edit_tabs = 4;
    edit_blkcursor = false;
    edit_backup = 2;

    prg_debug = true;

    img_toolbar = "toolbar.bmp";

    recentFiles.clear();

    createFonts();
}

void Prefs::createFonts()
{
    editFont.Detach();
    tabsFont.Detach();
    debugFont.Detach();
    windowFont.Detach();

    editFont.CreatePointFont(font_editor_height * 10, font_editor.c_str());
    tabsFont.CreatePointFont(font_tabs_height * 10, font_tabs.c_str());
    debugFont.CreatePointFont(font_debug_height * 10, font_debug.c_str());
    windowFont.CreatePointFont(font_window_height * 10, font_window.c_str());
}
