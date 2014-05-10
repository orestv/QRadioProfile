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
    mdouble wavelength = Processor::LIGHTSPEED / _params.frequency;
    std::cout<<"Start angle: "<<_params.viewpointStartAngle<<", end angle: "<<_params.viewpointEndAngle<<std::endl;
    int iteration = 0;

    for (mdouble viewpointAzimuth = _params.viewpointStartAngle;
            viewpointAzimuth <= _params.viewpointEndAngle;
            viewpointAzimuth += _params.viewpointRotationStep) {
        if (_cancelled)
            return;
        mdouble x, y, z;
        y = _params.viewpointHeight;
        z = _params.viewpointDistance * cos(viewpointAzimuth);
        x = _params.viewpointDistance * sin(viewpointAzimuth);
        
        MVector viewpoint;
        viewpoint<<x, y, z;

        std::complex<mdouble> e = Processor::getE(viewpoint,
                                                      _model,
                                                      wavelength,
                                                      _params.amplitude);

        Processor::CALCULATION_RESULT localResult;
        localResult.azimuth = viewpointAzimuth;
        localResult.E = std::abs(e);
        localResult.eComplex = e;

        _results.push_back(localResult);
        iteration++;
        emit iterationFinished(iteration);
    }
}

QList<Processor::CALCULATION_RESULT> CalculationThread::results() const {
    return _results;
}

void CalculationThread::cancel() {
    _cancelled = true;
    exit();
}
