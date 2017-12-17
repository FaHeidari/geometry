#include "psurface.h"

template <typename T>
inline
PSimpleSub<T>::PSimpleSub( GMlib::PSurf<T,3>* s, int n1, int n2)
{
    _d=1;
    _k=_d+1;
    _closedU = s->isClosedU();
    _closedV =s->isClosedV();
    if(_closedU ){
        n1++;
    }
    if(_closedV){
        n2++;
    }
    _c.setDim(n1,n2);
    su = s->getParStartU();
    eu = s->getParEndU();
    sv = s->getParStartV();
    ev = s->getParEndV();
    generateKnotVector(n1,su,eu,true);
    generateKnotVector(n2,sv,ev,false);
    generateLocalSurface(s,n1,n2);

}


template <typename T>
void PSimpleSub<T>::eval( T u, T v, int d1, int d2, bool /*lu*/, bool /*lv*/) const {    
    this->_p.setDim(d1+1,d2+1);

    int i = getIndex(u, true);
    int j = getIndex(v, false);

    DMatrix<T> Bu(2,2); //Bu
    DMatrix<T> Bv(2,2); //Bv
    DMatrix<DMatrix<Vector<T,3>>> C(2,2);

    //Intiallizing Bu and Bv
    Bu[0][0]= 1 - getB(getW(u,i,1,true));
    Bu[0][1] = getB(getW(u,i,1,true));
    Bu[1][0] = - getDerW(u,i,1,true) * getDerB(getW(u,i,1,true));
    Bu[1][1] = getDerW(u,i,1,true) * getDerB(getW(u,i,1,true));

    Bv[0][0]= 1 - getB(getW(v,j,1,false));
    Bv[0][1] = - getDerW(v,j,1,false)*(getDerB(getW(v,j,1,false)));
    Bv[1][0] = getB(getW(v,j,1,false));
    Bv[1][1] = getDerW(v,j,1,false)* getDerB(getW(v,j,1,false));

    C[0][0] = _c(i-1)(j-1)->evaluateParent(u,v,d1,d2);
    C[0][1] = _c(i-1)(j)->evaluateParent(u,v,d1,d2);
    C[1][0] = _c(i)(j-1)->evaluateParent(u,v,d1,d2);
    C[1][1] = _c(i)(j)->evaluateParent(u,v,d1,d2);

    auto h1 = Bu[0][0] * C(0)(0)(0)(0) + Bu[0][1] * C(1)(0)(0)(0);
    auto h2 = Bu[1][0] * C(0)(0)(0)(0) + Bu[1][1] * C(1)(0)(0)(0);
    auto h3 = Bu[0][0] * C(0)(1)(0)(0) + Bu[0][1] * C(1)(1)(0)(0);
    auto h4 = Bu[1][0] * C(0)(1)(0)(0) + Bu[1][1] * C(1)(1)(0)(0);
    auto h5 = Bu[0][0] * C(0)(0)(1)(0) + Bu[0][1] * C(1)(0)(1)(0);
    auto h6 = Bu[0][0] * C(1)(0)(1)(0) + Bu[0][1] * C(1)(1)(1)(0);;
    auto h7 = Bu[0][0] * C(0)(0)(0)(1) + Bu[0][1] * C(1)(0)(0)(1);;
    auto h8 = Bu[0][0] * C(1)(0)(0)(1) + Bu[0][1] * C(1)(1)(0)(1);;


    this->_p[0][0] = h1*Bv[0][0] + h3*Bv[1][0];
    this->_p[1][0] = (h2+h5)*Bv[0][0] + (h4+h6)*Bv[1][0];
    this->_p[0][1] = h1*Bv[0][1]+h3*Bv[1][1] + h7*Bv[0][0] +h8*Bv[1][0];
}


template <typename T>
T PSimpleSub<T>::getStartPU() const {
    //return _su;
    return _tu(_d);
}


template <typename T>
T PSimpleSub<T>::getEndPU() const {
    // return _eu;
    return _tu(_c.getDim1());
}


template <typename T>
T PSimpleSub<T>::getStartPV() const {
    //return _sv;
    return _tv(_d);
}


template <typename T>
T PSimpleSub<T>::getEndPV() const {
    // return _ev;
    return _tv(_c.getDim2());
}


//***************************
// Defining help functions  **
//***************************

template <typename T>
inline
T PSimpleSub<T>::getW( T t,int i,int d,bool uv) const{    
    if(uv){
        return T((t - _tu(i))/ (_tu(i+d)- _tu(i)));
    }
    else{
        return T((t - _tv(i))/ (_tv(i+d)- _tv(i)));
    }
}


template <typename T>
inline
T PSimpleSub<T>::getDerW( T t,int i,int d, bool uv) const{    
    if(uv){
        return T(1/(_tu(i+d) - _tu(i)));
    }
    else{
        return T(1/(_tv(i+d) - _tv(i)));
    }
}


template <typename T>
inline
T PSimpleSub<T>::getB(T t) const{
    //psge 157
    return T(3*std::pow(t,2) - 2*std::pow(t,3));
}


template <typename T>
inline
T PSimpleSub<T>::getDerB(T t) const{
    //psge 157
    return T(6*t - 6*std::pow(t,2));
}


template <typename T>
inline
int PSimpleSub<T>::getIndex( T t, bool uv) const{
    int index = _d;
    if(uv){
        int n = _c.getDim1();
        for(;index<n;index++){
            if(_tu(index)<=t && t< _tu(index+1))
                break;
        }
        if(index>=n)
            index = n-1;
    }
    else{
        int n = _c.getDim2();
        for(;index<n;index++){
            if(_tv(index)<=t && t< _tv(index+1))
                break;
        }
        if(index>=n)
            index = n-1;
    }
    return (index);

}

template <typename T>
inline
void PSimpleSub<T>::generateLocalSurface(PSurf<T,3>* p, int n, int m){    
    int index1 =n, index2=m;
    if(_closedU){
        index1-=1;
    }
    if(_closedV){
        index2-=1;
    }

    for(int i=0; i<index1 ; i++){
        for(int j=0; j<index2 ; j++){
            _c[i][j] = new PSimpleSubSurf<T>(p,_tu(i),_tu(i+2),_tu(i+1),_tv(j),_tv(j+2),_tv(j+1));
            this->insert(_c[i][j]);
            _c[i][j]->toggleDefaultVisualizer();
            _c[i][j]->replot(100,100,1,1);
            _c[i][j]->setCollapsed(true);
            _c[i][j]->setVisible(true);

        }
    }
    if(_closedU){
        for(int i=0; i<m ; i++){
            _c[n-1][i]=_c[0][i];
        }
    }

    if(_closedV){
        for(int i=0; i<n ; i++){
            _c[i][m-1]=_c[i][0];
        }
    }

}


template <typename T>
inline
void PSimpleSub<T>::generateKnotVector(int n,T start, T end,bool uv)  {

    T dt = (end-start) / T(n-1);
    _tu.setDim(n+_k);
    _tv.setDim(n+_k);

    if(uv){
        if(!_closedU){
            _tu[0] = start;
            _tu[1] = start;

            for(int i=2;i<n;i++){
                _tu[i] = start + T(i-1) * dt;
            }
            _tu[n] = end;
            _tu[n+1] = end;
        }
        else {
            _tu[1]=start;
            for(int i=2;i<n;i++){
                _tu[i] = start + T(i-1) * dt;
            }
            _tu[n] = end;
            _tu[n+1] = _tu[n] + (_tu[2]-_tu[1]);
            _tu[0]= start - (_tu[n]-_tu[n-1]);

        }

        std::cout<<"tu Knot vector"<<_tu<<std::endl;
    }


    else{
        if(!_closedV){

            _tv[0] = start;
            _tv[1] = start;

            for(int i=2;i<n;i++){
                _tv[i] = start + T(i-1) * dt;
            }

            _tv[n] = end;
            _tv[n+1] = end;

        }

        else{
            _tv[1]=start;
            for(int i=2;i<n;i++){
                _tv[i] = start + T(i-1) * dt;
            }
            _tv[n] = end;
            _tv[n+1] = _tv[n] + (_tv[2]-_tv[1]);
            _tv[0]= start - (_tv[n]-_tv[n-1]);

        }
        std::cout<<"tv Knot vector"<<_tv<<std::endl;
    }

}


template <typename T>
inline
bool PSimpleSub<T>::isClosedU()const{
    return _closedU;
}


template <typename T>
inline
bool PSimpleSub<T>::isClosedV()const{
    return _closedV;
}
