/* 
 * File:   CalculationThread.h
 * Author: seth
 *
 * Created on February 6, 2014, 9:39 PM
 */

#ifndef CALCULATIONTHREAD_H
#define	CALCULATIONTHREAD_H

#include <QThread>

#include "Processor.h"
#include <Qt3D/QTriangle3D>

class CalculationThread : public QThread{
    Q_OBJECT
public:
    CalculationThread(QObject *parent, Processor::PARAMS params,
            QList<QTriangle3D> &model);
    virtual ~CalculationThread();
    
    virtual void run();
    
    QList<Processor::CALCULATION_RESULT> results() const;
    
signals:
    void iterationFinished(int numIteration);

private:
    Processor::PARAMS _params;
    QList<Processor::CALCULATION_RESULT> _results;
    QList<QTriangle3D> _model;
};

#endif	/* CALCULATIONTHREAD_H */

