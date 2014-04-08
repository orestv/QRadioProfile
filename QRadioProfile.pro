QT       += core gui widgets

TARGET = RadioProfile
CONFIG   += app_bundle

QMAKE_CXXFLAGS = -std=c++0x

DEFINES += DEBUG_OUTPUT

TEMPLATE = app

LIBS += -L"C:\\Program Files\\GnuWin32\\bin" -lgsl -L"C:\\Program Files\\GnuWin32\\bin" -lgslcblas -lm

INCLUDEPATH += C:\\ "C:\\Program Files\\GnuWin32\\include"

SOURCES += main.cpp \
    geometry/RightTriangle.cpp \
    gui/ParamsWidget.cpp \
    gui/MainWindow.cpp \
    gui/FilePicker.cpp \
    import/Importer.cpp \
    processing/ProcessParams.cpp \
    processing/Processor.cpp \
    processing/CalculationThread.cpp \
    geometry/triangle.cpp \
    processing/ecalculator.cpp

HEADERS += \
    geometry/RightTriangle.h \
    gui/ParamsWidget.h \
    gui/MainWindow.h \
    gui/FilePicker.h \
    import/Importer.h \
    processing/ProcessParams.h \
    processing/Processor.h \
    processing/CalculationThread.h \
    geometry/triangle.h \
    processing/ICalculator.h \
    processing/ecalculator.h

