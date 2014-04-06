#include "triangle.h"
#include <iostream>
using namespace std;

Triangle::Triangle(QVector3D &p, QVector3D &q, QVector3D &r) {
    _p<<p.x(), p.y(), p.z();
    _q<<q.x(), q.y(), q.z();
    _r<<r.x(), r.y(), p.z();

    init();
}

Triangle::Triangle(Vector3d &p, Vector3d &q, Vector3d &r) {
    _p = p;
    _q = q;
    _r = r;

    init();
}

Triangle::Triangle(const Triangle &other)
{
    _p = other._p;
    _q = other._q;
    _r = other._r;

    init();
}

void Triangle::init() {
    _center = (_p + _q + _r)/3;
//    cout<<"Calculating normal"<<endl;
//    cout<<"q - p = "<<_q - _p<<endl;
//    cout<<"r - p = "<<_r - _p<<endl;
//    cout<<"Cross = "<<(_q-_p).cross(_r-_p)<<endl;
    _faceNormal = (_q-_p).cross(_r-_p).normalized();
    _v0 = _r - _p;
    _v1 = _q - _p;
}

Vector3d Triangle::p() const
{
    return _p;
}

Vector3d Triangle::q() const
{
    return _q;
}

Vector3d Triangle::r() const
{
    return _r;
}

Vector3d Triangle::v0() const {
    return _v0;
}

Vector3d Triangle::v1() const {
    return _v1;
}

Vector3d Triangle::faceNormal() const
{
    return _faceNormal;
}

Vector3d Triangle::center() const
{
    return _center;
}

Vector3d Triangle::leftMost() const {
    Vector3d result = _p;
    if (_q[0] <= result[0])
        if (_q[0] < result[0] || _q[1] <= result[1])
            result = _q;
    if (_r[0] <= result[0] && _r[1] <= result[1])
        if (_r[0] < result[0] || _r[1] <= result[1])
            result = _r;
    return result;
}

Vector3d Triangle::rightMost() const {
    Vector3d result = _p;
    if (_q[0] >= result[0])
        if (_q[0] > result[0] || _q[1] >= result[1])
            result = _q;
    if (_r[0] >= result[0] && _r[1] >= result[1])
        if (_r[0] > result[0] || _r[1] >= result[1])
            result = _r;
    return result;
}

Vector3d Triangle::middle() const {
    if (_p != leftMost() && _p != rightMost())
        return _p;
    if (_q != leftMost() && _q != rightMost())
        return _q;
    if (_r != leftMost() && _r != rightMost())
        return _r;
}

Plane::Plane(Vector3d &origin, Vector3d &norm)
{
    _origin = origin;
    _norm = norm;
}

Plane::Plane(const Plane &other) {
    _origin = other._origin;
    _norm = other._norm;
}

Vector3d Plane::normal() const {
    return _norm;
}

Vector3d Plane::origin() const {
    return _origin;
}
