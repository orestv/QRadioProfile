/* 
 * File:   Point.cpp
 * Author: ovol
 * 
 * Created on February 3, 2014, 3:48 PM
 */

#include "Point.h"

Point::Point() {
}

Point::Point(double x, double y, double z){
    this->_x = x;
    this->_y = y;
    this->_z = z;
}

Point::Point(const Point& orig) {
    this->_x = orig._x;
    this->_y = orig._y;
    this->_z = orig._z;
}

Point::~Point() {
}

