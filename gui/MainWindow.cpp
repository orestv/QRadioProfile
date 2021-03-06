/* 
 * File:   MainWindow.cpp
 * Author: ovol
 * 
 * Created on February 6, 2014, 1:41 PM
 */

#include "MainWindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>

#include <math.h>

#include "processing/Processor.h"
#include "import/Importer.h"
#include "geometry/RightTriangle.h"
#include "processing/CalculationThread.h"

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
    _progressBar = new QProgressBar();
    _btnCancel = new QPushButton(tr("Відмінити"));
    _btnCancel->setEnabled(true);

    QVBoxLayout *layout = new QVBoxLayout();
    QHBoxLayout *progressLayout = new QHBoxLayout();

    progressLayout->addWidget(_progressBar);
    progressLayout->addWidget(_btnCancel);
    
    layout->addWidget(_paramsWidget);
    layout->addStretch();
    layout->addLayout(progressLayout);
    layout->addWidget(_btnCalculate);    
    
    setLayout(layout);
    
    _progressBar->setHidden(true);
    _btnCancel->setHidden(true);
}

void MainWindow::initSignals() {
    QObject::connect(_paramsWidget, &ParamsWidget::updated,
            this, &MainWindow::paramsWidgetUpdated);
    QObject::connect(_btnCalculate, &QPushButton::clicked, 
            this, &MainWindow::calculateClicked);
    QObject::connect(_btnCancel, &QPushButton::clicked,
                     this, &MainWindow::btnCancelClicked);
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
MainWindow::convertParams(
    ParamsWidget::CALCULATION_PARAMS params) {
    Processor::PARAMS result;
    
    result.viewpointStartAngle = params.viewpointStartAngle * M_PI / 180;
    result.viewpointEndAngle = params.viewpointEndAngle * M_PI / 180;
    result.viewpointHeight = params.viewpointHeight;
    result.viewpointDistance = params.viewpointDistance;
    result.frequency = params.frequency * pow(10, 9);
    result.viewpointRotationStep = params.viewpointRotationStep * M_PI / 180.;
    result.amplitude =  params.amplitude;
    
    return result;
}

void MainWindow::beginCalculation() {
    ParamsWidget::CALCULATION_PARAMS inputParams = 
            _paramsWidget->gatherParams();    
    Processor::PARAMS calculationParams = 
            MainWindow::convertParams(inputParams);
    QList<Triangle> model;
    try {
        model = Importer::import(inputParams.inputPath);
    }
    catch (char const *exceptionMessage) {
        QMessageBox::critical(this, tr("Помилка"), tr("Не вдалось відкрити файл моделі!"));
        return;
    }
    int iterations = (calculationParams.viewpointEndAngle - calculationParams.viewpointStartAngle) /
            calculationParams.viewpointRotationStep + 1;
    _progressBar->setValue(0);
    _progressBar->setMaximum(iterations);
    _calculationThread = new CalculationThread(this, calculationParams, model);
    
    QObject::connect(_calculationThread, &CalculationThread::iterationFinished,
            this, &MainWindow::threadIterationFinished);
    QObject::connect(_calculationThread, &CalculationThread::finished,
            this, &MainWindow::threadFinished);
   
    this->_paramsWidget->setDisabled(true);
    this->_btnCalculate->setDisabled(true);
    this->_progressBar->setVisible(true);
    this->_btnCancel->setVisible(true);
    this->_btnCancel->setEnabled(true);
    
    _calculationThread->start();
}

void MainWindow::threadIterationFinished(int iteration) {
    _progressBar->setValue(iteration);
}

void MainWindow::threadFinished() {
    this->_progressBar->setVisible(false);
    this->_btnCancel->setVisible(false);
    ParamsWidget::CALCULATION_PARAMS inputParams = 
            _paramsWidget->gatherParams();   
    bool retry = true, success = false;
    while (retry && !success) {
        try {
            Importer::exportToFile(inputParams.resultPath, _calculationThread->results());
            success = true;
        } catch (char const *exceptionMessage) {
            QMessageBox::critical(this, tr("Помилка"), tr("Не вдалось зберегти файл результату. Спробувати ще раз?"));
            QMessageBox::StandardButton response = QMessageBox::question(this, tr("Модель проаналізовано!"),
                    tr("Спробувати запис в файл ще раз??"),
                    QMessageBox::StandardButton::Yes|QMessageBox::StandardButton::No,
                    QMessageBox::StandardButton::Yes);
            retry = (response == QMessageBox::StandardButton::Yes);
        }
    }

    qDebug()<<"File saved";
    QMessageBox::StandardButton response = QMessageBox::question(this, tr("Модель проаналізовано!"),
            tr("Результати збережено. Бажаєте відкрити файл?"),
            QMessageBox::StandardButton::Yes|QMessageBox::StandardButton::No,
            QMessageBox::StandardButton::Yes);
    if (response == QMessageBox::StandardButton::Yes)
        QDesktopServices::openUrl(QUrl(inputParams.resultPath));
    this->_paramsWidget->setDisabled(false);
    this->_btnCalculate->setDisabled(false);
}

void MainWindow::btnCancelClicked() {
    if (_calculationThread)
        _calculationThread->cancel();
    _btnCancel->setDisabled(true);
}
