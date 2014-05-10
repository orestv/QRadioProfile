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
#include <QGridLayout>

#include "FilePicker.h"

#include "geometry/mtypes.h"

class ParamsWidget : public QWidget{
    Q_OBJECT
public:
    
    struct CALCULATION_PARAMS {
        QString inputPath;
        QString resultPath;
        mdouble viewpointHeight;
        mdouble viewpointDistance;
        mdouble viewpointRotationStep;       //degrees
        mdouble viewpointStartAngle;
        mdouble viewpointEndAngle;
        mdouble frequency;                   //GHz
        mdouble amplitude;
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

    void addWidget(QGridLayout &layout, const QString labelText,
                   QWidget *widget);
    
    FilePicker *_fpModel;
    FilePicker *_fpResult;
    QDoubleSpinBox *_spViewpointHeight;
    QDoubleSpinBox *_spViewpointDistance;
    QDoubleSpinBox *_spViewpointRotationStep;
    QDoubleSpinBox *_spViewpointRotationStartAngle;
    QDoubleSpinBox *_spViewpointRotationEndAngle;
    QDoubleSpinBox *_spFrequency;
    QDoubleSpinBox *_spAmplitude;
};

#endif	/* PARAMSWIDGET_H */

