
#include "terrain.h"
#include "std.h"
#include "terrainrep.h"

Terrain::Terrain(const int size_shift) :
        rep(d_new TerrainRep(size_shift)) {
}

Terrain::~Terrain() {
    delete rep;
}

void Terrain::setDetail(const int n, const bool m) const
{
    rep->setDetail(n, m);
}

void Terrain::setShading(const bool t) const
{
    rep->setShading(t);
}

void Terrain::setHeight(const int x, const int z, const float h, const bool realtime) const
{
    if (x >= 0 && z >= 0 && x <= rep->getSize() && z <= rep->getSize()) rep->setHeight(x, z, h, realtime);
}

int Terrain::getSize() const {
    return rep->getSize();
}

float Terrain::getHeight(const int x, const int z) const {
    return (x >= 0 && z >= 0 && x <= rep->getSize() && z <= rep->getSize()) ? rep->getHeight(x, z) : 0;
}

bool Terrain::render(const RenderContext &rc) {
    rep->render(this, rc);
    return false;
}

bool Terrain::collide(const Line &line, const float radius, Collision *curr_coll, const Transform &tf) {
    return rep->collide(line, radius, curr_coll, tf);
}
