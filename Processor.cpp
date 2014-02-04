/* 
 * File:   Processor.cpp
 * Author: seth
 * 
 * Created on February 3, 2014, 10:02 PM
 */

#include <qt5/QtCore/qlist.h>
#include <QDebug>
#include "Processor.h"
#include <math.h>

const double ANGLE_PRECISION = 0.01;

QList<RightTriangle> 
Processor::getVisibleTriangles(
        QList<RightTriangle> triangles, 
        QVector3D viewpoint) {
    QList<RightTriangle> result;
    
    for (auto t = triangles.begin(); t != triangles.end(); t++) {
        if (Processor::isTriangleVisible(*t, viewpoint))
            result.push_back(*t);
    }
    
    return result;
}

bool 
Processor::isTriangleVisible(
        RightTriangle triangle, 
        QVector3D viewpoint) {
    QVector3D viewVector =  viewpoint - triangle.vertex();
    viewVector.normalize();
    float viewAngleCos = QVector3D::dotProduct(viewVector, triangle.normal());
    return viewAngleCos > 0;
}

Processor::VIEWPOINT_SUMS
Processor::calculateViewpointSums(
        QList<RightTriangle> triangles, 
        QVector3D viewpoint, 
        double wavelength) {
    
    VIEWPOINT_SUMS result;
    
    for (auto t = triangles.begin(); t != triangles.end(); t++) {
        TRIANGLE_ANGLES angles = calculateTriangleAngles(*t, viewpoint);
        QVector3D shortLeg = t->shortLeg(), 
            longLeg = t->longLeg();
        double En = calculateEn(angles, shortLeg, longLeg, wavelength);
    }
        
    return result;
}

Processor::TRIANGLE_ANGLES
Processor::calculateTriangleAngles(
        RightTriangle &triangle, 
        QVector3D &viewpoint) {
        
    QVector3D shortLeg = triangle.shortLeg(), 
            longLeg = triangle.longLeg();
    
    QVector3D viewVector = viewpoint - triangle.vertex();
    QVector3D shortLegPlaneNormal = longLeg;
    if (QVector3D::dotProduct(viewVector, shortLegPlaneNormal) < 0)
        shortLegPlaneNormal = -shortLegPlaneNormal;
    shortLegPlaneNormal.normalize();
    
    QVector3D vv_projection = projectOntoPlane(viewVector, 
            shortLegPlaneNormal);
    QVector3D vv_projection_unit = vv_projection.normalized();
    
    double cos_alpha = QVector3D::dotProduct(vv_projection_unit, 
            triangle.normal());
    double cos_beta = QVector3D::dotProduct(viewVector.normalized(), 
            vv_projection_unit);
    
    TRIANGLE_ANGLES result;
    result.alpha = acos(cos_alpha);
    result.beta = acos(cos_beta);    
    return result;
}

QVector3D 
Processor::projectOntoPlane(
        QVector3D &vector, 
        QVector3D plane_normal) {
    
    plane_normal.normalize();
    
    QVector3D projection_onto_normal = plane_normal * 
            QVector3D::dotProduct(plane_normal, vector);
    
    return vector - projection_onto_normal;
}

double 
Processor::calculateEn(
        TRIANGLE_ANGLES& angles, 
        QVector3D& shortLeg, 
        QVector3D& longLeg,
        double wavelength) {
    
    double result;
    
    double alpha = angles.alpha, beta = angles.beta;
    if (alpha < ANGLE_PRECISION)
        alpha = 0;
    if (beta < ANGLE_PRECISION)
        beta = 0;
    double a = shortLeg.length();
    double b = longLeg.length();
    
    double sigma = 4 * M_PI * 
                shortLeg.lengthSquared() * longLeg.lengthSquared() / (
                pow(wavelength, 2));
    double k = 2 * M_PI / wavelength;
    
    if (alpha == 0 && beta == 0) {
        result = sigma;
    } else if (beta == 0) {
        result = sigma * pow(cos(alpha), 2) * 
                pow((sin(k*a*cos(alpha))) / (k*a*sin(alpha)), 4);
    } else if (alpha == 0) {
        double kbsinb = k * b * sin(beta);
        result = sigma * pow(cos(beta), 2) * 
                (
                    pow(sin(kbsinb)/kbsinb, 4) + 
                    pow(((1 - (sin(2*kbsinb))/(kbsinb)) / (kbsinb)), 2)
                );
    } else {
        double kasinacosb = 2*k*a*sin(alpha)*cos(beta),
                kbsinb = 2*k*b*sin(beta);
        result = (sigma * pow(cos(alpha)*cos(beta), 2) / 
                pow(
                    pow(k*a*sin(alpha)*cos(beta), 2) - 
                    pow(k*b*sin(beta), 2)
                , 2)) * 
                (pow(
                    pow(sin(k*a*sin(alpha)*cos(beta)), 2) - 
                    pow(sin(k*b*sin(beta)), 2)
                , 2) + 
                pow(k*b*sin(beta), 2) * 
                    pow( (sin(kasinacosb)/kasinacosb) - (sin(kbsinb)/kbsinb), 2)
                );
    }
    
    return result;
}