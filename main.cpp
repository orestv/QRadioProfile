/*
 * File:   main.cpp
 * Author: ovol
 *
 * Created on February 3, 2014, 3:42 PM
 */
#include <iostream>

//#include <QApplication>
//#include "import/Importer.h"
//#include "processing/Processor.h"
//#include "gui/MainWindow.h"
//#include <QVector3D>

#include <gsl/gsl_math.h>
#include <gsl/gsl_monte.h>
#include <gsl/gsl_monte_plain.h>
#include <gsl/gsl_monte_miser.h>
#include <gsl/gsl_monte_vegas.h>

#include <Qt3D/QTriangle3D>

bool planeContains(const QPlane3D &plane, const QVector3D &point) {
    QVector3D planeVector = point - plane.origin();
    return QVector3D::dotProduct(plane.normal(), planeVector) == 0;
}

bool triangleContains(const QTriangle3D &triangle, const QVector3D &point) {
    if (!planeContains(triangle.plane(), point))
        return false;
    QVector3D v0 = triangle.r() - triangle.p(),
            v1 = triangle.q() - triangle.p(),
            v2 = point - triangle.p();

    double dot00 = QVector3D::dotProduct(v0, v0);
    double dot01 = QVector3D::dotProduct(v0, v1);
    double dot02 = QVector3D::dotProduct(v0, v2);
    double dot11 = QVector3D::dotProduct(v1, v1);
    double dot12 = QVector3D::dotProduct(v1, v2);

    double invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
    double u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    double v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    return (u >= 0) && (v >= 0) && (u + v < 1);
}


QVector3D point(1, 1, 0);

double
g(double *k, size_t dim, void *params) {
    QTriangle3D triangle(QVector3D(0, 0, 0),
                         QVector3D(1, 0, 0),
                         QVector3D(0, 1, 0));
    double x = k[0], y = k[1];
    QVector3D v(x, y, 0);
    if (!triangleContains(triangle, v))
        return 0;
    return 1;
}


int main(int argc, char *argv[]) {
    
    // initialize resources, if needed
    // Q_INIT_RESOURCE(resfile);

    double xl[2] = {0, 0};
    double xu[2] = {1, 1};
    double res, rerr;

    const gsl_rng_type *T;
    gsl_rng *r;
    gsl_monte_function G = {&g, 2, 0};
    size_t calls = 5000;

    T = gsl_rng_default;
    r = gsl_rng_alloc(T);

    {
        gsl_monte_plain_state *s = gsl_monte_plain_alloc(2);
        gsl_monte_plain_integrate(&G, xl, xu, 2,
                                  calls, r, s, &res, &rerr);
        gsl_monte_plain_free(s);
        std::cout<<"Result: "<<res;
    }

    return 0;

//    QApplication app(argc, argv);

//    MainWindow w;
//    w.show();

//    return app.exec();
}
