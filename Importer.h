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

#include "RightTriangle.h"

class Importer {
public:

    static QList<RightTriangle> import(QString path);
    
private:
    static QList<Point> _read_vertices(QString path);
    static QList<QList<int>> _read_indices(QString path);
    
    static Point _parse_vertex_line(QByteArray line);
    static QList<int> _parse_face_line(QByteArray line);
    static QList<QList<Point>> _generate_faces(QList<Point> points, QList<QList<int>> face_indices);
    static QList<QList<Point>> _generate_triangles(QList<QList<Point>> faces);
    static QList<QList<RightTriangle>> _generate_right_triangles(QList<QList<Point>> triangles);
    
    QString _filename;
};

#endif	/* IMPORTER_H */

