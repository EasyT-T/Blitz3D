cmake_minimum_required(VERSION 3.16)

project(gxruntime)

set(SRCS
        std.cpp
        std.h
        asmcoder.h
        gxdir.cpp
        gxdir.h
        gxfilesystem.cpp
        gxfilesystem.h
        gxtimer.cpp
        gxtimer.h
        std.cpp
        std.h
)

set(SRCS ${SRCS}
        gxruntime_b3d.cpp
        gxruntime_b3d.h
        ddutil.cpp
        ddutil.h
        gxcanvas.cpp
        gxcanvas.h
        gxfont.cpp
        gxfont.h
        gxgraphics.cpp
        gxgraphics.h
        gxdevice.cpp
        gxdevice.h
        gxinput.cpp
        gxinput.h
        gxlight.cpp
        gxlight.h
        gxmesh.cpp
        gxmesh.h
        gxmovie.cpp
        gxmovie.h
        gxscene.cpp
        gxscene.h
)

add_library(gxruntime ${SRCS})

target_compile_options(gxruntime PRIVATE /Gz)

set(LIBS stdutil winmm Ws2_32.lib)

set(LIBS ${LIBS} freeimage amstrmid dinput8 dxguid ddraw)

target_link_libraries(gxruntime ${LIBS})
