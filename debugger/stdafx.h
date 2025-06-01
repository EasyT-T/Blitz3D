#ifndef STDAFX_H
#define STDAFX_H

#pragma warning(disable:4786)

#define _WIN32_WINNT 0x601

#include <afxcmn.h>            // Common Controls
#include <afxrich.h>        // CRich edit
#include <afxwin.h>         // Core

#include <list>
#include <map>
#include <string>
#include <vector>

//some stuff that should be in std libs
int atoi(const std::string& s);

double atof(const std::string& s);

std::string itoa(int n);

std::string ftoa(float n);

std::string tolower(const std::string& s);

std::string toupper(const std::string& s);

std::string fullfilename(const std::string& t);

std::string filenamepath(const std::string& t);

std::string filenamefile(const std::string& t);

#endif
