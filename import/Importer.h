/* 
 * File:   Importer.h
 * Author: ovol
 *
 * Created on February 3, 2014, 3:54 PM
 */

#ifndef IMPORTER_H
#define	IMPORTER_H

#include <QList>
#include <QString>
#include <QFile>
#include <QVector3D>

#include "geometry/RightTriangle.h"
#include "geometry/triangle.h"
#include "processing/Processor.h"

class Importer {
public:

    static QList<Triangle> import(QString path);
    static void exportToFile(QString path, QList<Processor::CALCULATION_RESULT> results);
    
    static QList<RightTriangle> generateRightTriangles(QList<QVector3D> triangle);    
private:
    static QList<QVector3D> _read_vertices(QString path);
    static QList<QList<int> > _read_indices(QString path);
    
    static QVector3D _parse_vertex_line(QByteArray line);
    static QList<int> _parse_face_line(QByteArray line);
    static QList<QList<QVector3D> > _generate_faces(QList<QVector3D> points, QList<QList<int> > face_indices);
    static QList<Triangle> _generate_triangles(QList<QList<QVector3D> > faces);
    static QList<RightTriangle> _generate_right_triangles(QList<QList<QVector3D> > triangles);    

    static bool _isTriangleAdequate(QVector3D &p, QVector3D &q, QVector3D &r);
    
    QString _filename;
};

#endif	/* IMPORTER_H */

