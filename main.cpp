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

    MainWindow w;
    w.show();

    return app.exec();
}
