/* 
 * File:   CalculationThread.cpp
 * Author: seth
 * 
 * Created on February 6, 2014, 9:39 PM
 */

#include <QtConcurrent/QtConcurrent>

#include <iostream>
#include "CalculationThread.h"

CalculationThread::CalculationThread(QObject *parent, 
        Processor::PARAMS params,
        QList<Triangle> &model): QThread(parent) {
    this->_params = params;
    this->_model = model;
    this->_cancelled = false;
}

CalculationThread::~CalculationThread() {
}

void CalculationThread::run() {
    long double wavelength = Processor::LIGHTSPEED / _params.frequency;
    std::cout<<"Start angle: "<<_params.viewpointStartAngle<<", end angle: "<<_params.viewpointEndAngle<<std::endl;
    int iteration = 0;
    QList<Processor::E_CALCULATION_PARAMS> paramsList;

    QList<QPair<long double, QFuture<std::complex<long double> > > > futureList;
    for (long double viewpointAzimuth = _params.viewpointStartAngle;
            viewpointAzimuth <= _params.viewpointEndAngle;
            viewpointAzimuth += _params.viewpointRotationStep) {
        if (_cancelled)
            return;
        long double x, y, z;
        y = _params.viewpointHeight;
        z = _params.viewpointDistance * cos(viewpointAzimuth);
        x = _params.viewpointDistance * sin(viewpointAzimuth);
        
        MVector viewpoint;
        viewpoint<<x, y, z;

        std::complex<long double> e = Processor::getE(viewpoint, _model, wavelength);

        Processor::CALCULATION_RESULT localResult;
        localResult.azimuth = viewpointAzimuth;
        localResult.E = std::abs(e);
        localResult.eComplex = e;

        _results.push_back(localResult);
        iteration++;
        emit iterationFinished(iteration);

//        std::cout<<"viewpoint: "<<viewpoint;
//        Processor::E_CALCULATION_PARAMS params = {viewpoint, _model, wavelength};
//        paramsList.append(params);

//        QFuture<std::complex<long double> > future = QtConcurrent::run(Processor::getE, params);
//        futureList.append(QPair<long double, QFuture<std::complex<long double> > >(viewpointAzimuth, future));
    }

//    for (auto future = futureList.begin(); future != futureList.end(); future++) {
//        QPair<long double, QFuture<std::complex<long double> > > p = *future;
//        std::complex<long double> e = p.second;
//        Processor::CALCULATION_RESULT localResult;
//        localResult.azimuth = p.first;
//        localResult.E = std::abs(e);
//        localResult.eComplex = e;

//        _results.push_back(localResult);

//        iteration++;
//        emit iterationFinished(iteration);
//    }

//    std::complex<long double> e = Processor::getE(viewpoint, _model, wavelength);

//    Processor::CALCULATION_RESULT localResult;
//    localResult.azimuth = viewpointAzimuth;
//    localResult.E = std::abs(e);
//    localResult.eComplex = e;

//    _results.push_back(localResult);
//    iteration++;
//    emit iterationFinished(iteration);
}

QList<Processor::CALCULATION_RESULT> CalculationThread::results() const {
    return _results;
}

void CalculationThread::cancel() {
    _cancelled = true;
    exit();
}
