#ifndef SCENARIO_H
#define SCENARIO_H

#include "application/gmlibwrapper.h"

// qt
#include <QObject>

//MyProjects
namespace GMlib {
template<typename T, int n>
class PCurve;

}

class Scenario : public GMlibWrapper {
    Q_OBJECT
public:
    using GMlibWrapper::GMlibWrapper;

    void    initializeScenario() override;
    void    cleanupScenario() override;

public slots:

    void  reDraw();

private:
    GMlib::PCurve<float,3>* _mySpline{nullptr};
    GMlib::PCurve<float,3>* _mySpline_2{nullptr};
    GMlib::PCurve<float,3>* _myCurve{nullptr};
    GMlib::PCurve<float,3>* _myErb{nullptr};
    GMlib::PSurf<float,3>* _mySubSurf_1{nullptr};
    GMlib::PSurf<float,3>* _mySubSurf_2{nullptr};

};

#endif // SCENARIO_H
