/*
 * File:   main.cpp
 * Author: ovol
 *
 * Created on February 3, 2014, 3:42 PM
 */
#include <iostream>

#include <QApplication>
#include "gui/MainWindow.h"
#include <QDebug>

QList<double> convertToDoubles(QString &sourceString) {
    auto coords = sourceString.remove("(").remove(")").split(";");
    QList<double> values;
    bool ok;
    for (auto s = coords.begin(); s != coords.end(); s++) {
        values<<s->toDouble(&ok);
        if (!ok) {
            QString error;
            error = "Error converting " + *s + " into a number";
            throw error;
        }
    }
    return values;
}

Triangle getTriangle(QString &sourceString) {
    QList<Vector3d> vectors;
    QList<QString> vertices = sourceString.split(",");
    for (auto vertexString = vertices.begin(); vertexString != vertices.end(); vertexString++) {
        QList<double> values = convertToDoubles(*vertexString);
        Vector3d vertex;
        vertex<<values[0], values[1], values[2];
        vectors.push_back(vertex);
    }

    return Triangle(vectors[0], vectors[1], vectors[2]);
}

Vector3d getViewpoint(QString &sourceString) {
    qDebug()<<sourceString;
    Vector3d result;
    QList<double> values = convertToDoubles(sourceString);
    if (values.length() != 3)
        throw QString("Error converting " + sourceString + " into a vector");
    result<<values[0], values[1], values[2];
    return result;
}

void printVector(const Vector3d &vector) {
    for (int i = 0; i < 3; i++)
        std::cout<<vector.data()[i]<<"\t";
    std::cout<<std::endl;
}


int main(int argc, char *argv[]) {

    // initialize resources, if needed
    // Q_INIT_RESOURCE(resfile);

    QApplication app(argc, argv);

#ifndef DEBUG_OUTPUT
    QStringList args = app.arguments();

    MainWindow w;
    w.show();

    return app.exec();
#else
    QStringList args = app.arguments();
    if (args.length() == 1) {
        MainWindow w;
        w.show();

        return app.exec();
    }
    QString optTriangle = args[1];
    QString optViewpoint = args[2];
    QString optFrequency = args[3];

    Triangle globalTriangle;
    Vector3d globalViewpoint;
    double frequency;

    try {
        globalTriangle = getTriangle(optTriangle);
        globalViewpoint = getViewpoint(optViewpoint);
        frequency = optFrequency.toDouble();
    }
    catch (const QString error) {
        qDebug()<<"Виникла помилка:"<<error;
        return 0;
    }
    double wavelength = Processor::LIGHTSPEED / frequency;
    if (!Processor::isTriangleVisible(globalTriangle, QList<Triangle>(), globalViewpoint)) {
        std::cout<<"Triangle is invisible from this viewpoint, exiting..."<<std::endl;
        return 0;
    }

    std::complex<double> e = Processor::getE0(globalViewpoint, globalTriangle, wavelength);
#endif
}
