/* 
 * File:   Point.h
 * Author: ovol
 *
 * Created on February 3, 2014, 3:48 PM
 */

#ifndef POINT_H
#define	POINT_H

class Point {
public:
    Point();
    Point(double, double, double);
    Point(const Point& orig);
    virtual ~Point();
    
    double x() const {return this->_x;}
    double y() const {return this->_y;}
    double z() const {return this->_z;}
    
private:
    double _x, _y, _z;    
};

#endif	/* POINT_H */

