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

class Processor {
public:
    struct PARAMS {
        double viewpointHeight;
        double viewpointDistance;
        double viewpointRotationStep;
        double frequency;
    };
    
    struct CALCULATION_RESULT {
        double azimuth;
        double E;
        double sum_sin;
        double sum_cos;
    };
    
    struct VIEWPOINT_SUMS {
        double sum_cos;
        double sum_sin;
    };
    
    struct TRIANGLE_ANGLES {
        double alpha;
        double beta;
    };
    
    constexpr static double LIGHTSPEED = 299792458;
    
    static QList<CALCULATION_RESULT> analyzeModel(QList<RightTriangle> triangles,
        Processor::PARAMS parameters);
    
    static QList<RightTriangle> getVisibleTriangles(QList<RightTriangle> triangles, 
            QVector3D viewpoint);
    static bool isTriangleVisible(RightTriangle triangle, QVector3D viewpoint);
    static VIEWPOINT_SUMS calculateViewpointSums(QList<RightTriangle> triangles, QVector3D viewpoint, double wavelength);
    static TRIANGLE_ANGLES calculateTriangleAngles(RightTriangle &triangle, QVector3D &viewpoint);
    static TRIANGLE_ANGLES calculateTriangleAngles(QVector3D &triangleNormal, QVector3D planeNormal, QVector3D &viewVector);
    static QVector3D projectOntoPlane(QVector3D &vector, QVector3D plane_normal);
    static double calculateEn(TRIANGLE_ANGLES &angles, QVector3D &shortLeg, QVector3D &longLeg, double wavelength);
    
private:

};

#endif	/* PROCESSOR_H */

