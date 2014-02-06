/* 
 * File:   FilePicker.h
 * Author: ovol
 *
 * Created on February 6, 2014, 1:46 PM
 */

#ifndef FILEPICKER_H
#define	FILEPICKER_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>

class FilePicker : public QWidget {
    Q_OBJECT
public:
    FilePicker(bool save, QString filters = "*.*");
    virtual ~FilePicker();

    QString selectedPath() const;
signals:
    void updated();

private slots:
    void showFileDialog();
   
private:
    void initWidgets();
    void initSignals();
    
    QPushButton *_btnSelect;
    QLabel *_lblFilename;    
    
    bool _save;
    QString _filters;
    QString _selectedPath;
};

#endif	/* FILEPICKER_H */

