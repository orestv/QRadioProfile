/* 
 * File:   RightTriangle.h
 * Author: ovol
 *
 * Created on February 3, 2014, 3:50 PM
 */

#ifndef RIGHTTRIANGLE_H
#define	RIGHTTRIANGLE_H

#include <QVector3D>

class RightTriangle {
public:
    RightTriangle();
    RightTriangle(QVector3D vertex, QVector3D p1, QVector3D p2);
    RightTriangle(const RightTriangle& orig);
    virtual ~RightTriangle();
private:
    QVector3D _vertex, _p1, _p2;
};

#endif	/* RIGHTTRIANGLE_H */

