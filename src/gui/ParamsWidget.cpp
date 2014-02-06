/* 
 * File:   ParamsWidget.cpp
 * Author: ovol
 * 
 * Created on February 6, 2014, 1:43 PM
 */

#include "ParamsWidget.h"
#include <QGridLayout>
#include <QLabel>

ParamsWidget::ParamsWidget() {
    initWidgets();
    initSignals();
}

ParamsWidget::~ParamsWidget() {
}

void ParamsWidget::initWidgets() {
    _fpModel = new FilePicker(false, "Файли моделі (*.obj)");
    _fpResult = new FilePicker(true, "*.csv");

    _spViewpointHeight = new QDoubleSpinBox();
    _spViewpointDistance = new QDoubleSpinBox();
    _spViewpointRotationStep = new QDoubleSpinBox();
    _spFrequency = new QDoubleSpinBox();
    
    _spViewpointHeight->setMinimum(0);
    _spViewpointHeight->setValue(25);
    
    _spViewpointDistance->setMinimum(0);
    _spViewpointDistance->setValue(75);
    
    _spViewpointRotationStep->setMinimum(-360);
    _spViewpointRotationStep->setMaximum(360);
    _spViewpointRotationStep->setSingleStep(0.05);
    _spViewpointRotationStep->setValue(0.1);
    
    _spFrequency->setMinimum(0);
    _spFrequency->setSingleStep(0.1);
    _spFrequency->setValue(95);
    
#ifdef QT_DEBUG
    _fpModel->setSelectedPath("models/cub.obj");
    _fpResult->setSelectedPath("output.csv");
    _spViewpointRotationStep->setValue(45);
#endif
    
    QGridLayout *layout = new QGridLayout();
    
    layout->addWidget(new QLabel(tr("Файл моделі")), 0, 0);
    layout->addWidget(_fpModel, 0, 1);
    
    layout->addWidget(new QLabel(tr("Файл результату")), 1, 0);
    layout->addWidget(_fpResult, 1, 1);
    
    layout->addWidget(new QLabel(tr("Висота ТС, м")), 2, 0);
    layout->addWidget(_spViewpointHeight, 2, 1);
    
    layout->addWidget(new QLabel(tr("Відстань між ТС і центром координат, м")), 3, 0);
    layout->addWidget(_spViewpointDistance, 3, 1);
    
    layout->addWidget(new QLabel(tr("Крок повороту напрямку на ТС, °")), 4, 0);
    layout->addWidget(_spViewpointRotationStep, 4, 1);
    
    layout->addWidget(new QLabel(tr("Частота скануючого сигналу, ГГц")), 5, 0);
    layout->addWidget(_spFrequency, 5, 1);
    
    setLayout(layout);
}

void ParamsWidget::initSignals() {
    QObject::connect(_fpModel, &FilePicker::updated,
            this, &ParamsWidget::filePickerUpdated);
    QObject::connect(_fpResult, &FilePicker::updated,
            this, &ParamsWidget::filePickerUpdated);
}

void ParamsWidget::filePickerUpdated() {
    emit updated();
}

bool ParamsWidget::isReady() const {
    return (!_fpModel->selectedPath().isEmpty() && 
            !_fpResult->selectedPath().isEmpty());
}

ParamsWidget::CALCULATION_PARAMS
ParamsWidget::gatherParams() const {
    CALCULATION_PARAMS result;
    
    result.inputPath = _fpModel->selectedPath();
    result.resultPath = _fpResult->selectedPath();
    result.viewpointHeight = _spViewpointHeight->value();
    result.viewpointDistance = _spViewpointDistance->value();
    result.viewpointRotationStep = _spViewpointRotationStep->value();
    result.frequency = _spFrequency->value();
    
    return result;
}