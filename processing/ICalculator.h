#ifndef ICALCULATOR_H
#define ICALCULATOR_H

#include <stdlib.h>

class ICalculator
{
public:
    virtual long double calculate(double *k, size_t dim, void *params) = 0;
};

#endif // ICALCULATOR_H
