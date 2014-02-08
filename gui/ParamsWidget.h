/* 
 * File:   ParamsWidget.h
 * Author: ovol
 *
 * Created on February 6, 2014, 1:43 PM
 */

#ifndef PARAMSWIDGET_H
#define	PARAMSWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QDoubleSpinBox>

#include "FilePicker.h"

class ParamsWidget : public QWidget{
    Q_OBJECT
public:
    
    struct CALCULATION_PARAMS {
        QString inputPath;
        QString resultPath;
        double viewpointHeight;
        double viewpointDistance;
        double viewpointRotationStep;       //degrees
        double frequency;                   //GHz
    };    
    
    ParamsWidget();
    virtual ~ParamsWidget();
    
    bool isReady() const;
    CALCULATION_PARAMS gatherParams() const;

signals:
    void updated();

private slots:
    void filePickerUpdated();

private:
    void initWidgets();
    void initSignals();
    
    FilePicker *_fpModel;
    FilePicker *_fpResult;
    QDoubleSpinBox *_spViewpointHeight;
    QDoubleSpinBox *_spViewpointDistance;
    QDoubleSpinBox *_spViewpointRotationStep;
    QDoubleSpinBox *_spFrequency;
};

#endif	/* PARAMSWIDGET_H */

