#ifndef ECALCULATOR_H
#define ECALCULATOR_H

#include "ICalculator.h"
#include <QVector3D>
#include <Qt3D/QTriangle3D>

class ECalculator : public ICalculator
{
public:
    ECalculator(const QVector3D &observationPoint,
                const QTriangle3D &triangle,
                long double f, long double wavelength);
    long double calculate(double *k, size_t dim, void *params) override;
};

#endif // ECALCULATOR_H
