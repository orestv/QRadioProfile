/*
 * File:   main.cpp
 * Author: ovol
 *
 * Created on February 3, 2014, 3:42 PM
 */
#include <iostream>

#include <QApplication>
#include "import/Importer.h"
#include "processing/Processor.h"
#include "gui/MainWindow.h"
#include <QVector3D>


int main(int argc, char *argv[]) {
   
    // initialize resources, if needed
    // Q_INIT_RESOURCE(resfile);

    QApplication app(argc, argv);

    // create and show your widgets here
    
    MainWindow w;
    w.show();

    return app.exec();
    
//    QVector3D a(5, 1, 1), b(2, 2, 1), c(1, 1, 1);
//    QList<QVector3D> triangle;
//    triangle.push_back(a);
//    triangle.push_back(b);
//    triangle.push_back(c);
//    Importer::generateRightTriangles(triangle);
    
    
    return 0;
    
    QString path = "models/cub.obj";
    QList<RightTriangle> triangles;
    try {
        triangles = Importer::import(path);
    }
    catch (char *exception) {
        std::cout<<"Exception caught: "<<exception;
        return 0;
    }
    double wavelength = 299792458 / 95000000000.;
        
    QVector3D viewpoint(0, 25, 75);
//    QVector3D viewpoint(53.3, 25, 53.3);
    std::cout<<"Processing "<<triangles.length()<<" triangles."<<std::endl;
    QList<RightTriangle> visibleTriangles = Processor::getVisibleTriangles(triangles, viewpoint);
    std::cout<<"Visible triangles count: "<<visibleTriangles.length()<<std::endl;
    Processor::VIEWPOINT_SUMS sums = Processor::calculateViewpointSums(visibleTriangles, viewpoint, wavelength);
    qDebug()<<"Viewpoint sums calculated: "<<sums.sum_cos<<", "<<sums.sum_sin;
    qDebug()<<"E = "<<sqrt(pow(sums.sum_cos, 2) + pow(sums.sum_sin, 2));
}
