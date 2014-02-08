QT       += core gui widgets

TARGET = RadioProfile
CONFIG   += app_bundle

QMAKE_CXXFLAGS = -std=c++0x

TEMPLATE = app


SOURCES += main.cpp \
    geometry/RightTriangle.cpp \
    gui/ParamsWidget.cpp \
    gui/MainWindow.cpp \
    gui/FilePicker.cpp \
    import/Importer.cpp \
    processing/ProcessParams.cpp \
    processing/Processor.cpp \
    processing/CalculationThread.cpp

HEADERS += \
    geometry/RightTriangle.h \
    gui/ParamsWidget.h \
    gui/MainWindow.h \
    gui/FilePicker.h \
    import/Importer.h \
    processing/ProcessParams.h \
    processing/Processor.h \
    processing/CalculationThread.h

