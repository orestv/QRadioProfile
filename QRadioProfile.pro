QT       += core gui widgets

TARGET = RadioProfile
CONFIG   += app_bundle console

QMAKE_CXXFLAGS = -std=c++0x

DEFINES += DEBUG_OUTPUT

TEMPLATE = app

SOURCES += main.cpp \
    geometry/RightTriangle.cpp \
    gui/ParamsWidget.cpp \
    gui/MainWindow.cpp \
    gui/FilePicker.cpp \
    import/Importer.cpp \
    processing/Processor.cpp \
    processing/CalculationThread.cpp \
    geometry/triangle.cpp

HEADERS += \
    geometry/RightTriangle.h \
    gui/ParamsWidget.h \
    gui/MainWindow.h \
    gui/FilePicker.h \
    import/Importer.h \
    processing/Processor.h \
    processing/CalculationThread.h \
    geometry/triangle.h \
    geometry/mtypes.h

