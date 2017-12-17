template <typename T>
inline
myCurve<T>::myCurve( T size ) {

    _size = size;

}

template <typename T>
inline
myCurve<T>::myCurve( const myCurve<T>& copy ) : PCurve<T,3>( copy ) {}

template <typename T>
bool myCurve<T>::isClosed() const {
    return true;
}

template <typename T>
void myCurve<T>::eval( T t, int d, bool /*l*/ ) const {

    this->_p.setDim( d + 1 );

    //loxodrome
    this->_p[0][0] =  T(2*sin(2*t)*cos(20*t));
    this->_p[0][1] =  T(2*sin(2*t)*sin(20*t));
    this->_p[0][2] =  T(2*cos(2*t));

    //    //http://mathworld.wolfram.com/SphericalSpiral.html
    //    this->_p[0][0] =  T(2*cos(2*t)*cos(20*t));
    //    this->_p[0][1] =  T(2*sin(2*t)*sin(20*t));
    //    this->_p[0][2] =  T(-2*sin(2*t));





}


template <typename T>
T myCurve<T>::getStartP() const {
    return T(0);
}


template <typename T>
T myCurve<T>::getEndP() const {
    return T(M_PI);
}
