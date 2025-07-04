#include "gxdevice.h"
#include "gxruntime.h"
#include "std.h"

gxDevice::gxDevice()
{
    reset();
}

gxDevice::~gxDevice()
{
}

void gxDevice::reset()
{
    memset(down_state, 0, sizeof(down_state));
    memset(axis_states, 0, sizeof(axis_states));
    memset(hit_count, 0, sizeof(hit_count));
    put = get = 0;
}

void gxDevice::downEvent(const int key)
{
    down_state[key] = true;
    ++hit_count[key];
    if (put - get < QUE_SIZE) que[put++ & QUE_MASK] = key;
}

void gxDevice::upEvent(const int key)
{
    down_state[key] = false;
}

void gxDevice::setDownState(const int key, const bool down)
{
    if (down == down_state[key]) return;
    if (down) downEvent(key);
    else upEvent(key);
}

void gxDevice::flush()
{
    update();
    memset(hit_count, 0, sizeof(hit_count));
    put = get = 0;
}

bool gxDevice::keyDown(const int key)
{
    update();
    return down_state[key];
}

int gxDevice::keyHit(const int key)
{
    update();
    const int n = hit_count[key];
    hit_count[key] -= n;
    return n;
}

int gxDevice::getKey()
{
    update();
    return get < put ? que[get++ & QUE_MASK] : 0;
}

float gxDevice::getAxisState(const int axis)
{
    update();
    return axis_states[axis];
}
