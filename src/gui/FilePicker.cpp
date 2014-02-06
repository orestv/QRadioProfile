/* 
 * File:   FilePicker.cpp
 * Author: ovol
 * 
 * Created on February 6, 2014, 1:46 PM
 */

#include "FilePicker.h"

#include <QHBoxLayout>
#include <QDebug>
#include <QFileInfo>

FilePicker::FilePicker(bool save, QString filters) {
    this->_save = save;
    this->_filters = filters;
    initWidgets();
    initSignals();
}

FilePicker::~FilePicker() {
}

void FilePicker::initWidgets() {
    _btnSelect = new QPushButton(tr("Вибрати..."));
    _lblFilename = new QLabel(tr("N/A"));
    
    QHBoxLayout *layout = new QHBoxLayout();
    
    layout->addWidget(_lblFilename);
    layout->addStretch();
    layout->addWidget(_btnSelect);
    
    setLayout(layout);
}

void FilePicker::initSignals() {
    QObject::connect(_btnSelect, &QPushButton::clicked,
            this, &FilePicker::showFileDialog);
}

void FilePicker::showFileDialog() {
    QString selectedPath;
    if (_save)
        selectedPath = QFileDialog::getSaveFileName(this,
            tr("Виберіть файл"), _selectedPath, this->_filters);
    else {
        QFileDialog fd;
        selectedPath = QFileDialog::getOpenFileName(this,
            tr("Виберіть файл"), _selectedPath, this->_filters);
    }
    if (selectedPath.isNull())
        return;    
    _selectedPath = selectedPath;
    QFileInfo fileInfo(selectedPath);
    _lblFilename->setText(fileInfo.fileName());
    emit updated();
}