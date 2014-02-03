/* 
 * File:   Importer.cpp
 * Author: ovol
 * 
 * Created on February 3, 2014, 3:54 PM
 */

#include "Importer.h"
#include <QDebug>
#include <ctype.h>

QList<RightTriangle> Importer::import(QString path) {
    QList<RightTriangle> result;
    
    QList<Point> points = Importer::_read_vertices(path);
    QList<QList<int>> face_indices = Importer::_read_indices(path);
    QList<QList<Point>> faces = Importer::_generate_faces(points, face_indices);
    QList<QList<Point>> triangles = Importer::_generate_triangles(faces);
    
    return result;
}

QList<Point> Importer::_read_vertices(QString path) {
    QList<Point> result;
    QFile file(path);
    if (!file.exists())
        throw "File doesn't exist!";
    if (!file.open(QIODevice::ReadOnly))
        throw "Failed to open file";
    int vertex_count = 0;
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        if (!line.startsWith("v "))
            continue;
        Point pt = _parse_vertex_line(line);
        result.push_back(pt);
        
        vertex_count++;
    }

    qDebug()<<"File opened successfully!";
    file.close();
    qDebug()<<"File closed successfully!";
    qDebug()<<vertex_count<<" vertices imported.";
    return result;
}

QList<QList<int>> Importer::_read_indices(QString path) {
    QList<QList<int>> result;
    QFile file(path);
    if (!file.exists())
        throw "File doesn't exist!";
    if (!file.open(QIODevice::ReadOnly))
        throw "Failed to open file";
    int face_count = 0;
    
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        if (!line.startsWith('f'))
            continue;
        QList<int> face_list = _parse_face_line(line);
        result.push_back(face_list);
        face_count++;
    }
    
    qDebug()<<"File opened successfully!";
    file.close();
    qDebug()<<"File closed successfully!";
    qDebug()<<face_count<<" faces imported.";
    return result;
}

Point Importer::_parse_vertex_line(QByteArray line) {
    while (!isdigit(line[0]))
        line.remove(0, 1);
    line = line.trimmed();
    QList<QByteArray> coordinates = line.split(' ');
    auto iter = coordinates.begin();
    double x, y, z;
    x = (*iter).toDouble();iter++;
    y = (*iter).toDouble();iter++;
    z = (*iter).toDouble();iter++;

    return Point(x, y, z);
}

QList<int> Importer::_parse_face_line(QByteArray line) {
//    qDebug()<<"Parsing face line: "<<line;
    
    QList<int> face_list;
    while (!isdigit(line[0]))
        line.remove(0, 1);
    line = line.trimmed();
    
//    qDebug()<<"Removed 'face' specificator: "<<line;
    
    QList<QByteArray> indices = line.split(' ');
    for (auto iter = indices.begin(); iter != indices.end(); iter++) {
        int first_nondigit_index = 0;
        while (isdigit(iter->at(first_nondigit_index)))
            first_nondigit_index++;
        iter->remove(first_nondigit_index, iter->length());
        
//        qDebug()<<"Removed everything after the first slash: "<<*iter<<line;
                
        int face_index = iter->toInt();

        face_list.push_back(face_index);
    }
    return face_list;
}

QList<QList<Point>> Importer::_generate_faces(QList<Point> points, QList<QList<int> > indices) {
    QList<QList<Point>> faces;
    for (auto face_indices = indices.begin(); face_indices != indices.end(); face_indices++) {
        QList<Point> face_points;
        for (auto index = face_indices->begin(); index != face_indices->end(); index++) {
            if (points.length() <= *index - 1) {
                qDebug()<<"Failed to get vertex with index "<<*index-1;
                continue;
            }
            face_points.push_back(points.at(*index - 1));        
        }
        faces.push_back(face_points);
    }
    return faces;
}

QList<QList<Point>> Importer::_generate_triangles(QList<QList<Point> > faces) {
    QList<QList<Point>> triangles;

    for (auto face = faces.begin(); face != faces.end(); face++) {
        if (face->length() < 3) {
            qDebug()<<"Face has less than 3 vertices; skipped";
            continue;
        }
        if (face->length() > 4) {
            qDebug()<<"Face has "<<face->length()<<" vertices; skipped";
            continue;
        }
        if (face->length() == 3) {
            triangles.push_back(*face);
            continue;
        }
        int triangle_indices_1[] = {0, 1, 2};
        int triangle_indices_2[] = {0, 2, 3};
        QList<Point> triangle_1, triangle_2;
        for (int i = 0; i < 3; i++) {
            triangle_1.push_back(face->at(triangle_indices_1[i]));
            triangle_2.push_back(face->at(triangle_indices_2[i]));
        }
        triangles.push_back(triangle_1);
        triangles.push_back(triangle_2);
    }
    qDebug()<<faces.length()<<" faces converted to "<<triangles.length()<<" triangles.";
    return triangles;
}

QList<QList<RightTriangle>> Importer::_generate_right_triangles(QList<QList<Point> > triangles) {
    QList<QList<RightTriangle>> right_triangles;
    
    return right_triangles;
}