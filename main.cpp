/*
 * File:   main.cpp
 * Author: ovol
 *
 * Created on February 3, 2014, 3:42 PM
 */
#include <iostream>

#include <QApplication>
#include "Importer.h"
#include <QVector3D>

int __main(int argc, char *argv[]) {
    // initialize resources, if needed
    // Q_INIT_RESOURCE(resfile);

//    QApplication app(argc, argv);

    // create and show your widgets here

//    return app.exec();
    
//    QVector3D a(5, 1, 1), b(2, 2, 1), c(1, 1, 1);
//    QList<QVector3D> triangle;
//    triangle.push_back(a);
//    triangle.push_back(b);
//    triangle.push_back(c);
//    Importer::generateRightTriangles(triangle);
    
    QString path = "/home/seth/dev/pymodelanalyzer/models/t72.obj";
    try {
        QList<RightTriangle> triangles = Importer::import(path);
    }
    catch (char *exception) {
        std::cout<<"Exception caught: "<<exception;
    }
}
