
#ifndef MODEL_H
#define MODEL_H

#include "brush.h"
#include "object.h"
#include "rendercontext.h"

class Sprite;

class Terrain;

class PlaneModel;

class Q3BSPModel;

class Model : public Object {
public:
    enum {
        RENDER_SPACE_LOCAL = 0,
        RENDER_SPACE_WORLD = 1
    };
    enum {
        COLLISION_GEOMETRY_DEFAULT = 0,
        COLLISION_GEOMETRY_TRIS = 1,
        COLLISION_GEOMETRY_BOX = 2,
        COLLISION_GEOMETRY_SPHERE = 3
    };
    enum {
        QUEUE_OPAQUE = 0,
        QUEUE_TRANSPARENT = 1
    };

    Model();

    Model(const Model &m);

    //Entity interface
    Model *getModel() override { return this; }

    //Object interface
    void capture() override;

    bool beginRender(float tween) override;

    //Model interface
    virtual void setRenderBrush(const Brush &b) {}

    virtual bool render(const RenderContext &rc) { return false; }

    virtual void renderQueue(int type);

    virtual Sprite *getSprite() { return nullptr; }

    virtual Terrain *getTerrain() { return nullptr; }

    virtual PlaneModel *getPlaneModel() { return nullptr; }

    virtual MeshModel *getMeshModel() { return nullptr; }

    virtual MD2Model *getMD2Model() { return nullptr; }

    virtual Q3BSPModel *getBSPModel() { return nullptr; }

    virtual void setBrush(const Brush &b) {
        brush = b;
        w_brush = true;
    }

    virtual void setColor(const Vector &c) {
        brush.setColor(c);
        w_brush = true;
    }

    virtual void setAlpha(const float a) {
        brush.setAlpha(a);
        w_brush = true;
    }

    virtual void setShininess(const float t) {
        brush.setShininess(t);
        w_brush = true;
    }

    virtual void setTexture(const int i, const Texture &t, const int f) {
        brush.setTexture(i, t, f);
        w_brush = true;
    }

    virtual void setBlend(const int n) {
        brush.setBlend(n);
        w_brush = true;
    }

    virtual void setFX(const int n) {
        brush.setFX(n);
        w_brush = true;
    }

    const Brush &getBrush() const { return brush; }

    void setRenderSpace(const int n) { space = n; }

    int getRenderSpace() const { return space; }

    void setAutoFade(const float nr, const float fr) {
        auto_fade_nr = nr;
        auto_fade_fr = fr;
        auto_fade = true;
    }

    bool doAutoFade(const Vector &eye);

    void enqueue(gxMesh *mesh, int first_vert, int vert_cnt, int first_tri, int tri_cnt);

    void enqueue(gxMesh *mesh, int first_vert, int vert_cnt, int first_tri, int tri_cnt, const Brush &b);

    int queueSize(const int type) const { return queues[type].size(); }

private:
    class MeshQueue;

    int space;
    Brush brush, render_brush;

    mutable bool w_brush;
    float captured_alpha, tweened_alpha;

    bool auto_fade;
    float auto_fade_nr, auto_fade_fr;

    std::vector<MeshQueue *> queues[2];

    void enqueue(MeshQueue *q);
};

#endif