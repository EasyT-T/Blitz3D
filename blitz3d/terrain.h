
#ifndef TERRAIN_H
#define TERRAIN_H

#include "model.h"

struct TerrainRep;

class Terrain : public Model {
public:
    Terrain(int size_shift);

    ~Terrain() override;

    Terrain *getTerrain() override { return this; }

    void setDetail(int n, bool morph) const;

    void setHeight(int x, int z, float h, bool realtime) const;

    void setShading(bool shading) const;

    int getSize() const;

    float getHeight(int x, int z) const;

    //model interface
    bool render(const RenderContext &rc) override;

    //object interface
    bool collide(const Line &line, float radius, Collision *curr_coll, const Transform &tf) override;

private:
    TerrainRep *rep;
};

#endif