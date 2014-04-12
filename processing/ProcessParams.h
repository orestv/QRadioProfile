/* 
 * File:   ProcessParams.h
 * Author: seth
 *
 * Created on February 3, 2014, 10:02 PM
 */

#ifndef PROCESSPARAMS_H
#define	PROCESSPARAMS_H

#include <QVector3D>

class ProcessParams {
public:
    ProcessParams();
    ProcessParams(const ProcessParams& orig);
    virtual ~ProcessParams();
private:
    
    long double _wavelength;
    QVector3D _viewpoint;
    

};

#endif	/* PROCESSPARAMS_H */

