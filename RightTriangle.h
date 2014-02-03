/* 
 * File:   RightTriangle.h
 * Author: ovol
 *
 * Created on February 3, 2014, 3:50 PM
 */

#ifndef RIGHTTRIANGLE_H
#define	RIGHTTRIANGLE_H

#include "Point.h"

class RightTriangle {
public:
    RightTriangle();
    RightTriangle(Point vertex, Point p1, Point p2);
    RightTriangle(const RightTriangle& orig);
    virtual ~RightTriangle();
private:
    Point _vertex, _p1, _p2;
};

#endif	/* RIGHTTRIANGLE_H */

