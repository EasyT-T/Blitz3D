
#ifndef MESHMODEL_H
#define MESHMODEL_H

#include "model.h"
#include "surface.h"

class MeshCollider;

class MeshModel : public Model {
public:
    typedef std::vector<Surface *> SurfaceList;

    MeshModel();

    MeshModel(const MeshModel &t);

    ~MeshModel() override;

    //Entity interface
    MeshModel *getMeshModel() override { return this; }

    Entity *clone() override { return d_new MeshModel(*this); }

    //Object interface
    bool collide(const Line &line, float radius, Collision *curr_coll, const Transform &t) override;

    //Model interface
    void setRenderBrush(const Brush &b) override;

    bool render(const RenderContext &rc) override;

    void renderQueue(int type) override;

    //boned mesh!
    void createBones();

    //MeshModel interface
    Surface *createSurface(const Brush &b);

    void setCullBox(const Box &box) const;

    void updateNormals() const;

    void flipTriangles() const;

    void transform(const Transform &t) const;

    void paint(const Brush &b) const;

    void add(const MeshModel &t) const;

    void optimize();

    //accessors
    const SurfaceList &getSurfaces() const;

    Surface *findSurface(const Brush &b) const;

    bool intersects(const MeshModel &m) const;

    MeshCollider *getCollider() const;

    const Box &getBox() const;

private:
    struct Rep;

    Rep *rep;
    int brush_changes;
    Brush render_brush;
    std::vector<Brush> brushes;

    std::vector<Surface::Bone> surf_bones;

    MeshModel &operator=(const MeshModel &);
};

#endif
