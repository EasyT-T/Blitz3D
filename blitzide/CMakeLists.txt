cmake_minimum_required(VERSION 3.16)

project(blitzide)

add_executable(blitzide WIN32
        about.cpp
        blitzide.cpp
        editor.cpp
        funclist.cpp
        helphtml.cpp
        libs.cpp
        mainframe.cpp
        prefs.cpp
        stdafx.cpp
        tabber.cpp
        blitzide.rc
        about.h
        blitzide.h
        editor.h
        funclist.h
        helphtml.h
        libs.h
        mainframe.h
        prefs.h
        stdafx.h
        tabber.h
)

target_compile_options(blitzide PRIVATE /Gz)

target_link_options(blitzide PRIVATE "/FORCE:MULTIPLE")

copy_exe_to_install(blitzide bin/ide)
