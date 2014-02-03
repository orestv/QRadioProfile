/*
 * File:   main.cpp
 * Author: ovol
 *
 * Created on February 3, 2014, 3:42 PM
 */
#include <iostream>

#include <QApplication>
#include "Importer.h"

int main(int argc, char *argv[]) {
    // initialize resources, if needed
    // Q_INIT_RESOURCE(resfile);

//    QApplication app(argc, argv);

    // create and show your widgets here

//    return app.exec();
    
    QString path = "/home/seth/dev/pymodelanalyzer/models/t72.obj";
    try {
        QList<RightTriangle> triangles = Importer::import(path);
    }
    catch (char *exception) {
        std::cout<<"Exception caught: "<<exception;
    }
}
