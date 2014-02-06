/* 
 * File:   MainWindow.h
 * Author: ovol
 *
 * Created on February 6, 2014, 1:41 PM
 */

#ifndef MAINWINDOW_H
#define	MAINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QProgressBar>
#include "ParamsWidget.h"
#include "processing/Processor.h"
#include "processing/CalculationThread.h"

class MainWindow : public QWidget{
    Q_OBJECT
public:
    MainWindow();
    virtual ~MainWindow();
    
private slots:
    void paramsWidgetUpdated();
    void calculateClicked();
    void threadIterationFinished(int iteration);
    void threadFinished();
    
private:
    void initWidgets();
    void initSignals();
    
    void beginCalculation();
    
    void updateWidgetsReadiness();
    static Processor::PARAMS convertParams(ParamsWidget::CALCULATION_PARAMS);
    
    ParamsWidget *_paramsWidget;
    QProgressBar *_progressBar;
    QPushButton *_btnCalculate;    
    CalculationThread *_calculationThread;
};

#endif	/* MAINWINDOW_H */
