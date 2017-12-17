#ifndef PSIMPLESUB_H
#define PSIMPLESUB_H

#include <gmCoreModule>
#include <gmParametricsModule>
#include "psimplesubsurface.h"

using namespace GMlib;

template <typename T>
class PSimpleSub : public GMlib::PSurf<T,3> {
    GM_SCENEOBJECT(PSimpleSub)
    public:
        // Constructors and destructor
        PSimpleSub( GMlib::PSurf<T,3>* s,int n1, int n2);
    PSimpleSub( const PSimpleSub<T>& copy ){}
    virtual ~PSimpleSub(){}

protected:
    // Virtual functions from PSurf, which have to be implemented locally
    void          eval(T u, T v, int d1, int d2, bool lu = true, bool lv = true ) const override;
    T             getStartPU() const override;
    T             getEndPU()   const override;
    T             getStartPV() const override;
    T             getEndPV()   const override;
    void                      generateKnotVector(int n,T start, T end,bool uv);
    void                      generateLocalSurface(PSurf<T,3>* p, int n, int m);
    T                         getW( T t,int i,int d, bool uv) const;
    T                         getDerW(T t,int i,int d, bool uv) const;
    int                       getIndex(T t, bool uv) const;
    T                         getB(T t) const;
    T                         getDerB(T t) const;
    bool                      isClosedU()const override;
    bool                      isClosedV()const override;

    // Protected data for the surface
    GMlib::PSurf<T,3>*     _s;  // The original surface
    int                    _d;
    int                    _k;
    int                    _m;
    int                    _n;
    T                      su; // Start parameter value in u-direction
    T                      sv; // Start parameter value in v-direction
    T                      eu; // End parameter value in u-direction
    T                      ev; // End parameter value in v-direction
    T                      u;  // Center parameter value in u-direction
    T                      v;  // Center parameter value in v-direction
    DMatrix<PSimpleSubSurf<T>*>  _c; // Matrix of surface
    DVector<T>                  _tu; // knot vector
    DVector<T>                  _tv; // knot vector
    bool                      _closedU;
    bool                      _closedV;

}; // END class PSIMPLESUB_H

#include "psurface.c"

#endif // PSIMPLESUB_H
