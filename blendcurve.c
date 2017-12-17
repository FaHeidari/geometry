
//First constructor
template <typename T>
inline
Blendcurve<T>::Blendcurve( PCurve<T,3>* c1,PCurve<T,3>* c2, T x) {
    _d = 1;
    _k = 2;
    _c1=c1;
    _c2=c2;
    _x=x;

}


template <typename T>
inline
void Blendcurve<T>::eval( T t, int /*d_not_used*/, bool /*l*/ ) const {

    this->_p.setDim( _d + 1 );

    T end1 = _c1->getParEnd();
    T delta1 = _c1->getParDelta();
    T start2 = _c2->getParStart();
    T delta2 = _c2->getParDelta();

    if(t< end1 - delta1*_x)
        this->_p = _c1->evaluateParent(t,0);

    else if(t< end1){
        auto t_hat = (t-(end1 - delta1*_x))/ (_x*delta1);
        auto var1 = _c1->evaluateParent(t,0);
        auto var2 = _c2->evaluateParent(start2 + t_hat * _x * delta2,0);
        this->_p = var1 + getB(t_hat) * (var2 - var1 );
    }

    else if(t<= end1 + (1- _x)*delta2  )
        this->_p = _c2->evaluateParent((start2+_x*delta2+t-end1),0);

}



template <typename T>
T Blendcurve<T>::getEndP() const {
    return _c2->getParEnd() + (_c1->getParEnd() - _c1->getParDelta() * _x );
}


template <typename T>
T Blendcurve<T>::getStartP() const {
    return  _c1->getParStart();
}


//B function
template <typename T>
inline
T Blendcurve<T>::getB( T t) const {
    //page153 - Bfunction
    return ( 3*std::pow(t,2) - 2*std::pow(t,3));
}


template <typename T>
inline
bool Blendcurve<T>::isClosed() const {
    return false;
}

