
#ifndef LIGHT_H
#define LIGHT_H

#include "geom.h"
#include "object.h"
#include "../gxruntime/gxlight.h"

class World;

class Light : public Object {
public:
    Light(int type);

    ~Light() override;

    Light *getLight() override { return this; }

    void setRange(float r) const;

    void setColor(const Vector &v) const;

    void setConeAngles(float inner, float outer) const;

    bool beginRender(float tween) override;

    gxLight *getGxLight() const { return light; }

private:
    friend class World;

    gxLight *light;
};

#endif