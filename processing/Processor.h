/* 
 * File:   Processor.h
 * Author: seth
 *
 * Created on February 3, 2014, 10:02 PM
 */

#ifndef PROCESSOR_H
#define	PROCESSOR_H

#include "geometry/RightTriangle.h"
#include <QVector3D>
#include "geometry/triangle.h"
#include <complex>
#include "geometry/mmatrix.h"

class Processor {
public:
    struct E_CALCULATION_PARAMS {
        MVector viewpoint;
        QList<Triangle> &model;
        long double wavelength;
    };

    struct E0_CALCULATION_RESULT {
        std::complex<long double> e;
        std::complex<long double> e1;
        std::complex<long double> e2;
        std::complex<long double> e3;
        std::complex<long double> e4;
        std::complex<long double> e5;
        Triangle newTriangle;
        MMatrix newBasis;
        MVector newViewpoint;
        long double x1, y1, x2, y2, x3, y3;
    };

    struct SIGMA_CALCULATION_RESULT {
        E0_CALCULATION_RESULT e;
        long double sigma;
    };

    struct PARAMS {
        long double viewpointHeight;
        long double viewpointDistance;
        long double viewpointRotationStep;
        long double viewpointStartAngle;
        long double viewpointEndAngle;
        long double frequency;
    };
    
    struct CALCULATION_RESULT {
        long double azimuth;
        std::complex<long double> eComplex;
        long double E;
    };
    
    struct VIEWPOINT_SUMS {
        long double sum_cos;
        long double sum_sin;
    };
    
    struct TRIANGLE_ANGLES {
        long double cos_alpha;
        long double cos_beta;
        long double sin_alpha;
        long double sin_beta;
    };
    
    constexpr static long double LIGHTSPEED = 299792458;

    static std::complex<long double> getE(const E_CALCULATION_PARAMS params);
    static std::complex<long double> getE(const MVector &viewPoint, QList<Triangle> &model, const long double wavelength);
    static std::complex<long double> getE0(const MVector &viewpoint, const Triangle &triangle, const long double wavelength);
    static bool isTriangleVisible(const Triangle &triangle, const QList<Triangle> &model, const MVector &viewPoint);
    static long double getSigma(const MVector &observationPoint, const Triangle &triangle, const long double R, const long double wavelength);
    static MMatrix getCoordinatesTransformationMatrix(const Triangle &triangle);
    static MVector switchCoordinates(const MVector &vector, const MMatrix &matrix, const MVector &dCenter);
    static MVector switchCoordinates(const MVector &vector, const MMatrix &matrix);

    static QVector3D projectOntoPlane(const QVector3D &vector, QVector3D plane_normal);
    static MVector projectOntoPlane(const MVector &point, MVector plane_normal);
    static long double calculateEn(TRIANGLE_ANGLES &angles, QVector3D &shortLeg, QVector3D &longLeg, long double wavelength);
    
private:

};

#endif	/* PROCESSOR_H */

