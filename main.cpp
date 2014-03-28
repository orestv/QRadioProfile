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


int main(int argc, char *argv[]) {

//    calc();
//    return 0;
    
    // initialize resources, if needed
    // Q_INIT_RESOURCE(resfile);

    QApplication app(argc, argv);

    MainWindow w;
    w.show();

    return app.exec();
}
