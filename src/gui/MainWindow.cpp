/* 
 * File:   MainWindow.cpp
 * Author: ovol
 * 
 * Created on February 6, 2014, 1:41 PM
 */

#include "MainWindow.h"
#include <QVBoxLayout>

MainWindow::MainWindow() {
    setWindowTitle(tr("Аналіз моделі"));
    initWidgets();
    initSignals();
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
    
}