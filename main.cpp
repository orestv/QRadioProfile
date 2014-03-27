/*
 * File:   main.cpp
 * Author: ovol
 *
 * Created on February 3, 2014, 3:42 PM
 */
#include <iostream>

#include <QApplication>
#include "import/Importer.h"
#include "processing/Processor.h"
#include "gui/MainWindow.h"
#include <QVector3D>

#include <gsl/gsl_math.h>
#include <gsl/gsl_monte.h>
#include <gsl/gsl_monte_plain.h>
#include <gsl/gsl_monte_miser.h>
#include <gsl/gsl_monte_vegas.h>

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>

using namespace Eigen;

#include <Qt3D/QTriangle3D>

QVector3D point(1, 1, 0);

//double
//g(double *k, size_t dim, void *params) {
//    QTriangle3D triangle(QVector3D(0, 0, 0),
//                         QVector3D(1, 0, 0),
//                         QVector3D(0, 1, 0));
//    double x = k[0], y = k[1];
//    QVector3D v(x, y, 0);
//    if (!triangleContains(triangle, v))
//        return 0;
//    return 1;
//}


int main(int argc, char *argv[]) {
    
    // initialize resources, if needed
    // Q_INIT_RESOURCE(resfile);

    QTriangle3D triangle(QVector3D(-1, 0, 3),
                         QVector3D(1, 0, 1),
                         QVector3D(0, 1, 3));

    Eigen::Matrix3d newBasis = Processor::getCoordinatesTransformationMatrix(triangle);
    std::cout<<newBasis<<std::endl<<std::endl;

    QVector3D newCenter = triangle.center();

    Vector3d eCenter = Processor::switchCoordinates(newCenter, newBasis);
    eCenter = -eCenter;

    std::cout<<eCenter<<std::endl<<std::endl;

    QVector3D pt(0, 0, -2);
    pt = triangle.q();
    Vector3d ept = Processor::switchCoordinates(pt, newBasis, eCenter);

    std::cout<<ept<<std::endl;

    return 0;

//    double xl[2] = {0, 0};
//    double xu[2] = {1, 1};
//    double res, rerr;

//    const gsl_rng_type *T;
//    gsl_rng *r;
//    gsl_monte_function G = {&g, 2, 0};
//    size_t calls = 5000;

//    T = gsl_rng_default;
//    r = gsl_rng_alloc(T);

//    {
//        gsl_monte_plain_state *s = gsl_monte_plain_alloc(2);
//        gsl_monte_plain_integrate(&G, xl, xu, 2,
//                                  calls, r, s, &res, &rerr);
//        gsl_monte_plain_free(s);
//        std::cout<<"Result: "<<res;
//    };

//    return 0;

    QApplication app(argc, argv);

    MainWindow w;
    w.show();

    return app.exec();
}
