/* 
 * File:   test_processor.cpp
 * Author: seth
 *
 * Created on Feb 3, 2014, 10:10:08 PM
 */

#include <stdlib.h>
#include <iostream>
#include <qt5/QtGui/qvector3d.h>
#include "Processor.h"
#include <QList>

/*
 * Simple C++ Test Suite
 */

void testGetVisibleTriangles() {
    QList<RightTriangle> triangles;
    QVector3D viewpoint;
    QList<RightTriangle> result = Processor::getVisibleTriangles(triangles, viewpoint);
    if (true /*check result*/) {
        std::cout << "%TEST_FAILED% time=0 testname=testGetVisibleTriangles (test_processor) message=error message sample" << std::endl;
    }
}

void testIsTriangleVisible() {
    RightTriangle triangle(QVector3D(0, 0, 0),
            QVector3D(1, 0, 0),
            QVector3D(0, 1, 0));
    QVector3D viewpoint(1, 1, 1);
    
    bool result = Processor::isTriangleVisible(triangle, viewpoint);    
    if (!result)    
        std::cout << "%TEST_FAILED% This triangle should have been visible!" << std::endl;
    result = Processor::isTriangleVisible(triangle, QVector3D(1, 1, -1));
    if (result)
        std::cout << "%TEST_FAILED% This triangle should have been hidden!" << std::endl;
}

int _main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% test_processor" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% testGetVisibleTriangles (test_processor)" << std::endl;
    testGetVisibleTriangles();
    testIsTriangleVisible();
    std::cout << "%TEST_FINISHED% time=0 testGetVisibleTriangles (test_processor)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

