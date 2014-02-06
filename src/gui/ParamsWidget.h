/* 
 * File:   ParamsWidget.h
 * Author: ovol
 *
 * Created on February 6, 2014, 1:43 PM
 */

#ifndef PARAMSWIDGET_H
#define	PARAMSWIDGET_H

#include <QWidget>

#include "FilePicker.h"

class ParamsWidget : public QWidget{
    Q_OBJECT
public:
    ParamsWidget();
    virtual ~ParamsWidget();
private:
    void initWidgets();
    void initSignals();
    
    FilePicker *_fpModel;
    FilePicker *_fpResult;
};

#endif	/* PARAMSWIDGET_H */

