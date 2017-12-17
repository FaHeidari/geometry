#ifndef BLENDCURVE_H
#define BLENDCURVE_H

#include <gmParametricsModule>
#include "mycurve.h"
#include <cmath>

using namespace GMlib;

template <typename T>
class Blendcurve : public PCurve<T,3> {
    GM_SCENEOBJECT(Blendcurve)
    public:
        Blendcurve( PCurve<T,3>* c1,PCurve<T,3>* c2, T x);
    Blendcurve( Blendcurve<T>* c1 );
    virtual ~Blendcurve(){}

    // from PCurve
    bool                      isClosed() const override;

protected:
    // Virtual function from PCurve that has to be implemented locally
    void                      eval(T t, int d = 0, bool l = false) const override;
    T                         getStartP() const override;
    T                         getEndP()   const override;


    // Help function
    T                         getB(T t) const;
    // Protected data for the curve
    PCurve<T,3>*               _c1; // curve1
    PCurve<T,3>*               _c2; // curve2
    int                       _d; // polynomial degree
    int                       _k; //Order which is degree+1
    T                         _x;

}; // END class Blendcurve



//
#include "blendcurve.c"


#endif // BLENDCURVE_H


