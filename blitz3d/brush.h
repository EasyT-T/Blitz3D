#ifndef BRUSH_H
#define BRUSH_H

#include "geom.h"
#include "texture.h"

class Brush
{
public:
    Brush();

    Brush(const Brush& t);

    Brush(const Brush& a, const Brush& b);

    ~Brush();

    Brush& operator=(const Brush& t);

    void setColor(const Vector& color) const;

    void setAlpha(float alpha) const;

    void setShininess(float shininess) const;

    void setBlend(int blend) const;

    void setFX(int fx) const;

    void setTexture(int index, const Texture& t, int frame) const;

    const Vector& getColor() const;

    float getAlpha() const;

    float getShininess() const;

    int getBlend() const;

    int getFX() const;

    Texture getTexture(int index) const;

    const gxScene::RenderState& getRenderState() const;

    bool operator<(const Brush& b) const;

private:
    struct Rep;
    mutable Rep* rep;

    Rep* write() const;
};

#endif
