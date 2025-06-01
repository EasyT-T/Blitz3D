
#ifndef MIRROR_H
#define MIRROR_H

#include "object.h"

class Mirror : public Object {
public:
    Mirror();

    Mirror(const Mirror &t);

    ~Mirror() override;

    //Entity interface
    Entity *clone() override { return d_new Mirror(*this); }

    Mirror *getMirror() override { return this; }
};

#endif