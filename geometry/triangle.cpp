#include "triangle.h"
#include <iostream>
using namespace std;

Triangle::Triangle(QVector3D &p, QVector3D &q, QVector3D &r) {
    _vertices[0]<<p.x(), p.y(), p.z();
    _vertices[1]<<q.x(), q.y(), q.z();
    _vertices[2]<<r.x(), r.y(), p.z();

    init();
}

Triangle::Triangle(MVector &p, MVector &q, MVector &r) {
    _vertices[0] = p;
    _vertices[1] = q;
    _vertices[2] = r;

    init();
}

Triangle::Triangle(const Triangle &other)
{
    for (int i = 0; i < 3; i++)
        _vertices[i] = other._vertices[i];

    init();
}

void Triangle::init() {
    _center << 0, 0, 0;
    for (int i = 0; i < 3; i++)
        _center += _vertices[i];
    _center /= 3;
//    cout<<"Calculating normal"<<endl;
//    cout<<"q - p = "<<_q - _p<<endl;
//    cout<<"r - p = "<<_r - _p<<endl;
//    cout<<"Cross = "<<(_q-_p).cross(_r-_p)<<endl;
    _faceNormal = (_vertices[1]-_vertices[0]).
            cross(_vertices[2]-_vertices[0]).normalized();
}

MVector Triangle::p() const
{
    return _vertices[0];
}

MVector Triangle::q() const
{
    return _vertices[1];
}

MVector Triangle::r() const
{
    return _vertices[2];
}

MVector Triangle::faceNormal() const
{
    return _faceNormal;
}

MVector Triangle::center() const
{
    return _center;
}

MVector Triangle::leftMost() const {
    MVector result = _vertices[0];
    for (int i = 1; i < 3; i++)
        if (_vertices[i][0] < result[0] ||
                (_vertices[i][0] == result[0] && _vertices[i][1] <= result[1]))
            result = _vertices[i];
    return result;
}

MVector Triangle::rightMost() const {
    MVector result = _vertices[0];
    for (int i = 1; i < 3; i++)
        if (_vertices[i][0] > result[0] ||
                (_vertices[i][0] == result[0] && _vertices[i][1] >= result[1]))
            result = _vertices[i];
    return result;
}

MVector Triangle::middle() const {
    for (int i = 0; i < 3; i++)
        if (_vertices[i] != leftMost() && _vertices[i] != rightMost())
            return _vertices[i];
}

const MVector& Triangle::operator [](int index) const {
    index = index % 3;
    return _vertices[index];
}

Plane::Plane(MVector &origin, MVector &norm)
{
    _origin = origin;
    _norm = norm;
}

Plane::Plane(const Plane &other) {
    _origin = other._origin;
    _norm = other._norm;
}

MVector Plane::normal() const {
    return _norm;
}

MVector Plane::origin() const {
    return _origin;
}
