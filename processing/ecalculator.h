#ifndef ECALCULATOR_H
#define ECALCULATOR_H

#include "ICalculator.h"
#include <QVector3D>
#include <Qt3D/QTriangle3D>

class ECalculator
{
public:
    ECalculator(const QVector3D &viewpoint,
                const QTriangle3D &triangle,
                long double f, long double wavelength);
    long double calculateSin(double *k, size_t dim, void *params);
    long double calculateCos(double *k, size_t dim, void *params);

private:
    QTriangle3D _triangle;
    long double _wavelength;
    QVector3D _viewpoint;
};

#endif // ECALCULATOR_H
