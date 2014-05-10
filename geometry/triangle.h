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

    MVector leftMost() const;
    MVector rightMost() const;
    MVector middle() const;

    MVector faceNormal() const;
    MVector center() const;

    const MVector& operator[](int index) const;

private:
    void init();

    MVector _faceNormal;
    MVector _center;

    MVector _vertices[3];
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
