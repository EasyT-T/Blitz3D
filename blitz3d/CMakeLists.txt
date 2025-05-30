cmake_minimum_required(VERSION 3.16)

project(blitz)

add_library(blitz3d
        animation.cpp
        animator.cpp
        brush.cpp
        cachedtexture.cpp
        camera.cpp
        collision.cpp
        entity.cpp
        frustum.cpp
        geom.cpp
        light.cpp
        listener.cpp
        loader_3ds.cpp
        loader_b3d.cpp
        loader_x.cpp
        md2model.cpp
        md2norms.cpp
        md2rep.cpp
        meshcollider.cpp
        meshloader.cpp
        meshmodel.cpp
        meshutil.cpp
        mirror.cpp
        model.cpp
        object.cpp
        pivot.cpp
        planemodel.cpp
        q3bspmodel.cpp
        q3bsprep.cpp
        sprite.cpp
        std.cpp
        surface.cpp
        terrain.cpp
        terrainrep.cpp
        texture.cpp
        world.cpp
        animation.h
        animator.h
        blitz3d.h
        brush.h
        cachedtexture.h
        camera.h
        collision.h
        entity.h
        frustum.h
        geom.h
        light.h
        listener.h
        loader_3ds.h
        loader_b3d.h
        loader_x.h
        md2model.h
        md2norms.h
        md2rep.h
        meshcollider.h
        meshloader.h
        meshmodel.h
        meshutil.h
        mirror.h
        model.h
        object.h
        pivot.h
        planemodel.h
        q3bspmodel.h
        q3bsprep.h
        rendercontext.h
        sprite.h
        std.h
        surface.h
        terrain.h
        terrainrep.h
        texture.h
        world.h
)

target_compile_options(blitz3d PRIVATE /Gz)

set(LIBS gxruntime ${CMAKE_CURRENT_SOURCE_DIR}/minidx9/lib/d3dxof.lib)
set(LIBS ${LIBS} soloud)

target_link_libraries(blitz3d ${LIBS})
