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
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>

class Processor {
public:
    struct PARAMS {
        double viewpointHeight;
        double viewpointDistance;
        double viewpointRotationStep;
        double viewpointStartAngle;
        double viewpointEndAngle;
        double frequency;
    };
    
    struct CALCULATION_RESULT {
        double azimuth;
        std::complex<double> eComplex;
        double E;
    };

    struct LOCAL_CALCULATION_RESULT {
        double E;

    };
    
    struct VIEWPOINT_SUMS {
        double sum_cos;
        double sum_sin;
    };
    
    struct TRIANGLE_ANGLES {
        double cos_alpha;
        double cos_beta;
        double sin_alpha;
        double sin_beta;
    };
    
    constexpr static double LIGHTSPEED = 299792458;

    static std::complex<double> getE(const Vector3d &viewPoint, QList<Triangle> &model, const double wavelength);
    static std::complex<double> getE0(const Vector3d &viewpoint, const Triangle &triangle, const double wavelength);
    static bool isTriangleVisible(const Triangle &triangle, const QList<Triangle> &model, const Vector3d &viewPoint);
    static double getSigma(const Vector3d &observationPoint, const Triangle &triangle, const double wavelength);
    static long double getU(const Vector3d &observationPoint, const Triangle &triangle, const double wavelength);
    static Eigen::Matrix3d getCoordinatesTransformationMatrix(const Triangle &triangle);
    static Eigen::Vector3d switchCoordinates(const Vector3d &vector, const Eigen::Matrix3d &matrix, const Eigen::Vector3d &dCenter);
    static Eigen::Vector3d switchCoordinates(const Vector3d &vector, const Eigen::Matrix3d &matrix);
    
    static QList<CALCULATION_RESULT> analyzeModel(QList<RightTriangle> &triangles,
        Processor::PARAMS parameters);
    
    static QList<RightTriangle> getVisibleTriangles(QList<RightTriangle> &triangles,
            QVector3D viewpoint);
    static bool isTriangleVisible(RightTriangle triangle, QVector3D viewpoint);
    static VIEWPOINT_SUMS calculateViewpointSums(QList<RightTriangle> &triangles, QVector3D viewpoint, double wavelength);
    static TRIANGLE_ANGLES calculateTriangleAngles(RightTriangle &triangle, QVector3D &viewpoint);
    static TRIANGLE_ANGLES calculateTriangleAngles(QVector3D &triangleNormal, QVector3D planeNormal, QVector3D &viewVector);
    static QVector3D projectOntoPlane(const QVector3D &vector, QVector3D plane_normal);
    static Vector3d projectOntoPlane(const Vector3d &vector, Vector3d plane_normal);
    static double calculateEn(TRIANGLE_ANGLES &angles, QVector3D &shortLeg, QVector3D &longLeg, double wavelength);
    
private:

};

#endif	/* PROCESSOR_H */

