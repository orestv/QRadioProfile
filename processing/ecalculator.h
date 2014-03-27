#ifndef ECALCULATOR_H
#define ECALCULATOR_H

#include "ICalculator.h"
#include <QVector3D>
#include <Qt3D/QTriangle3D>
#include <complex>
#include <eigen3/Eigen/Core>

class ECalculator
{
public:
    struct PARAMS {
        QTriangle3D triangle;
        QVector3D viewpoint;
        double wavelength;
    };

    ECalculator(const QVector3D &viewpoint,
                const QTriangle3D &triangle,
                double wavelength);
    std::complex<double> calculateIntegral() const;
    Eigen::Vector2d getLowerLeftBounds() const;
    Eigen::Vector2d getUpperRightBounds() const;

private:
    QTriangle3D _triangle;
    double _wavelength;
    QVector3D _viewpoint;
};

double calculateSin(double *k, size_t dim, void *params);
double calculateCos(double *k, size_t dim, void *params);

#endif // ECALCULATOR_H
