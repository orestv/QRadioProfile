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
#include "ParamsWidget.h"
#include "processing/Processor.h"

class MainWindow : public QWidget{
    Q_OBJECT
public:
    MainWindow();
    virtual ~MainWindow();
    
private slots:
    void paramsWidgetUpdated();
    void calculateClicked();
    
private:
    void initWidgets();
    void initSignals();
    
    void beginCalculation();
    
    void updateWidgetsReadiness();
    static Processor::PARAMS convertParams(ParamsWidget::CALCULATION_PARAMS);
    
    ParamsWidget *_paramsWidget;
    QPushButton *_btnCalculate;    
};

#endif	/* MAINWINDOW_H */
