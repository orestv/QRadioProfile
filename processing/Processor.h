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
    
    constexpr static mdouble LIGHTSPEED = 299792458;

    static std::complex<mdouble> getE(const MVector &viewPoint, QList<Triangle> &model, const mdouble wavelength, const mdouble amplitude);
    static std::complex<mdouble> getEs(const MVector &viewpoint, const Triangle &triangle, const mdouble wavelength, const mdouble amplitude);
    static bool isTriangleVisible(const Triangle &triangle, const QList<Triangle> &model, const MVector &viewPoint);
    static MMatrix getCoordinatesTransformationMatrix(const Triangle &triangle);
    static MVector switchCoordinates(const MVector &vector, const MMatrix &matrix, const MVector &dCenter);
    static MVector switchCoordinates(const MVector &vector, const MMatrix &matrix);

    static mdouble getAngleBisectionLength(const Triangle &triangle, int angleIndex);

    static QVector3D projectOntoPlane(const QVector3D &vector, QVector3D plane_normal);
    static MVector projectOntoPlane(const MVector &point, MVector plane_normal);
    
private:

};

#endif	/* PROCESSOR_H */

