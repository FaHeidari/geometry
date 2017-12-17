//First constructor
template <typename T>
inline
PBSpline<T>::PBSpline( const DVector< Vector<T, 3> >& c, int d) {

    _c = c; //Control points
    _d = d; //Degree
    _k = _d+1; //Order
    generateKnotVector(_c.getDim());

    for(int i=0;i<_c.getDim();i++){
        Selector<T,3>* s = new Selector<T,3> (_c[i],i,this,0.2);
        this->insert(s);
    }

}


//Second constructor
template <typename T>
inline
PBSpline<T>::PBSpline( const DVector< Vector<T, 3> >& p,int d,int n ){
    _d = d;
    _k = d+1;

    generateKnotVector(n);
    generateControlPoints(p,n);

}

template <typename T>
inline
PBSpline<T>::PBSpline( const PBSpline<T>& copy ) : PCurve<T,3>( copy ) {
    _c = copy._c;
    _d = copy._d;
    _t = copy._t;
}

template <typename T>
PBSpline<T>::~PBSpline() {}

template <typename T>
inline
void PBSpline<T>::generateKnotVector(int n) {

    _t.setDim( n + _k );
    int step_knots = _t.getDim() - ( _k* 2 );

    T knot_value = T(0);
    int i = 0;

    // Set the start knots
    for( ; i < _k; i++ )
        _t[i] = knot_value;

    // Set the "step"-knots
    for( int j = 0; j < step_knots; j++ )
        _t[i++] = ++knot_value;

    // Set the end knots
    knot_value++;
    for( ; i < _t.getDim(); i++ )
        _t[i] = knot_value;

    //std::cout<<"B-spline knot vector"<<_t<<std::endl;
}

template <typename T>
inline
void PBSpline<T>::generateControlPoints(DVector<Vector<T,3>> p,int n) {
    int m = p.getDim();

    //Initializing A matrix
    DMatrix<T> A(m,n);
    for(int k=0;k<m;k++){
        for(int j=0;j<n;j++){
            A[k][j] = T(0);
        }
    }

    int i=0;
    _c.setDim(n);
    T start = getStartP();
    T end= getEndP();
    T delta = (end-start) / T(m-1);
    for(int j=0;j<m;j++){
        T t = _t(start) + T(j) * delta;
        i = getIndex(t);

        DMatrix<T> d1(1,2); //first derivative
        DMatrix<T> d2(2,3); //second derivative

        d1[0][0]= 1-getW(t,i,1);
        d1[0][1]= getW(t,i,1);

        d2[0][0]= 1 - getW(t,i-1,2);
        d2[0][1]= getW(t,i-1,2);
        d2[0][2]= T(0);
        d2[1][0]= T(0);
        d2[1][1]= 1 - getW(t,i,2);
        d2[1][2]= getW(t,i,2);

        T b0= d1[0][0]*d2[0][0]+d1[0][1]*d2[1][0];
        T b1= d1[0][0]*d2[0][1]+d1[0][1]*d2[1][1];
        T b2= d1[0][0]*d2[0][2]+d1[0][1]*d2[1][2];

        A[j][i-2]=b0;
        A[j][i-1]=b1;
        A[j][i]=b2;

//        std::cout<<"j"<<j<<std::endl;
//        std::cout<<"i"<<i<<std::endl;

    }

    // Solve A*p = _c
    DMatrix<T>AT=A;
    AT.transpose();
    DMatrix<T> B = AT*A;
    DVector<Vector<T,3>> X = AT * p;
    B.invert();
    _c = B*X;
}


template <typename T>
inline
void PBSpline<T>::eval( T t, int /*d_not_used*/, bool /*l*/ ) const {
    this->_p.setDim(_d+1);
    int i = getIndex(t);

    DMatrix<T> d1(1,2); //first derivative
    DMatrix<T> d2(2,3); //second derivative

    //Initiallizing the matrices
    d1[0][0]= T(1-getW(t,i,1));
    d1[0][1]= T(getW(t,i,1));

    d2[0][0]= T(1 - getW(t,i-1,2));
    d2[0][1]= T(getW(t,i-1,2));
    d2[0][2]= T(0);
    d2[1][0]= T(0);
    d2[1][1]= T(1 - getW(t,i,2));
    d2[1][2]= T(getW(t,i,2));

    T b0= d1[0][0]*d2[0][0]+d1[0][1]*d2[1][0];
    T b1= d1[0][0]*d2[0][1]+d1[0][1]*d2[1][1];
    T b2= d1[0][0]*d2[0][2]+d1[0][1]*d2[1][2];

    this->_p[0] = (b0 * _c[i-2]) + (b1*_c[i-1]) +(b2*_c[i]) ;
}



template <typename T>
inline
int PBSpline<T>::getIndex(T t) const {
    int index = _d;
    int n = _c.getDim();
    for(;index<n;index++){
        if(_t(index)<=t && t< _t(index+1))
            break;
    }
    if(index>=n)
        index = n-1;
    return (index);
}



template <typename T>
inline
T PBSpline<T>::getW( T t,int i,int d) const {
    return T((t - _t(i))/ (_t(i+d)- _t(i)));
}


template <typename T>
T PBSpline<T>::getStartP() const {
    return _t(_d);
}


template <typename T>
T PBSpline<T>::getEndP() const {
    return _t(_c.getDim());
}

template <typename T>
inline
int PBSpline<T>::getDegree() const {
    return _d;
}


template <typename T>
inline
bool PBSpline<T>::isClosed() const {
    return false;
}




