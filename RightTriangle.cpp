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

QVector3D RightTriangle::vertex() const {
    return this->_vertex;
}

QVector3D RightTriangle::p1() const {
    return this->_p1;
}

QVector3D RightTriangle::leg_1() const {
    return _p1 - _vertex;
}

QVector3D RightTriangle::p2() const {
    return this->_p2;
}

QVector3D RightTriangle::leg_2() const {
    return _p2 - _vertex;
}

QVector3D RightTriangle::normal() const {
    return QVector3D::normal(leg_1(), leg_2());
}