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


QDebug operator<< (QDebug d, RightTriangle &triangle) {
    d<<triangle.vertex()<<triangle.p1()<<triangle.p2();
    return d;
}

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
    result.sum_cos = 0;
    result.sum_sin = 0;
    double k = 2 * M_PI / wavelength;
    
    for (auto t = triangles.begin(); t != triangles.end(); t++) {
        TRIANGLE_ANGLES angles = calculateTriangleAngles(*t, viewpoint);
        QVector3D shortLeg = t->shortLeg(), 
            longLeg = t->longLeg();
        double En = calculateEn(angles, shortLeg, longLeg, wavelength);
        double R = (viewpoint - t->vertex()).length();  
        
        qDebug()<<"En == "<<En;
        
        result.sum_cos += En * cos(k*R);
        result.sum_sin += En * sin(k*R);
    }
        
    return result;
}

Processor::TRIANGLE_ANGLES
Processor::calculateTriangleAngles(
        RightTriangle &triangle, 
        QVector3D &viewpoint) {
    
    QVector3D viewVector = viewpoint - triangle.vertex();    
    QVector3D triangleNormal = triangle.normal();
    
    TRIANGLE_ANGLES result;
    
    if (abs(triangle.leg_1().lengthSquared() - 
            triangle.leg_2().lengthSquared()) > 0.01) {
        QVector3D shortLegPlaneNormal = triangle.longLeg().normalized();
        result = calculateTriangleAngles(triangleNormal, shortLegPlaneNormal, viewVector);
    } else {
        QVector3D normal_1 = triangle.leg_1().normalized(),
                normal_2 = triangle.leg_2().normalized();
        TRIANGLE_ANGLES result_1 = calculateTriangleAngles(triangleNormal,
                normal_1, viewVector),
                result_2 = calculateTriangleAngles(triangleNormal,
                normal_2, viewVector);
        result.alpha = fmin(result_1.alpha, result_2.alpha);
        result.beta = fmin(result_1.beta, result_2.beta);
    }    
    return result;
}

Processor::TRIANGLE_ANGLES
Processor::calculateTriangleAngles (
        QVector3D& triangleNormal,
        QVector3D& planeNormal, 
        QVector3D& viewVector) {
        
    QVector3D vv_projection = projectOntoPlane(viewVector, 
            planeNormal);
//    qDebug()<<"Projected vector: "<<vv_projection<<", was projected onto "<<planeNormal;
    QVector3D vv_projection_unit = vv_projection.normalized();
//    qDebug()<<"Projection unit: "<<vv_projection_unit;
    
    double cos_alpha = QVector3D::dotProduct(vv_projection_unit, 
            triangleNormal);
//    qDebug()<<"Calculating angle between "<<vv_projection_unit<<" and "<<triangleNormal;
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