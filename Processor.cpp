/* 
 * File:   Processor.cpp
 * Author: seth
 * 
 * Created on February 3, 2014, 10:02 PM
 */

#include <qt5/QtCore/qlist.h>
#include <QDebug>
#include "Processor.h"

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
        QVector3D short_leg, long_leg;
        if (t->leg_1().lengthSquared() > t->leg_2().lengthSquared()) {
            short_leg = t->leg_2();
            long_leg = t->leg_1();
        } else {
            short_leg = t->leg_1();
            long_leg = t->leg_2();
        }
        
        
    }
        
    return result;
}

QVector3D projectOntoPlane(QVector3D vector, QVector3D plane_normale) {
    plane_normale.normalize();
    
    
}