/* 
 * File:   CalculationThread.cpp
 * Author: seth
 * 
 * Created on February 6, 2014, 9:39 PM
 */

#include "CalculationThread.h"

CalculationThread::CalculationThread(QObject *parent, 
        Processor::PARAMS params,
        QList<RightTriangle> &model): QThread(parent) {
    this->_params = params;
    this->_model = model;
}

CalculationThread::~CalculationThread() {
}

void CalculationThread::run() {
    double wavelength = Processor::LIGHTSPEED / _params.frequency;
    int iteration = 0;
    for (double viewpointAzimuth = 0;
            viewpointAzimuth < 2*M_PI;
            viewpointAzimuth += _params.viewpointRotationStep) {
        double x, y, z;
        y = _params.viewpointHeight;
        z = _params.viewpointDistance * cos(viewpointAzimuth);
        x = _params.viewpointDistance * sin(viewpointAzimuth);
        
        QVector3D viewpoint(x, y, z);
//        std::cout<<"viewpoint: "<<viewpoint;
        
        QList<RightTriangle> visibleTriangles = Processor::getVisibleTriangles(
                _model, viewpoint);
        
        Processor::VIEWPOINT_SUMS sums = Processor::calculateViewpointSums(
                visibleTriangles, viewpoint, wavelength);
        
        double E = sqrt(pow(sums.sum_cos, 2) + pow(sums.sum_sin, 2));
        
        Processor::CALCULATION_RESULT localResult;        
        localResult.azimuth = viewpointAzimuth;
        localResult.E = E;
        localResult.sum_cos = sums.sum_cos;
        localResult.sum_sin = sums.sum_sin;
        
        _results.push_back(localResult);
        iteration++;
        emit iterationFinished(iteration);
    }
}

QList<Processor::CALCULATION_RESULT> CalculationThread::results() const {
    return _results;
}
