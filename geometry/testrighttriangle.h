#ifndef TESTRIGHTTRIANGLE_H
#define TESTRIGHTTRIANGLE_H

#include <QObject>
#include <QtTest/QtTest>

class TestRightTriangle : public QObject
{
    Q_OBJECT
public:
    explicit TestRightTriangle(QObject *parent = 0);
    
private slots:
    void toUpper();
    
};

#endif // TESTRIGHTTRIANGLE_H
