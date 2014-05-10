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
#include "geometry/mtypes.h"

class Processor {
public:

    struct E0_CALCULATION_RESULT {
        std::complex<mdouble> e;
        std::complex<mdouble> e1;
        std::complex<mdouble> e2;
        std::complex<mdouble> e3;
        std::complex<mdouble> e4;
        std::complex<mdouble> e5;
        Triangle newTriangle;
        MMatrix newBasis;
        MVector newViewpoint;
        mdouble x1, y1, x2, y2, x3, y3;
    };

    struct SIGMA_CALCULATION_RESULT {
        E0_CALCULATION_RESULT e;
        mdouble sigma;
    };

    struct PARAMS {
        mdouble viewpointHeight;
        mdouble viewpointDistance;
        mdouble viewpointRotationStep;
        mdouble viewpointStartAngle;
        mdouble viewpointEndAngle;
        mdouble frequency;
        mdouble amplitude;
    };
    
    struct CALCULATION_RESULT {
        mdouble azimuth;
        std::complex<mdouble> eComplex;
        mdouble E;
    };
    
    struct VIEWPOINT_SUMS {
        mdouble sum_cos;
        mdouble sum_sin;
    };
    
    struct TRIANGLE_ANGLES {
        mdouble cos_alpha;
        mdouble cos_beta;
        mdouble sin_alpha;
        mdouble sin_beta;
    };
    
    constexpr static mdouble LIGHTSPEED = 299792458;

    static std::complex<mdouble> getE(const MVector &viewPoint, QList<Triangle> &model, const mdouble wavelength, const mdouble amplitude);
    static std::complex<mdouble> getE0(const MVector &viewpoint, const Triangle &triangle, const mdouble wavelength);
    static bool isTriangleVisible(const Triangle &triangle, const QList<Triangle> &model, const MVector &viewPoint);
    static mdouble getSigma(const MVector &observationPoint, const Triangle &triangle, const mdouble R, const mdouble wavelength);
    static MMatrix getCoordinatesTransformationMatrix(const Triangle &triangle);
    static MVector switchCoordinates(const MVector &vector, const MMatrix &matrix, const MVector &dCenter);
    static MVector switchCoordinates(const MVector &vector, const MMatrix &matrix);

    static QVector3D projectOntoPlane(const QVector3D &vector, QVector3D plane_normal);
    static MVector projectOntoPlane(const MVector &point, MVector plane_normal);
    static mdouble calculateEn(TRIANGLE_ANGLES &angles, QVector3D &shortLeg, QVector3D &longLeg, mdouble wavelength);
    
private:

};

#endif	/* PROCESSOR_H */

