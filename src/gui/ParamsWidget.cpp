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
    
    QGridLayout *layout = new QGridLayout();
    
    layout->addWidget(new QLabel(tr("Файл моделі")), 0, 0);
    layout->addWidget(_fpModel, 0, 1);
    
    layout->addWidget(new QLabel(tr("Файл результату")), 1, 0);
    layout->addWidget(_fpResult, 1, 1);
    
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