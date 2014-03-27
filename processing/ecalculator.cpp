#include "ecalculator.h"

#include <math.h>

ECalculator::ECalculator(
        const QVector3D &viewpoint,
        const QTriangle3D &triangle,
        long double f,
        long double wavelength) {

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

long double ECalculator::calculateSin(double *k, size_t dim, void *params) {
    QVector3D point(k[0], k[1], 0);
    if (!triangleContains(_triangle, point))
        return 0;

    return sin(2*M_PI*_viewpoint.x()*k[0]/(_wavelength*_viewpoint.z()) +
            2*M_PI*_viewpoint.y()*k[1]/(_wavelength*_viewpoint.z()));
}
