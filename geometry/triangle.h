#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QVector3D>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>

using namespace Eigen;

class Triangle
{
public:
    Triangle() {};
    Triangle(QVector3D &p, QVector3D &q, QVector3D &r);
    Triangle(Vector3d &p, Vector3d &q, Vector3d &r);

    Triangle(const Triangle &other);

    Vector3d p() const;
    Vector3d q() const;
    Vector3d r() const;

    Vector3d v0() const;
    Vector3d v1() const;

    Vector3d faceNormal() const;
    Vector3d center() const;

private:
    void init();

    Vector3d _p;
    Vector3d _q;
    Vector3d _r;

    Vector3d _v0, _v1;

    Vector3d _faceNormal;
    Vector3d _center;
};

class Plane {
public:
    Plane(Vector3d &origin, Vector3d &norm);
    Plane(const Plane &other);

    Vector3d origin() const;
    Vector3d normal() const;

private:
    Vector3d _origin;
    Vector3d _norm;
};

#endif // TRIANGLE_H
