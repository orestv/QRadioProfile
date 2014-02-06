/* 
 * File:   MainWindow.cpp
 * Author: ovol
 * 
 * Created on February 6, 2014, 1:41 PM
 */

#include "MainWindow.h"

#include <QVBoxLayout>
#include <QDebug>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>

#include <math.h>

#include "processing/Processor.h"
#include "import/Importer.h"
#include "geometry/RightTriangle.h"

MainWindow::MainWindow() {
    setWindowTitle(tr("Аналіз моделі"));
    initWidgets();
    initSignals();
    
    updateWidgetsReadiness();
}

MainWindow::~MainWindow() {
}

void MainWindow::initWidgets() {
    _paramsWidget = new ParamsWidget();
    _btnCalculate = new QPushButton(tr("Обчислити"));
    
    QVBoxLayout *layout = new QVBoxLayout();
    
    layout->addWidget(_paramsWidget);
    layout->addStretch();
    layout->addWidget(_btnCalculate);
    
    setLayout(layout);
}

void MainWindow::initSignals() {
    QObject::connect(_paramsWidget, &ParamsWidget::updated,
            this, &MainWindow::paramsWidgetUpdated);
    QObject::connect(_btnCalculate, &QPushButton::clicked, 
            this, &MainWindow::calculateClicked);
}

void MainWindow::paramsWidgetUpdated() {
    updateWidgetsReadiness();
}

void MainWindow::updateWidgetsReadiness() {
    bool paramsReady = _paramsWidget->isReady();
    _btnCalculate->setEnabled(paramsReady);
}

void MainWindow::calculateClicked() {
    beginCalculation();    
}

Processor::PARAMS
MainWindow::convertParams(ParamsWidget::CALCULATION_PARAMS params) {
    Processor::PARAMS result;
    
    result.viewpointHeight = params.viewpointHeight;
    result.viewpointDistance = params.viewpointDistance;
    result.frequency = params.frequency * pow(10, 9);
    result.viewpointRotationStep = params.viewpointRotationStep * M_PI / 180.;
    
    return result;
}

void MainWindow::beginCalculation() {
    ParamsWidget::CALCULATION_PARAMS inputParams = 
            _paramsWidget->gatherParams();    
    Processor::PARAMS calculationParams = 
            MainWindow::convertParams(inputParams);
    QList<RightTriangle> model;
    try {
        model = Importer::import(inputParams.inputPath);
    }
    catch (char *exceptionMessage) {
        qDebug()<<exceptionMessage;
        return;
    }
    QList<Processor::CALCULATION_RESULT> result = Processor::analyzeModel(model, calculationParams);
    qDebug()<<"Calculation complete, items count: "<<result.length();
    for (auto item = result.begin(); item != result.end() && false; item++) {
        double azimuthDeg = item->azimuth * 180 / M_PI;
        qDebug()<<"For azimuth "<<azimuthDeg<<" e == "<<item->E;
    }
    Importer::exportToFile(inputParams.resultPath, result);
    qDebug()<<"File saved";
    QMessageBox::StandardButton response = QMessageBox::question(this, tr("Модель проаналізовано!"),
            tr("Результати збережено. Бажаєте відкрити файл?"),
            QMessageBox::StandardButton::Yes|QMessageBox::StandardButton::No,
            QMessageBox::StandardButton::Yes);
    if (response == QMessageBox::StandardButton::Yes)
        QDesktopServices::openUrl(QUrl(inputParams.resultPath));        
}