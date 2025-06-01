#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

#include "cachedtexture.h"

class Texture
{
public:
    Texture();

    Texture(const std::string& file, int flags);

    Texture(const std::string& file, int flags, int w, int h, int first, int cnt);

    Texture(int width, int height, int flags, int cnt);

    Texture(const Texture& texture);

    ~Texture();

    Texture& operator=(const Texture& texture);

    void setScale(float u_scale, float v_scale) const;

    void setRotation(float rot) const;

    void setPosition(float u_pos, float v_pos) const;

    void setBlend(int blend) const;

    void setFlags(int flags) const;

    int getCanvasFlags() const;

    gxCanvas* getCanvas(int frame) const;

    const gxScene::Matrix* getMatrix() const;

    int getBlend() const;

    int getFlags() const;

    CachedTexture* getCachedTexture() const;

    bool isTransparent() const;

    bool operator<(const Texture& t) const;

    static void clearFilters();

    static void addFilter(const std::string& filter, int flags);

private:
    struct Rep;
    Rep* rep;
};

#endif
