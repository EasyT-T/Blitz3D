
#ifndef LIBS_H
#define LIBS_H

#include "stdafx.h"

extern int compiler_ver, linker_ver, runtime_ver;

void initLibs();

std::string quickHelp(const std::string &kw);

bool isMediaFile(const std::string &file);

#endif
