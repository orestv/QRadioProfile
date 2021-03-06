/*
 * File:   Processor.cpp
 * Author: seth
 *
 * Created on February 3, 2014, 10:02 PM
 */

#include <QList>
#include <QDebug>
#include <iostream>
#include <complex>
#include "geometry/mtypes.h"

#include "Processor.h"
#include <math.h>

#ifndef M_PIl
#define M_PIl M_PI
#endif

using namespace std;

MVector
Processor::projectOntoPlane(
        const MVector &point,
        MVector plane_normal) {

    plane_normal.normalize();

    MVector projection_onto_normal = plane_normal *
            plane_normal.dot(point);

    return point - projection_onto_normal;
}

bool
Processor::isTriangleVisible(
        const Triangle &triangle,
        const QList<Triangle> &model,
        const MVector &viewPoint) {

    MVector viewVector = (viewPoint - triangle.center());
//    cout<<"View MVector: "<<viewVector<<endl<<endl;
    viewVector.normalize();


    mdouble dot = viewVector.dot(triangle.faceNormal());


//    cout<<"Dot: "<<dot<<endl;
    if (dot <= 0 || isnan(dot))
        return false;

    return true;
}

complex<mdouble>
Processor::getEs(
        const MVector &viewpoint,
        const Triangle &triangle,
        const mdouble wavelength,
        const mdouble amplitude) {

    MMatrix basis = getCoordinatesTransformationMatrix(triangle);

    Eigen::FullPivLU<MMatrix> basisLU(basis);

    MVector eCenter = - Processor::switchCoordinates(triangle.center(), basisLU);

    MVector newViewpoint = Processor::switchCoordinates(viewpoint, basisLU, eCenter);
    MVector p, q, r;
    p = Processor::switchCoordinates(triangle.p(), basisLU, eCenter);
    q = Processor::switchCoordinates(triangle.q(), basisLU, eCenter);
    r = Processor::switchCoordinates(triangle.r(), basisLU, eCenter);
    Triangle newTriangle(p, q, r);

    mdouble x1,y1, x2,y2, x3,y3;
    mdouble xc = newViewpoint[0],
           yc = newViewpoint[1],
           zc = newViewpoint[2];

    if (abs(xc) < 0.000001)
        xc = 0.000001;
    if (abs(yc) < 0.000001)
        yc = 0.000001;

//    cout<<"P:"<<endl<<newTriangle.p()<<endl<<endl;
//    cout<<"Q:"<<endl<<newTriangle.q()<<endl<<endl;
//    cout<<"R:"<<endl<<newTriangle.r()<<endl<<endl;

    MVector leftMost, rightMost, middle;
    leftMost = newTriangle.leftMost();
    rightMost = newTriangle.rightMost();
    middle = newTriangle.middle();

    x1 = rightMost[0], y1 = rightMost[1];
    x2 = leftMost[0], y2 = leftMost[1];
    x3 = middle[0], y3 = middle[1];

    mdouble longestBisect = 0;
    for (int i = 0; i < 3; i++) {
        mdouble bisectLength = Processor::getAngleBisectionLength(newTriangle, i);
        longestBisect = max(bisectLength, longestBisect);
    }

    if (zc / 10 > M_PIl * longestBisect * longestBisect / wavelength) {
#ifdef DEBUG_OUTPUT
        cout<<"z == "<<zc<<endl;
        cout<<"pi*a^2/lambda = "<<M_PIl * longestBisect * longestBisect / wavelength<<endl;
        cout<<"Skipping this triangle."<<endl<<endl;
#endif
        return complex<mdouble>(0, 0);
    }

    complex<mdouble> e_0(0,
                (-1/16.)*(
                -x3*y2-y1*x2+y1*x3+x1*y2+y3*x2-x1*y3)*
                wavelength * amplitude);

    mdouble e_1_1_1 = (xc*x2-xc*x3+yc*y2-yc*y3);
    complex<mdouble> e_1_1_2_imag(0, 2*M_PIl * (
                2*zc*zc + xc*xc*zc + yc*yc*zc - 2*xc*x1 - 2*yc*y1)/
                (wavelength * zc));
    complex<mdouble> e_1_1_2_e = std::exp(e_1_1_2_imag);

    mdouble e_1_2_1 = (-xc*x1 + yc*y3 - yc*y1 + xc*x3);
    complex<mdouble> e_1_2_2_imag(0, 2*M_PIl * (
                2*zc*zc + xc*xc*zc + yc*yc*zc - 2*xc*x2 - 2*yc*y2)/
                (wavelength*zc));
    complex<mdouble> e_1_2_2_e = std::exp(e_1_2_2_imag);

    mdouble e_1_3_1 = (-xc*x2 + xc*x1 - yc*y2 + yc*y1);
    complex<mdouble> e_1_3_2_imag(0, 2*M_PIl * (
                2*zc*zc + xc*xc*zc + yc*yc*zc - 2*xc*x3 - 2*yc*y3)/
                (wavelength*zc));
    complex<mdouble> e_1_3_2_e = std::exp(e_1_3_2_imag);

    mdouble e_2_1 = (-xc*x3 + xc*x1 - yc*y3 + yc*y1);
    mdouble e_2_2 = (-xc*x2 + xc*x1 - yc*y2 + yc*y1);
    mdouble e_2_3 = (xc*x2 - xc*x3 + yc*y2 - yc*y3);

    complex<mdouble> e_1_1 = e_1_1_1 * e_1_1_2_e;
    complex<mdouble> e_1_2 = e_1_2_1 * e_1_2_2_e;
    complex<mdouble> e_1_3 = e_1_3_1 * e_1_3_2_e;

    complex<mdouble> e_1 = e_1_1 + e_1_2 + e_1_3;

    complex<mdouble> e_2 = e_2_1 * e_2_2 * e_2_3 * M_PIl * M_PIl;

    complex<mdouble> e = e_0 * e_1 / e_2;

    mdouble L1 = (-x3*y2 + x3*y1 - y1*x2 + x1*y2) / (x1 - x2);
    complex<mdouble> result = e;
    if (y3 < L1)
        result *= -1;

#ifdef DEBUG_OUTPUT
    cout<<"New basis: "<<endl<<
               basis<<endl<<endl;
    cout<<"Triangle vertices in local coordinates:"<<endl<<
               newTriangle.p()<<endl<<endl<<
               newTriangle.q()<<endl<<endl<<
               newTriangle.r()<<endl<<endl;
    cout<<"Triangle centroid in local coordinates:"<<endl<<
               newTriangle.center()<<endl<<endl;
//    cout<<"Viewpoint in local coordinates: "<<endl<<
//               newViewpoint<<endl<<endl;
    cout.precision(15);

    cout<<"lambda:="<<wavelength<<";"<<endl;
    cout<<"A[0]:="<<amplitude<<";"<<endl<<endl;

    cout<<"x[1]:="<<x1<<";\ty[1]:="<<y1<<";"<<endl;
    cout<<"x[2]:="<<x2<<";\ty[2]:="<<y2<<";"<<endl;
    cout<<"x[3]:="<<x3<<";\ty[3]:="<<y3<<";"<<endl<<endl;
    cout<<"x[c]:="<<newViewpoint[0]<<";"<<endl;
    cout<<"y[c]:="<<newViewpoint[1]<<";"<<endl;
    cout<<"z:="<<newViewpoint[2]<<";"<<endl<<endl;

    cout<<"Longest bisect = "<<longestBisect<<endl;
    cout<<"z == "<<zc<<endl;
    cout<<"pi*a^2/lambda = "<<M_PIl * longestBisect * longestBisect / wavelength<<endl<<endl;

    cout<<"e_0 = "<<e_0<<endl;
    cout<<"e_1 = "<<e_1<<endl;
    cout<<"e_2 = "<<e_2<<endl;

    cout<<"e_1_1 = "<<e_1_1<<endl;
    cout<<"e_1_1_1 = "<<e_1_1_1<<endl;
    cout<<"e_1_1_2_imag = "<<e_1_1_2_imag<<endl;

    cout<<"e_1_2 = "<<e_1_2<<endl;
    cout<<"e_1_2_1 = "<<e_1_2_1<<endl;
    cout<<"e_1_2_2_imag = "<<e_1_2_2_imag<<endl;

    cout<<"e_1_3 = "<<e_1_3<<endl;
    cout<<"e_1_3_1 = "<<e_1_3_1<<endl;
    cout<<"e_1_3_2_imag = "<<e_1_3_2_imag<<endl;

    cout<<"L1 = "<<L1<<"; y3 = "<<y3<<endl;

    if (isnan(result.real()) || isnan(result.imag())) {
        cout<<"Result is NAN in getEs!"<<endl;
    }
#endif
    return result;
}

MVector
Processor::switchCoordinates(
        const MVector &vector,
        const Eigen::FullPivLU<MMatrix> &matrixLU,
        const MVector &dCenter) {

    MVector eResult = matrixLU.solve(vector);

    eResult += dCenter;

    return eResult;
}

MVector
Processor::switchCoordinates(
        const MVector &vector,
        const Eigen::FullPivLU<MMatrix> &matrixLU) {

    MVector zeroCenter;
    zeroCenter<<0., 0., 0.;
    return switchCoordinates(vector, matrixLU, zeroCenter);
}

complex<mdouble>
Processor::getE(
        const MVector &viewPoint,
        QList<Triangle> &model,
        const mdouble wavelength,
        const mdouble amplitude) {

    mdouble k = 2*M_PI / wavelength;
    complex<mdouble> e;

#ifdef DEBUG_OUTPUT
    cout<<"Wavelength = "<<wavelength<<endl;
    cout<<"Amplitude = "<<amplitude<<endl;
    cout<<"k = "<<k<<endl;
    cout<<"viewpoint: ("<<viewPoint.x()<<";"<<viewPoint.y()<<";"<<viewPoint.z()<<")"<<endl;
#endif

    int i = 0;

    complex<mdouble> result(0, 0);

    MVector eViewpoint;
    eViewpoint<<viewPoint.x(), viewPoint.y(), viewPoint.z();
    for (auto triangle = model.begin(); triangle != model.end(); triangle++) {
        i++;
        if (!isTriangleVisible(*triangle, model, eViewpoint)) {
            continue;
        }

        if (i % 10000 == 0)
            qDebug()<<"Processing triangle "<<i<<"out of "<<model.count();

#ifdef DEBUG_OUTPUT
        cout<<"Processing triangle"<<endl<<
                   triangle->p()<<endl<<endl<<
                   triangle->q()<<endl<<endl<<
                   triangle->r()<<endl<<endl;
#endif

        complex<mdouble> Es = getEs(eViewpoint, *triangle, wavelength, amplitude);

#ifdef DEBUG_OUTPUT
        cout<<"Es = "<<Es<<endl;
#endif
        result += Es;

        cout<<endl<<endl;
    }

    return result;
}

MMatrix
Processor::getCoordinatesTransformationMatrix(
        const Triangle &triangle) {

    MVector OY;
    OY << 0, 1, 0;
    MVector Y = projectOntoPlane(OY, triangle.faceNormal()).normalized();
//    cout<<"Triangle face normal: "<<triangle.faceNormal()<<endl;

    if (Y.squaredNorm() == 0 || isnan(Y.squaredNorm())) { //triangle is horizontal; picking random direction
        Y << 1, 0, 0;
    }

    MVector Z = triangle.faceNormal();

    MVector X = Y.cross(Z);
    X.normalize();

//    cout<<"X: "<<X<<endl<<"Y:"<<Y<<endl<<"Z:"<<Z<<endl;

    MMatrix result;

    result<<X.x(), Y.x(), Z.x(),
            X.y(), Y.y(), Z.y(),
            X.z(), Y.z(), Z.z();
    return result;
}

mdouble
Processor::getAngleBisectionLength(
        const Triangle &triangle,
        int angleIndex) {
    MVector a = triangle[angleIndex],
            b = triangle[angleIndex+1],
            c = triangle[angleIndex + 2];

    MVector bc = c - b;
    mdouble proportion = (b - a).norm() / (c - a).norm();
    mdouble cd_norm = bc.norm() / (proportion + 1);
    MVector cd = (-bc).normalized() * cd_norm;
    MVector d = c + cd;

    return (d - a).norm();
}
