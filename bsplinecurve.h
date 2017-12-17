#ifndef PBSPLINE_H
#define PBSPLINE_H

#include <gmParametricsModule>

using namespace GMlib;

template <typename T>
class PBSpline : public PCurve<T,3> {
    GM_SCENEOBJECT(PBSpline)
    public:
        PBSpline( const DVector< Vector<T, 3> >& c, int d);
    PBSpline( const DVector< Vector<T, 3> >& p,int d,int n);
    PBSpline( const PBSpline<T>& curve );
    virtual ~PBSpline();

    void                      setDegree( int d );
    int                       getDegree() const;
    void                      generateKnotVector(int n);
    void                      generateControlPoints(DVector<Vector<T,3>> p,int n);

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

    // Protected data for the curve
    DVector< Vector<T,3> >    _c; // control points (controlpolygon)
    int                       _d; // polynomial degree
    DVector<T>                _t; // knot vector
    int                       _k; //Order which is degree+1
    T                         _n;


}; // END class PBSpline



// Include PBSplineCurve class function implementations
#include "bsplinecurve.c"


#endif // PBSPLINE_H


