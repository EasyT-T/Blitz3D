#ifndef Q3BSPMODEL_H
#define Q3BSPMODEL_H

#include "model.h"

class Q3BSPModel : public Model
{
public:
    Q3BSPModel(const std::string& f, float gamma_adj);

    Q3BSPModel(const Q3BSPModel& m);

    ~Q3BSPModel() override;

    //Entity interface
    Entity* clone() override { return d_new Q3BSPModel(*this); }

    //Object interface
    bool collide(const Line& line, float radius, Collision* curr_coll, const Transform& t) override;

    //Model interface
    Q3BSPModel* getBSPModel() override { return this; }

    bool render(const RenderContext& rc) override;

    void setAmbient(const Vector& t) const;

    void setLighting(bool use_lmap) const;

    bool isValid() const;

private:
    struct Rep;
    Rep* rep;
};

#endif
