
#ifndef SPRITE_H
#define SPRITE_H

#include "brush.h"
#include "model.h"
#include "../gxruntime/gxmesh.h"

class Sprite : public Model {
public:
    enum {
        VIEW_MODE_FREE = 1,        //visible from any angle
        VIEW_MODE_FIXED = 2,        //visible only from front
        VIEW_MODE_UPRIGHT = 3,    //upright tree-style
        VIEW_MODE_UPRIGHT2 = 4    //better upright tree-style
    };

    Sprite();

    Sprite(const Sprite &t);

    ~Sprite() override;

    Sprite *getSprite() override { return this; }

    Entity *clone() override { return d_new Sprite(*this); }

    void capture() override;

    bool beginRender(float tween) override;

    void setRotation(float angle);

    void setScale(float x_scale, float y_scale);

    void setHandle(float x, float y);

    void setViewmode(int mode);

    bool render(const RenderContext &rc) override;

private:
    float xhandle, yhandle;
    float rot, xscale, yscale;
    float r_rot, r_xscale, r_yscale;
    int view_mode, mesh_index;
    bool captured;
};

#endif