#include "triangle.h"

Triangle::Triangle(QVector3D &p, QVector3D &q, QVector3D &r) {
    _p<<p.x(), p.y(), p.z();
    _p<<q.x(), q.y(), q.z();
    _r<<r.x(), r.y(), p.z();

    _center = (_p + _q + _r)/3;
}

Triangle::Triangle(Vector3d &p, Vector3d &q, Vector3d &r) {
    _p = p;
    _q = q;
    _r = r;

    _center = (_p + _q + _r)/3;
    _faceNormal = (_q-_p).cross(_r-_p);
    _v0 = _r - _p;
    _v1 = _q - _p;
}

Triangle::Triangle(const Triangle &other)
{
    _p = other._p;
    _q = other._q;
    _r = other._r;

    _center = other._center;
    _faceNormal = (_q-_p).cross(_r-_p);
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
