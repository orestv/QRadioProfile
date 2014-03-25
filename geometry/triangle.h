#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QVector>
#include <QVector3D>
#include <Qt3D/QTriangle3D>

class Triangle
{
public:
    Triangle();
    Triangle(Triangle &orig);

private:
    QVector<QVector3D> _vertices;
};

#endif // TRIANGLE_H
