#ifndef LISTENER_H
#define LISTENER_H

#include "object.h"

class Listener : public Object
{
public:
    Listener(float roll, float dopp, float dist);

    Listener(const Listener& t);

    ~Listener() override;

    //Entity interface
    Entity* clone() override { return d_new Listener(*this); }

    Listener* getListener() override { return this; }

    //Listener interface
    void renderListener() const;

private:
};

#endif
