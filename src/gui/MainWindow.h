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

class MainWindow : public QWidget{
    Q_OBJECT
public:
    MainWindow();
    virtual ~MainWindow();
    
private slots:
    void paramsWidgetUpdated();
    
private:
    void initWidgets();
    void initSignals();
    
    void updateWidgetsReadiness();
    
    ParamsWidget *_paramsWidget;
    QPushButton *_btnCalculate;    
};

#endif	/* MAINWINDOW_H */
