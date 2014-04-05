#include "ecalculator.h"
#include <iostream>

#include <math.h>
using namespace std;

#include <gsl/gsl_math.h>
#include <gsl/gsl_monte.h>
#include <gsl/gsl_monte_plain.h>
#include <gsl/gsl_monte_miser.h>
#include <gsl/gsl_monte_vegas.h>

#include <QDebug>

ECalculator::ECalculator(const Vector3d &viewpoint,
        const Triangle &triangle,
        double wavelength) {

    this->_viewpoint = viewpoint;
    this->_triangle = triangle;
    this->_wavelength = wavelength;
}

bool planeContains(const Plane &plane, const Vector3d &point) {
    Vector3d planeVector = point - plane.origin();
    planeVector.normalize();
    return plane.normal().dot(planeVector) == 0;
}

float sign(const Vector3d &p1, const Vector3d &p2, const Vector3d &p3)
{
  return (p1[0] - p3[0]) * (p2[1] - p3[1]) - (p2[0] - p3[0]) * (p1[1] - p3[1]);
}

bool
PointInTriangle(
        const Vector3d &pt,
        const Vector3d &v1,
        const Vector3d &v2,
        const Vector3d &v3)
{
  bool b1, b2, b3;

  b1 = sign(pt, v1, v2) < 0.0f;
  b2 = sign(pt, v2, v3) < 0.0f;
  b3 = sign(pt, v3, v1) < 0.0f;

  return ((b1 == b2) && (b2 == b3));
}


bool triangleContains(const Triangle &triangle, const Vector3d &point) {
    return PointInTriangle(point, triangle.p(), triangle.q(), triangle.r());
//    if (!planeContains(triangle.plane(), point))
//        return false;
//    Vector3d v0 = triangle.r() - triangle.p(),
//            v1 = triangle.q() - triangle.p(),
//            v2 = point - triangle.p();
    Vector3d v0 = triangle.r() - triangle.p(),
            v1 = triangle.q() - triangle.p(),
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

double calculateSin(double *k, size_t dim, void *params) {
    ECalculator::PARAMS *calcParams = (ECalculator::PARAMS*)params;
    const Triangle *_triangle = calcParams->triangle;
    const Vector3d *_viewpoint = calcParams->viewpoint;
    double _wavelength = calcParams->wavelength;

    Vector3d point;
    point<<k[0], k[1], 0;
    if (!triangleContains(*_triangle, point))
        return 0;

    double x = k[0], y = k[1];
    double X = (*_viewpoint)[0], Y = (*_viewpoint)[1], Z = (*_viewpoint)[2];

    double result = 2*M_PI*(X*x + Y*y)/(_wavelength*Z);
    return sin(result);
}

double calculateCos(double *k, size_t dim, void *params) {
    ECalculator::PARAMS *calcParams = (ECalculator::PARAMS*)params;
    const Triangle *_triangle = calcParams->triangle;
    const Vector3d *_viewpoint = calcParams->viewpoint;
    double _wavelength = calcParams->wavelength;

    Vector3d point;
    point<<k[0], k[1], 0;
    if (!triangleContains(*_triangle, point))
        return 0;

    double x = k[0], y = k[1];
    double X = (*_viewpoint)[0], Y = (*_viewpoint)[1], Z = (*_viewpoint)[2];

    double result = 2*M_PI*(X*x + Y*y)/(_wavelength*Z);
    return cos(result);
}

std::complex<double>
ECalculator::calculateIntegral() const {

//    qDebug()<<"Calculating integral over "<<_triangle.p()<<_triangle.q()<<_triangle.r();

    std::complex<double> result;

    Eigen::Vector2d lowerBounds, upperBounds;
    lowerBounds = getLowerLeftBounds();
    upperBounds = getUpperRightBounds();

    double xl[2] = {lowerBounds[0], lowerBounds[1]};
    double xu[2] = {upperBounds[0], upperBounds[1]};

//    cout<<"Triangle: "<<_triangle.p()<<endl<<_triangle.q()<<endl<<_triangle.r()<<endl;

//    cout<<"Upper bounds:"<<upperBounds<<endl;
//    cout<<"Lower bounds:"<<lowerBounds<<endl;

    double res_sin, res_cos, rerr;

    PARAMS params = {&_triangle, &_viewpoint, _wavelength};

    const gsl_rng_type *T;
    gsl_rng *r;
    gsl_monte_function G_cos = {&calculateCos, 2, &params};
    gsl_monte_function G_sin = {&calculateSin, 2, &params};
    size_t calls = 5000;

    T = gsl_rng_default;
    r = gsl_rng_alloc(T);

    {
        gsl_monte_miser_state *s = gsl_monte_miser_alloc(2);
        gsl_monte_miser_integrate(&G_cos, xl, xu, 2,
                                  calls, r, s, &res_cos, &rerr);
        gsl_monte_miser_integrate(&G_sin, xl, xu, 2,
                                  calls, r, s, &res_sin, &rerr);
        gsl_monte_miser_free(s);
        result.real(res_cos);
        result.imag(-res_sin);
    }

    return result;
}

Eigen::Vector2d
ECalculator::getLowerLeftBounds() const {

    Eigen::Vector2d result;
    result<<INFINITY, INFINITY;

    result[0] = min(_triangle.p()[0], result[0]);
    result[0] = min(_triangle.q()[0], result[0]);
    result[0] = min(_triangle.r()[0], result[0]);

    result[1] = min(_triangle.p()[1], result[1]);
    result[1] = min(_triangle.q()[1], result[1]);
    result[1] = min(_triangle.r()[1], result[1]);

    return result;
}

Eigen::Vector2d
ECalculator::getUpperRightBounds() const {

    Eigen::Vector2d result;
    result<<-INFINITY, -INFINITY;

    result[0] = max(_triangle.p()[0], result[0]);
    result[0] = max(_triangle.q()[0], result[0]);
    result[0] = max(_triangle.r()[0], result[0]);

    result[1] = max(_triangle.p()[1], result[1]);
    result[1] = max(_triangle.q()[1], result[1]);
    result[1] = max(_triangle.r()[1], result[1]);

    return result;
}
