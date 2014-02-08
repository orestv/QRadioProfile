/* 
 * File:   Importer.cpp
 * Author: ovol
 * 
 * Created on February 3, 2014, 3:54 PM
 */

#include "Importer.h"
#include <QDebug>
#include <QTextStream>

#include <iostream>
#include <math.h>

QList<RightTriangle> Importer::import(QString path) {
    QList<RightTriangle> result;
    
    QList<QVector3D> points = Importer::_read_vertices(path);
    QList<QList<int> > face_indices = Importer::_read_indices(path);
    QList<QList<QVector3D> > faces = Importer::_generate_faces(points, face_indices);
    QList<QList<QVector3D> > triangles = Importer::_generate_triangles(faces);
    result = Importer::_generate_right_triangles(triangles);
    
    return result;
}

QList<QVector3D> Importer::_read_vertices(QString path) {
    QList<QVector3D> result;
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
        QVector3D pt = _parse_vertex_line(line);
        result.push_back(pt);
        
        vertex_count++;
    }

    qDebug()<<"File opened successfully!";
    file.close();
    qDebug()<<"File closed successfully!";
    qDebug()<<vertex_count<<" vertices imported.";
    return result;
}

QList<QList<int> > Importer::_read_indices(QString path) {
    QList<QList<int> > result;
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

QVector3D Importer::_parse_vertex_line(QByteArray line) {
    line.remove(0, 1);
    line = line.trimmed();
    QList<QByteArray> coordinates = line.split(' ');
    auto iter = coordinates.begin();
    double x, y, z;
    x = (*iter).toDouble();iter++;
    y = (*iter).toDouble();iter++;
    z = (*iter).toDouble();iter++;
    
    QVector3D result(x, y, z);

    return QVector3D(x, y, z);
}

QList<int> Importer::_parse_face_line(QByteArray line) {
//    qDebug()<<"Parsing face line: "<<line;
    
    QList<int> face_list;
    while (!isdigit(line[0]))
        line.remove(0, 1);
    line = line.trimmed();
    
    QList<QByteArray> indices = line.split(' ');
    for (auto iter = indices.begin(); iter != indices.end(); iter++) {
        int first_nondigit_index = 0;
        while (isdigit(iter->at(first_nondigit_index)))
            first_nondigit_index++;
        iter->remove(first_nondigit_index, iter->length());
                
        int face_index = iter->toInt();
        face_list.push_back(face_index);
    }
    return face_list;
}

QList<QList<QVector3D>> Importer::_generate_faces(QList<QVector3D> points, QList<QList<int> > indices) {
    QList<QList<QVector3D>> faces;
    for (auto face_indices = indices.begin(); face_indices != indices.end(); face_indices++) {
        QList<QVector3D> face_points;
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

QList<QList<QVector3D>> Importer::_generate_triangles(QList<QList<QVector3D> > faces) {
    QList<QList<QVector3D>> triangles;

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
        QList<QVector3D> triangle_1, triangle_2;
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

QList<RightTriangle> Importer::_generate_right_triangles(QList<QList<QVector3D> > triangles) {
    QList<RightTriangle> right_triangles;
    int skipped_triangles = 0;
    for (auto triangle = triangles.begin(); triangle != triangles.end(); triangle++) {
        QList<RightTriangle> rt = Importer::generateRightTriangles(*triangle);
        if (rt.length() > 0)
            right_triangles.append(rt);
        else
            skipped_triangles++;
    }
    qDebug()<<skipped_triangles<<" triangles skipped.";
    qDebug()<<"Generated "<<right_triangles.length()<<" right triangles.";
    return right_triangles;
}

QList<RightTriangle> Importer::generateRightTriangles(QList<QVector3D> triangle) {
    QVector<QVector3D> t = triangle.toVector();
    QList<RightTriangle> result;
    int side_indices[][3] = { {0, 1, 2}, {1, 2, 0}, {2, 0, 1} };
    double max_length = 0;
    int max_length_index = -1;
    for (int i = 0; i < 3; i++) {
        int *side_i = side_indices[i];
        QVector3D a, b, c;
        a = t[side_i[0]];
        b = t[side_i[1]];
        c = t[side_i[2]];
        QVector3D side = b - a;
        if (side.length() < 0.01) {        //One of the sides is too short
            return result;
        }
        if (abs(QVector3D::dotProduct(b - a, c - a)) < 0.01) {
            result.push_back(RightTriangle(a, b, c));
            return result;
        }
        if (side.lengthSquared() > max_length) {
            max_length = side.lengthSquared();
            max_length_index = i;
        }
    }
    int *ml_side_indices = side_indices[max_length_index];
    QVector3D a = t[ml_side_indices[0]], 
            b = t[ml_side_indices[1]],
            c = t[ml_side_indices[2]];
    
    QVector3D hippotenuse = c - a;
    QVector3D longSide = b - a;
    
    float dotProduct = QVector3D::dotProduct(hippotenuse, longSide);
    float angleCos = dotProduct / (hippotenuse.length() * longSide.length());
    
    QVector3D right_angle_vertex = longSide.normalized() * 
            hippotenuse.length() * angleCos + a;
    
    RightTriangle rt1(right_angle_vertex, c, a),
            rt2(right_angle_vertex, b, c);
    
    result.push_back(rt1);
    result.push_back(rt2);
    return result;
}

void Importer::exportToFile(
        QString path, 
        QList<Processor::CALCULATION_RESULT> results) {
    QFile file(path);
    if (file.exists())
        file.remove();
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        throw "Failed to open file";
    QTextStream strm(&file);
    strm.setCodec("UTF-8");
    strm<<QString::fromUtf8("Азимут;Е;Сума косинусів;Сума синусів\n");
    for (auto result = results.begin(); result != results.end(); result++) {
        strm<<QString::number(result->azimuth, 'f')<<";"
                <<QString::number(result->E, 'f')<<";"
                <<QString::number(result->sum_cos, 'f')<<";"
                <<QString::number(result->sum_sin, 'f')
                <<"\n";
    }
    
    strm.flush();
    file.close();
}
