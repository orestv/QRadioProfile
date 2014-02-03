/* 
 * File:   RightTriangle.cpp
 * Author: ovol
 * 
 * Created on February 3, 2014, 3:50 PM
 */

#include "RightTriangle.h"

RightTriangle::RightTriangle() {
}

RightTriangle::RightTriangle(QVector3D vertex, QVector3D p1, QVector3D p2) {
    this->_vertex = vertex;
    this->_p1 = p1;
    this->_p2 = p2;
}

RightTriangle::RightTriangle(const RightTriangle& orig) {
    this->_vertex = orig._vertex;
    this->_p1 = orig._p1;
    this->_p2 = orig._p2;
}

RightTriangle::~RightTriangle() {
}

