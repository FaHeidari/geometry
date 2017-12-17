
//First constructor
template <typename T>
inline
BErb<T>::BErb(PCurve<T,3>* p, int n) {
    _d = 1; //Degree
    _k = 2; //Order
    if(p->isClosed()){
        _closed = true;
        n++;
    }
    _c.setDim(n);
    T s = p->getParStart();
    T e = p->getParEnd();
    generateKnotVector(n,s,e);
    generateSubCurve(p,n);

}

template <typename T>
inline
BErb<T>::BErb( const BErb<T>& copy ): PCurve<T,3>( copy ){
    _c = copy._c;
    _d = copy._d;
    _k = copy._k;
    _t = copy._t;
}

template <typename T>
inline
void BErb<T>::generateKnotVector(int n,T start, T end)  {
    _t.setDim( n +_k );
    T d = (end-start) / T(n);
    if(!_closed){
        _t[0] = start;
        _t[1] = start;
        for(int i=2;i<n;i++){
            _t[i] = start + T(i) * d;
        }
        _t[n] = end;
        _t[n+1] = end;
    }
    else{
        _t[1] = start;
        for(int i=2;i<n;i++){
            _t[i] = start + T(i) * d;
        }
        _t[n] = end;
        _t[n+1] = _t[n]+(_t[2] - _t[1]);
        _t[0] = start - (_t[n]-_t[n-1]);

    }
}

template <typename T>
inline
void BErb<T>::generateSubCurve(PCurve<T,3>* p,int n){

    for (int i=0 ;i<n;i++){
        if(p->isClosed() && i == n-1){
            _c[n-1] = _c[0];
        }
        else{
            _c[i] = new PSubCurve<T>(p,_t(i),_t(i+2),_t(i+1));
            this->insert(_c[i]);
            _c[i]->toggleDefaultVisualizer();
            _c[i]->replot(100);
            _c[i]->setCollapsed(false);
            _c[i]->setVisible(true);
        }
    }
}

template <typename T>
T BErb<T>::getEndP() const {
    return _t(_c.getDim());
}

template <typename T>
T BErb<T>::getStartP() const {
    return _t(_d);
}

template <typename T>
inline
int BErb<T>::getIndex( T t) const {
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
bool BErb<T>::isClosed() const {
    return _closed;
}

template <typename T>
inline
void BErb<T>::eval( T t, int d, bool /*l*/ ) const {
    this->_p.setDim( d + 1 );
    int i = getIndex(t);
    T b1 = getB(getW(t,i,d));
    T b0 = 1 - b1;
    this->_p[0] = _c[i-1]->evaluateParent(t,0)(0) * b0 + _c[i]->evaluateParent(t,0)(0) * b1;
}

template <typename T>
inline
T BErb<T>::getW( T t,int i,int d) const {
    return T(t - _t(i))/ (_t(i+d)- _t(i));
}

//B function
template <typename T>
inline
T BErb<T>::getB( T t) const {
    //page 156
    return T(std::pow(t,2)/(2*std::pow(t,2) - 2*t +1));
}

template <typename T>
inline
void BErb<T>::localSimulate(double dt){

   for(int i=0;i<_c.getDim();i++){
        _c[i]->rotate(1 , GMlib::Vector<float,3>( 0.0f, 0.0f, 0.5f*dt) );
//       _c[i]->rotate(dt , GMlib::Vector<float,3>( 1.0f, 0.0f, 0.0f) );
//       _c[i]->rotate(dt , GMlib::Vector<float,3>( 0.0f, 1.0f, 0.0f) );
//       _c[i]->rotate(dt , GMlib::Vector<float,3>( 0.0f, 0.0f, 1.0f) );

}
}
