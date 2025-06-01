#ifndef PLANEMODEL_H
#define PLANEMODEL_H

#include "brush.h"
#include "model.h"

class PlaneModel : public Model
{
public:
    PlaneModel(int sub_divs);

    PlaneModel(const PlaneModel& t);

    ~PlaneModel() override;

    Entity* clone() override { return d_new PlaneModel(*this); }

    //model interface
    bool render(const RenderContext& rc) override;

    //object interface
    bool collide(const Line& line, float radius, Collision* curr_coll, const Transform& tf) override;

    Plane getRenderPlane() const;

private:
    struct Rep;

    Rep* rep;

    PlaneModel* getPlaneModel() override { return this; }
};

#endif
