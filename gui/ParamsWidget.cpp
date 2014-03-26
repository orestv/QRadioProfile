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
    _spViewpointRotationStartAngle = new QDoubleSpinBox();
    _spViewpointRotationEndAngle = new QDoubleSpinBox();
    _spFrequency = new QDoubleSpinBox();
    
    _spViewpointHeight->setMinimum(0);
    _spViewpointHeight->setValue(25);
    
    _spViewpointDistance->setMinimum(0);
    _spViewpointDistance->setValue(75);
    
    _spViewpointRotationStep->setMinimum(0);
    _spViewpointRotationStep->setMaximum(1);
    _spViewpointRotationStep->setSingleStep(0.001);
    _spViewpointRotationStep->setValue(0.1);

    _spViewpointRotationStartAngle->setMinimum(0);
    _spViewpointRotationStartAngle->setMaximum(360);
    _spViewpointRotationStartAngle->setSingleStep(0.001);
    _spViewpointRotationStartAngle->setValue(0);

    _spViewpointRotationEndAngle->setMinimum(0);
    _spViewpointRotationEndAngle->setMaximum(360);
    _spViewpointRotationEndAngle->setSingleStep(0.001);
#ifdef QT_DEBUG
    _spViewpointRotationEndAngle->setValue(0);
#else
    _spViewpointRotationEndAngle->setValue(360);
#endif
    
    _spFrequency->setMinimum(0);
    _spFrequency->setSingleStep(0.1);
    _spFrequency->setValue(95);
    
#ifdef QT_DEBUG
    _fpModel->setSelectedPath("/home/seth/dev/QRadioProfile/models/cub.obj");
    _fpResult->setSelectedPath("/home/seth/dev/QRadioProfile/output.csv");
    _spViewpointRotationStep->setValue(45);
#endif
    
    QGridLayout *layout = new QGridLayout();
    
    addWidget(*layout, tr("Файл моделі"), _fpModel);
    addWidget(*layout, tr("Файл результату"), _fpResult);
    addWidget(*layout, tr("Висота ТС, м"), _spViewpointHeight);
    addWidget(*layout, tr("Відстань по горизонталі між ТС і центром координат, м"), _spViewpointDistance);
    addWidget(*layout, tr("Крок повороту напрямку на ТС, °"), _spViewpointRotationStep);
    addWidget(*layout, tr("Початковий азимут ТС, °"), _spViewpointRotationStartAngle);
    addWidget(*layout, tr("Кінцевий азимут ТС, °"), _spViewpointRotationEndAngle);
    addWidget(*layout, tr("Частота скануючого сигналу, ГГц"), _spFrequency);

    setLayout(layout);
}

void
ParamsWidget::addWidget(
        QGridLayout &layout,
        const QString labelText,
        QWidget *widget){

    int newRowIndex = layout.rowCount();
    layout.addWidget(new QLabel(labelText), newRowIndex, 0);
    layout.addWidget(widget, newRowIndex, 1);
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
    result.viewpointStartAngle = _spViewpointRotationStartAngle->value();
    result.viewpointEndAngle = _spViewpointRotationEndAngle->value();
    result.frequency = _spFrequency->value();
    
    return result;
}
