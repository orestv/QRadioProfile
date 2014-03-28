#ifndef ECALCULATOR_H
#define ECALCULATOR_H

#include "ICalculator.h"
#include <QVector3D>
#include "geometry/triangle.h"
#include <complex>
#include <eigen3/Eigen/Core>

class ECalculator
{
public:
    struct PARAMS {
        const Triangle *triangle;
        const Vector3d *viewpoint;
        double wavelength;
    };

    ECalculator(const Vector3d &viewpoint,
                const Triangle &triangle,
                double wavelength);
    std::complex<double> calculateIntegral() const;
    Eigen::Vector2d getLowerLeftBounds() const;
    Eigen::Vector2d getUpperRightBounds() const;

private:
    Triangle _triangle;
    double _wavelength;
    Vector3d _viewpoint;
};

double calculateSin(double *k, size_t dim, void *params);
double calculateCos(double *k, size_t dim, void *params);

#endif // ECALCULATOR_H
