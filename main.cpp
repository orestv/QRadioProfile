/*
 * File:   main.cpp
 * Author: ovol
 *
 * Created on February 3, 2014, 3:42 PM
 */
#include <iostream>

#include <QApplication>
#include "gui/MainWindow.h"
#include <QDebug>


#include <gsl/gsl_math.h>
#include <gsl/gsl_monte.h>
#include <gsl/gsl_monte_plain.h>
#include <gsl/gsl_monte_miser.h>
#include <gsl/gsl_monte_vegas.h>

#include "geometry/triangle.h"


double f(double *k, size_t dim, void *params) {
    double x = k[0], y = k[1];
    if (y > x)
        return 0;
    return 2;
}

double g(double *k, size_t dim, void *params) {
    double x = k[0], y = k[1];
    if (y > x)
        return 0;
    return 1;
}

void calc() {
    double xl[2] = {0, 0};
    double xu[2] = {1, 1};

    double res_f, res_g, rerr;

    const gsl_rng_type *T;
    gsl_rng *r;
    gsl_monte_function G_f = {&f, 2, 0};
    gsl_monte_function G_g = {&g, 2, 0};
    size_t calls = 5000;

    T = gsl_rng_default;
    r = gsl_rng_alloc(T);

    {
        gsl_monte_miser_state *s = gsl_monte_miser_alloc(2);
        gsl_monte_miser_integrate(&G_f, xl, xu, 2,
                                  calls, r, s, &res_f, &rerr);
        gsl_monte_miser_integrate(&G_g, xl, xu, 2,
                                  calls, r, s, &res_g, &rerr);
        gsl_monte_miser_free(s);
    }
    qDebug()<<res_f<<res_g;
}

bool _triangleContains(const Triangle &triangle, const Vector3d &point) {
    Vector3d v0 = triangle.v0(),
            v1 = triangle.v1(),
            v2 = point - triangle.p();

    double dot00 = v0.dot(v0);
    double dot01 = v0.dot(v1);
    double dot02 = v0.dot(v2);
    double dot11 = v1.dot(v1);
    double dot12 = v1.dot(v2);

    double invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
    double u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    double v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    return (u >= 0) && (v >= 0) && (u + v < 1);
}


int main(int argc, char *argv[]) {

//    Vector3d p, q, r;
//    p<<0, 0, 0;
//    q<<1, 0, 0;
//    r<<0, 2, 0;
//    Triangle t(p, q, r);
//    std::cout<<t.center()<<std::endl<<std::endl<<t.faceNormal()<<std::endl;
//    Vector3d z;

//    z<<0.5, 0.2, 10;
//    std::cout<<_triangleContains(t, z);
//    return 0;

//    calc();
//    return 0;
    
    // initialize resources, if needed
    // Q_INIT_RESOURCE(resfile);

    QApplication app(argc, argv);

    MainWindow w;
    w.show();

    return app.exec();
}
