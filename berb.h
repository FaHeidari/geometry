

#ifndef BERB_H
#define BERB_H

#include <gmParametricsModule>
#include "mycurve.h"
#include <cmath>
#include "../gmlib/modules/scene/src/selector/gmselector.h"

using namespace GMlib;

template <typename T>
class BErb : public PCurve<T,3> {
    GM_SCENEOBJECT(BErb)
    public:
        BErb(PCurve<T,3>* p, int n);
    BErb(const BErb<T>& copy );
    virtual ~BErb(){}

    // from PCurve
    bool                      isClosed() const override;

protected:
    // Virtual function from PCurve that has to be implemented locally
    void                      eval(T t, int d = 0, bool l = false) const override;
    T                         getStartP() const override;
    T                         getEndP()   const override;

    // Help function
    T                         getW( T t,int i,int d) const;
    int                       getIndex( T t) const;
    T                         getB(T t) const;
    void                      generateKnotVector(int n,T start, T end);
    void                      generateSubCurve(PCurve<T,3>* p,int n);
    void                      localSimulate(double dt);



    // Protected data for the curve
    DVector<PSubCurve<T>*>    _c; // Vector of curve
    int                       _d; // polynomial degree
    DVector<T>                _t; // knot vector
    int                       _k; //Order which is degree+1
    T                         _n;
    bool                      _closed;

}; // END class BErb


#include "berb.c"


#endif // BERB_H


