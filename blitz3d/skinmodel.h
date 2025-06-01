
#ifndef SKINMODEL_H
#define SKINMODEL_H

#include "meshmodel.h"

class SkinModel : public MeshModel {
    vector<Object *> _bones;
    vector <Surface::Bone> _surf_bones;
public:
    SkinModel();

    void setBones(const vector<Object *> &bones);

    //Model interface
    void render(const RenderContext &rc) override;
};

#endif