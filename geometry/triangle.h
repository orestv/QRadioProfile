#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QVector3D>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>

#include "geometry/mtypes.h"

using namespace Eigen;

class Triangle
{
public:
    Triangle() {};
    Triangle(QVector3D &p, QVector3D &q, QVector3D &r);
    Triangle(MVector &p, MVector &q, MVector &r);

    Triangle(const Triangle &other);

    MVector p() const;
    MVector q() const;
    MVector r() const;

    MVector v0() const;
    MVector v1() const;

    MVector leftMost() const;
    MVector rightMost() const;
    MVector middle() const;

    MVector faceNormal() const;
    MVector center() const;

private:
    void init();

    MVector _p;
    MVector _q;
    MVector _r;

    MVector _v0, _v1;

    MVector _faceNormal;
    MVector _center;
};

class Plane {
public:
    Plane(MVector &origin, MVector &norm);
    Plane(const Plane &other);

    MVector origin() const;
    MVector normal() const;

private:
    MVector _origin;
    MVector _norm;
};

#endif // TRIANGLE_H
