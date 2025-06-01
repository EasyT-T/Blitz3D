
#include "q3bspmodel.h"
#include "q3bsprep.h"
#include "std.h"

struct Q3BSPModel::Rep : public Q3BSPRep {
    int ref_cnt;

    Rep(const std::string &f, const float gam) : Q3BSPRep(f, gam),
                                      ref_cnt(1) {
    }
};

Q3BSPModel::Q3BSPModel(const std::string &f, const float gam) :
        rep(d_new Rep(f, gam)) {
}

Q3BSPModel::Q3BSPModel(const Q3BSPModel &t) : Model(t),
                                              rep(t.rep) {
    ++rep->ref_cnt;
}

Q3BSPModel::~Q3BSPModel() {
    if (!--rep->ref_cnt) delete rep;
}

bool Q3BSPModel::collide(const Line &line, const float radius, Collision *curr_coll, const Transform &t) {
    return rep->collide(line, radius, curr_coll, t);
}

bool Q3BSPModel::render(const RenderContext &rc) {
    rep->render(this, rc);
    return false;
}

void Q3BSPModel::setAmbient(const Vector &t) const
{
    rep->setAmbient(t);
}

void Q3BSPModel::setLighting(const bool l) const
{
    rep->setLighting(l);
}

bool Q3BSPModel::isValid() const {
    return rep->isValid();
}