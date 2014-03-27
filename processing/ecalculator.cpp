#include "ecalculator.h"

#include <math.h>
using namespace std;

#include <gsl/gsl_math.h>
#include <gsl/gsl_monte.h>
#include <gsl/gsl_monte_plain.h>
#include <gsl/gsl_monte_miser.h>
#include <gsl/gsl_monte_vegas.h>

#include <QDebug>

ECalculator::ECalculator(
        const QVector3D &viewpoint,
        const QTriangle3D &triangle,
        double wavelength) {

    this->_viewpoint = viewpoint;
    this->_triangle = triangle;
    this->_wavelength = wavelength;
}

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

double calculateSin(double *k, size_t dim, void *params) {
    ECalculator::PARAMS *calcParams = (ECalculator::PARAMS*)params;
    QTriangle3D &_triangle = calcParams->triangle;
    QVector3D &_viewpoint = calcParams->viewpoint;
    double _wavelength = calcParams->wavelength;

    QVector3D point(k[0], k[1], 0);
    if (!triangleContains(_triangle, point))
        return 0;

    return sin(2*M_PI*_viewpoint.x()*k[0]/(_wavelength*_viewpoint.z()) +
            2*M_PI*_viewpoint.y()*k[1]/(_wavelength*_viewpoint.z()));
}

double calculateCos(double *k, size_t dim, void *params) {
    ECalculator::PARAMS *calcParams = (ECalculator::PARAMS*)params;
    QTriangle3D &_triangle = calcParams->triangle;
    QVector3D &_viewpoint = calcParams->viewpoint;
    double _wavelength = calcParams->wavelength;

    QVector3D point(k[0], k[1], 0);
    if (!triangleContains(_triangle, point))
        return 0;

    return cos(2*M_PI*_viewpoint.x()*k[0]/(_wavelength*_viewpoint.z()) +
            2*M_PI*_viewpoint.y()*k[1]/(_wavelength*_viewpoint.z()));
}

std::complex<double>
ECalculator::calculateIntegral() const {

    qDebug()<<"Calculating integral over "<<_triangle.p()<<_triangle.q()<<_triangle.r();

    std::complex<double> result;

    Eigen::Vector2d lowerBounds, upperBounds;
    lowerBounds = getLowerLeftBounds();
    upperBounds = getUpperRightBounds();

    double xl[2] = {lowerBounds[0], lowerBounds[1]};
    double xu[2] = {upperBounds[0], upperBounds[1]};

    double res, rerr;

    PARAMS params = {_triangle, _viewpoint,_wavelength};

    const gsl_rng_type *T;
    gsl_rng *r;
    gsl_monte_function G_cos = {&calculateCos, 2, &params};
    size_t calls = 5000;

    T = gsl_rng_default;
    r = gsl_rng_alloc(T);

    {
        gsl_monte_plain_state *s = gsl_monte_plain_alloc(2);
        gsl_monte_plain_integrate(&G_cos, xl, xu, 2,
                                  calls, r, s, &res, &rerr);
        gsl_monte_plain_free(s);
        result.real(res);
    }

    r = gsl_rng_alloc(T);
    gsl_monte_function G_sin = {&calculateSin, 2, &params};
    {
        gsl_monte_plain_state *s = gsl_monte_plain_alloc(2);
        gsl_monte_plain_integrate(&G_sin, xl, xu, 2,
                                  calls, r, s, &res, &rerr);
        gsl_monte_plain_free(s);
        result.imag(res);
    }

    return result;
}

Eigen::Vector2d
ECalculator::getLowerLeftBounds() const {

    Eigen::Vector2d result;
    result<<INFINITY, INFINITY;

    result[0] = min(_triangle.p().x(), (float)result[0]);
    result[0] = min(_triangle.q().x(), (float)result[0]);
    result[0] = min(_triangle.r().x(), (float)result[0]);

    result[1] = min(_triangle.p().y(), (float)result[1]);
    result[1] = min(_triangle.q().y(), (float)result[1]);
    result[1] = min(_triangle.r().y(), (float)result[1]);

    return result;
}

Eigen::Vector2d
ECalculator::getUpperRightBounds() const {

    Eigen::Vector2d result;
    result<<-INFINITY, -INFINITY;

    result[0] = max(_triangle.p().x(), (float)result[0]);
    result[0] = max(_triangle.q().x(), (float)result[0]);
    result[0] = max(_triangle.r().x(), (float)result[0]);

    result[1] = max(_triangle.p().y(), (float)result[1]);
    result[1] = max(_triangle.q().y(), (float)result[1]);
    result[1] = max(_triangle.r().y(), (float)result[1]);

    return result;
}
