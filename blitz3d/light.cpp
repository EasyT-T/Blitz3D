
#include "light.h"
#include "std.h"
#include "../gxruntime/gxscene.h"

extern gxScene *gx_scene;

Light::Light(const int type) {
    light = gx_scene->createLight(type);
}

Light::~Light() {
    gx_scene->freeLight(light);
}

void Light::setRange(const float r) const
{
    light->setRange(r);
}

void Light::setColor(const Vector &v) const
{
    light->setColor((float *) &v.x);
}

void Light::setConeAngles(const float inner, const float outer) const
{
    light->setConeAngles(inner, outer);
}

bool Light::beginRender(const float tween) {
    Object::beginRender(tween);
    light->setPosition(&getRenderTform().v.x);
    light->setDirection(&getRenderTform().m.k.x);
    return true;
}
