#include "scenario.h"
#include "mycurve.h"
#include "testtorus.h"
#include "bsplinecurve.h"
#include "berb.h"
#include "blendcurve.h"
#include "psurface.h"

//// hidmanager
//#include "hidmanager/defaulthidmanager.h"

// gmlib
#include <gmOpenglModule>
#include <gmSceneModule>
#include <gmParametricsModule>

// qt
#include <QQuickItem>



void Scenario::initializeScenario() {

    // Insert a light
    GMlib::Point<GLfloat,3> init_light_pos( 2.0, 4.0, 10 );
    GMlib::PointLight *light = new GMlib::PointLight(  GMlib::GMcolor::white(), GMlib::GMcolor::white(),
                                                       GMlib::GMcolor::white(), init_light_pos );
    light->setAttenuation(0.8, 0.002, 0.0008);
    scene()->insertLight( light, false );

    // Insert Sun
    scene()->insertSun();

    // Default camera parameters
    int init_viewport_size = 600;
    GMlib::Point<float,3> init_cam_pos(  0.0f, 0.0f, 0.0f );
    GMlib::Vector<float,3> init_cam_dir( 0.0f, 1.0f, 0.0f );
    GMlib::Vector<float,3> init_cam_up(  0.0f, 0.0f, 1.0f );

    // Projection cam
    auto proj_rcpair = createRCPair("Projection");
    proj_rcpair.camera->set(init_cam_pos,init_cam_dir,init_cam_up);
    proj_rcpair.camera->setCuttingPlanes( 1.0f, 8000.0f );
    proj_rcpair.camera->rotateGlobal( GMlib::Angle(-45), GMlib::Vector<float,3>( 1.0f, 0.0f, 0.0f ) );
    proj_rcpair.camera->translateGlobal( GMlib::Vector<float,3>( 0.0f, -20.0f, 20.0f ) );
    scene()->insertCamera( proj_rcpair.camera.get() );
    proj_rcpair.renderer->reshape( GMlib::Vector<int,2>(init_viewport_size, init_viewport_size) );

    // Front cam
    auto front_rcpair = createRCPair("Front");
    front_rcpair.camera->set( init_cam_pos + GMlib::Vector<float,3>( 0.0f, -50.0f, 0.0f ), init_cam_dir, init_cam_up );
    front_rcpair.camera->setCuttingPlanes( 1.0f, 8000.0f );
    scene()->insertCamera( front_rcpair.camera.get() );
    front_rcpair.renderer->reshape( GMlib::Vector<int,2>(init_viewport_size, init_viewport_size) );

    // Side cam
    auto side_rcpair = createRCPair("Side");
    side_rcpair.camera->set( init_cam_pos + GMlib::Vector<float,3>( -50.0f, 0.0f, 0.0f ), GMlib::Vector<float,3>( 1.0f, 0.0f, 0.0f ), init_cam_up );
    side_rcpair.camera->setCuttingPlanes( 1.0f, 8000.0f );
    scene()->insertCamera( side_rcpair.camera.get() );
    side_rcpair.renderer->reshape( GMlib::Vector<int,2>(init_viewport_size, init_viewport_size) );

    // Top cam
    auto top_rcpair = createRCPair("Top");
    top_rcpair.camera->set( init_cam_pos + GMlib::Vector<float,3>( 0.0f, 0.0f, 50.0f ), -init_cam_up, init_cam_dir );
    top_rcpair.camera->setCuttingPlanes( 1.0f, 8000.0f );
    scene()->insertCamera( top_rcpair.camera.get() );
    top_rcpair.renderer->reshape( GMlib::Vector<int,2>(init_viewport_size, init_viewport_size) );


    // Surface visualizers
    auto surface_visualizer = new GMlib::PSurfDefaultVisualizer<float,3>;
    auto surface_visualizer1 = new GMlib::PSurfDefaultVisualizer<float,3>;
    auto surface_visualizer2 = new GMlib::PSurfDefaultVisualizer<float,3>;


    //surface->test01();

//    //Curve

    float size=5.0;
    _myCurve = new myCurve<float>(size);
    _myCurve->toggleDefaultVisualizer();
    _myCurve->insertVisualizer(surface_visualizer);
    _myCurve->setColor(GMlib::GMcolor::lavender());
    //_myCurve->rotate( GMlib::Angle(90), GMlib::Vector<float,3>( 0.0f, 0.0f, 1.0f) );
    // _myCurve->translateGlobal(GMlib::Vector<float,3>(0,1,0));
    _myCurve->setVisible(false);
    _myCurve->replot(200,0);
    scene()->insert(_myCurve);

    //BSpline
    DVector<Vector<float,3>> p;
    int m = 100;
    p.setDim(m);
    for(int i=0 ; i<m ; i++)
        p[i]=_myCurve->getPosition(_myCurve->getParStart()+i*_myCurve->getParDelta()/(m-1));
    _mySpline = new PBSpline<float>(p,2);
    _mySpline->toggleDefaultVisualizer();
    _mySpline->insertVisualizer(surface_visualizer);
    _mySpline->setColor(GMlib::GMcolor::turquoise());
    _mySpline->translateGlobal(GMlib::Vector<float,3>(6,1,0));
    _mySpline->replot(200,0);
    scene()->insert(_mySpline);


    //Leaste square
    auto _mySpline_2 = new PBSpline<float>(p,2,50);
    _mySpline_2->toggleDefaultVisualizer();
    _mySpline_2->insertVisualizer(surface_visualizer);
    _mySpline_2->setColor(GMlib::GMcolor::darkOliveGreen());
    _mySpline_2->translateGlobal(GMlib::Vector<float,3>(-6,1,0));
    _mySpline_2->replot(200,0);
    scene()->insert(_mySpline_2);

    //First circle
    auto _circle1 = new GMlib::PCircle<float>(10);
    _circle1->toggleDefaultVisualizer();
    _circle1->setColor(GMlib::GMcolor::darkTurquoise());
    _circle1->replot(100,0);
    scene()->insert(_circle1);

    //Second circle
    auto _circle2 = new GMlib::PCircle<float>(6);
    _circle2->toggleDefaultVisualizer();
    _circle2->setColor(GMlib::GMcolor::deepSkyBlue());
    _circle2->replot(100,0);
    scene()->insert(_circle2);

    //Blended curve
    auto _blend = new Blendcurve<float>(_circle2,_circle1,0.2);
    _blend->toggleDefaultVisualizer();
    _blend->setColor(GMlib::GMcolor::darkBlue());
    _blend->replot(100,0);
    scene()->insert(_blend);


    //GErbe
    _myErb = new BErb<float>(_myCurve,10);
    _myErb->toggleDefaultVisualizer();
    _myErb->setColor(GMlib::GMcolor::darkGreen());
    _myErb->insertVisualizer(surface_visualizer);
   // _myErb->setVisible(false);
    _myErb->replot(100,0);
    scene()->insert(_myErb);


    //Surface
    auto _myPlane = new  GMlib::PPlane<float>(
                GMlib::Point<float,3>(7.0f, 5.0f, 5.0f),
                GMlib::Vector<float,3>(10.0f, 0.0f, 0.0f),
                GMlib::Vector<float,3>(0.0f, 0.0f, -5.0f)
                );
    _myPlane->toggleDefaultVisualizer();
    _myPlane->replot(100,100,1,1);
    _myPlane->setVisible(false);
    scene()->insert(_myPlane);


    //Cylinder
    auto _cylinder = new GMlib::PCylinder<float>(1.5f,2.0f,3.0f);
    _cylinder->toggleDefaultVisualizer();
   // _cylinder->translate(Vector<float,3>(-10,3,3));
    _cylinder->replot(200,200,1,1);
    _cylinder->setVisible(false);
    scene()->insert(_cylinder);

    //first SubSurfaces
    _mySubSurf_1 = new PSimpleSub<float>(_myPlane ,3,3);
    _mySubSurf_1->toggleDefaultVisualizer();
    _mySubSurf_1->insertVisualizer(surface_visualizer1);
    _mySubSurf_1->setMaterial(GMlib::GMmaterial::sapphire());
    _mySubSurf_1->replot(100,100,1,1);
    scene()->insert(_mySubSurf_1);

    //second SubSurfaces
    _mySubSurf_2 = new PSimpleSub<float>(_cylinder ,3,3);
    _mySubSurf_2->toggleDefaultVisualizer();
    _mySubSurf_2->insertVisualizer(surface_visualizer2);
    _mySubSurf_2->setMaterial(GMlib::GMmaterial::turquoise());
    _mySubSurf_2->replot(100,100,1,1);
    scene()->insert(_mySubSurf_2);

}

void Scenario::cleanupScenario() {

}
void Scenario::reDraw(){
    if(_mySpline){
        _mySpline->replot(200,0);
    }
    if(_myErb){
        _myErb->replot(500,0);
    }
    if(_mySpline_2){
        _mySpline_2->replot(200,0);
    }
    if(_mySubSurf_1){
        _mySubSurf_1->replot(50,50,1,1);
    }
    if(_mySubSurf_2){
        _mySubSurf_2->replot(50,50,1,1);
    }
}


