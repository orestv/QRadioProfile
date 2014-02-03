/* 
 * File:   Processor.h
 * Author: seth
 *
 * Created on February 3, 2014, 10:02 PM
 */

#ifndef PROCESSOR_H
#define	PROCESSOR_H

#include "RightTriangle.h"
#include <QVector3D>

class Processor {
public:
    static QList<RightTriangle> getVisibleTriangles(QList<RightTriangle> triangles, 
            QVector3D viewpoint);
    static bool isTriangleVisible(RightTriangle triangle, QVector3D viewpoint);
private:

};

#endif	/* PROCESSOR_H */

