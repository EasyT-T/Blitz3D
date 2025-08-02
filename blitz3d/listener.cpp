#include "listener.h"
#include "std.h"

extern gxAudio* gx_audio;

Listener::Listener(const float roll, const float dopp, const float dist)
{
    gx_audio->set3DSettings(dopp, dist, roll);
    renderListener();
}

Listener::Listener(const Listener& t) :
    Object(t)
{
}

Listener::~Listener()
{
    constexpr ThreeDListenerAttributes attributes
    {
        {0, 0, 0},
        {0, 0, 1},
        {0, 1, 0},
        {0, 0, 0}
    };

    gx_audio->set3DListenerAttributes(attributes);
}

void Listener::renderListener() const
{
    const Vector& pos = getWorldTform().v;
    const Vector& vel = getVelocity();
    const Vector& forward = getWorldTform().m.k.normalized();
    const Vector& up = getWorldTform().m.j.normalized();

    ThreeDListenerAttributes attributes
    {
        {pos.x, pos.y, pos.z},
        {vel.x, vel.y, vel.z},
        {forward.x, forward.y, forward.z},
        {up.x, up.y, up.z},
    };

    gx_audio->set3DListenerAttributes(attributes);
}
