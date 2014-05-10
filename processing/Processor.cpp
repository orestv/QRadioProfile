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
//    std::cout<<"View MVector: "<<viewVector<<std::endl<<std::endl;
    viewVector.normalize();


    mdouble dot = viewVector.dot(triangle.faceNormal());


//    std::cout<<"Dot: "<<dot<<std::endl;
    if (dot <= 0 || std::isnan(dot))
        return false;

    return true;\
}

mdouble
Processor::getSigma(
        const MVector &observationPoint,
        const Triangle &triangle,
        const mdouble R,
        const mdouble wavelength) {

    std::complex<mdouble> E0 = Processor::getE0(observationPoint, triangle, wavelength);
    if (std::isnan(E0.real()) || std::isnan(E0.imag())) {
        std::cout<<"IsNAN in getSigma!"<<std::endl;
    }
    E0 /= wavelength;
    return 2*sqrt(M_PI)*R*std::abs(E0);
}

std::complex<mdouble>
Processor::getE0(
        const MVector &viewpoint,
        const Triangle &triangle,
        const mdouble wavelength) {

    MMatrix basis = getCoordinatesTransformationMatrix(triangle);

    MVector eCenter = - Processor::switchCoordinates(triangle.center(), basis);

    MVector newViewpoint = Processor::switchCoordinates(viewpoint, basis, eCenter);
    MVector p, q, r;
    p = Processor::switchCoordinates(triangle.p(), basis, eCenter);
    q = Processor::switchCoordinates(triangle.q(), basis, eCenter);
    r = Processor::switchCoordinates(triangle.r(), basis, eCenter);
    Triangle newTriangle(p, q, r);

    mdouble x1,y1, x2,y2, x3,y3;
    mdouble xc = newViewpoint[0],
           yc = newViewpoint[1],
           zc = newViewpoint[2];

    if (std::abs(xc) < 0.000001)
        xc = 0.000001;
    if (std::abs(yc) < 0.000001)
        yc = 0.000001;

//    std::cout<<"P:"<<std::endl<<newTriangle.p()<<std::endl<<std::endl;
//    std::cout<<"Q:"<<std::endl<<newTriangle.q()<<std::endl<<std::endl;
//    std::cout<<"R:"<<std::endl<<newTriangle.r()<<std::endl<<std::endl;

    MVector leftMost, rightMost, middle;
    leftMost = newTriangle.leftMost();
    rightMost = newTriangle.rightMost();
    middle = newTriangle.middle();

    x1 = rightMost[0], y1 = rightMost[1];
    x2 = leftMost[0], y2 = leftMost[1];
    x3 = middle[0], y3 = middle[1];

//    std::cout<<"x[1]:="<<x1<<";y[1]:="<<y1<<";"<<std::endl;
//    std::cout<<"x[2]:="<<x2<<";y[2]:="<<y2<<";"<<std::endl;
//    std::cout<<"x[3]:="<<x3<<";y[3]:="<<y3<<";"<<std::endl;

//    std::cout<<"x[c]:="<<xc<<";y[c]:="<<yc<<";z:="<<zc<<";"<<std::endl;

    std::complex<mdouble> e1 = (-x3*y2 - y1*x2 + y1*x3 + x1*y2 + y3*x2 - x1*y3);
//    std::complex<mdouble> e2 = std::exp(std::complex<mdouble>(0, -2*M_PI*(xc*x2 + yc*y2)/(wavelength*zc))) *
//            (-xc*x3 + xc*x1 - yc*y3 + yc*y1);
    std::complex<mdouble> e2_1 = (-xc*x3 + xc*x1 - yc*y3 + yc*y1);
    std::complex<mdouble> e2_2 = std::exp(std::complex<mdouble>(0, -2*M_PI*(xc*x2 + yc*y2)/(wavelength*zc)));
    std::complex<mdouble> e2 = e2_1 * e2_2;
    std::complex<mdouble> e3 = std::exp(std::complex<mdouble>(0, -2*M_PI*(xc*x3 + yc*y3)/(wavelength*zc)))*
            (-xc*x2 + xc*x1 - yc*y2 + yc*y1);
    std::complex<mdouble> e4 = std::exp(std::complex<mdouble>(0, -2*M_PI*(xc*x1 + yc*y1)/(wavelength*zc)))*
            (xc*x2 - xc*x3 + yc*y2 - yc*y3);
    mdouble e5 = zc*zc*wavelength*wavelength;
    mdouble e6 = -4*(-xc*x3 + xc*x1 - yc*y3 + yc*y1) *
            (-xc*x2 + xc*x1 - yc*y2 + yc*y1) *
            (xc*x2 - xc*x3 + yc*y2 - yc*y3)*M_PI*M_PI;

    std::complex<mdouble> result = e1 * (e2 - e3 - e4) * e5 / e6;

#ifdef DEBUG_OUTPUT
    std::cout<<"New basis: "<<std::endl<<
               basis<<std::endl<<std::endl;
    std::cout<<"Triangle vertices in local coordinates:"<<std::endl<<
               newTriangle.p()<<std::endl<<std::endl<<
               newTriangle.q()<<std::endl<<std::endl<<
               newTriangle.r()<<std::endl<<std::endl;
    std::cout<<"Triangle centroid in local coordinates:"<<std::endl<<
               newTriangle.center()<<std::endl<<std::endl;
//    std::cout<<"Viewpoint in local coordinates: "<<std::endl<<
//               newViewpoint<<std::endl<<std::endl;
    std::cout.precision(15);
    std::cout<<"x[1]:="<<x1<<";\ty[1]:="<<y1<<";"<<std::endl;
    std::cout<<"x[2]:="<<x2<<";\ty[2]:="<<y2<<";"<<std::endl;
    std::cout<<"x[3]:="<<x3<<";\ty[3]:="<<y3<<";"<<std::endl<<std::endl;
    std::cout<<"x[c]:="<<newViewpoint[0]<<";"<<std::endl;
    std::cout<<"y[c]:="<<newViewpoint[1]<<";"<<std::endl;
    std::cout<<"z[c]:="<<newViewpoint[2]<<";"<<std::endl<<std::endl;

    std::cout<<"E0 = "<<result<<std::endl;
    std::cout<<"e1 =\t"<<e1<<std::endl;
    std::cout<<"e2 = \t"<<e2<<std::endl;
    std::cout<<"e3 =\t"<<e3<<std::endl;
    std::cout<<"e4 =\t"<<e4<<std::endl;
    std::cout<<"e5 =\t"<<e5<<std::endl;
    std::cout<<"e6 =\t"<<e6<<std::endl;
#endif

    if (std::isnan(result.real()) || std::isnan(result.imag())) {
        std::cout<<"Result is NAN in getE0!"<<std::endl;
    }
    return result;
}

MVector
Processor::switchCoordinates(
        const MVector &vector,
        const MMatrix &matrix,
        const MVector &dCenter) {

    Eigen::ColPivHouseholderQR<MMatrix> solver(matrix);
    MVector eResult = solver.solve(vector);

    eResult += dCenter;

    return eResult;
}

MVector
Processor::switchCoordinates(
        const MVector &vector,
        const MMatrix &matrix) {

    MVector zeroCenter;
    zeroCenter<<0., 0., 0.;
    return switchCoordinates(vector, matrix, zeroCenter);
}

std::complex<mdouble>
Processor::getE(
        const MVector &viewPoint,
        QList<Triangle> &model,
        const mdouble wavelength,
        const mdouble amplitude) {

    mdouble k = 2*M_PI / wavelength;
    std::complex<mdouble> e;

#ifdef DEBUG_OUTPUT
    std::cout<<"Wavelength = "<<wavelength<<std::endl;
    std::cout<<"k = "<<k<<std::endl;
#endif

    int i = 0;
    mdouble sum_cos = 0, sum_sin = 0;

    MVector eViewpoint;
    eViewpoint<<viewPoint.x(), viewPoint.y(), viewPoint.z();
    for (auto triangle = model.begin(); triangle != model.end(); triangle++) {
        i++;
        if (!isTriangleVisible(*triangle, model, eViewpoint)) {
            continue;
        }

        if (i % 10000 == 0)
            qDebug()<<"Processing triangle "<<i<<"out of "<<model.count();

        mdouble R = (eViewpoint - triangle->center()).norm();
#ifdef DEBUG_OUTPUT
        std::cout<<"Processing triangle"<<std::endl<<
                   triangle->p()<<std::endl<<std::endl<<
                   triangle->q()<<std::endl<<std::endl<<
                   triangle->r()<<std::endl<<std::endl;
        std::cout<<"R = "<<R<<std::endl;
#endif
        mdouble sigma = getSigma(eViewpoint, *triangle, R, wavelength);
        mdouble SR = sigma / pow(R, 2);

        sum_cos += SR * cos(k*R);
        sum_sin += SR * sin(k*R);

#ifdef DEBUG_OUTPUT
        std::cout<<"sigma = "<<sigma<<std::endl;
        std::cout<<"k*R = "<<k*R<<std::endl;
        std::cout<<"sin(k*R) = "<<sin(k*R)<<std::endl;
        std::cout<<"cos(k*R) = "<<cos(k*R)<<std::endl;
        std::cout<<"sigma*cos(k*R)/(R^2) = "<<SR*cos(k*R)<<std::endl;
        std::cout<<"sigma*sin(k*R)/(R^2) = "<<SR*sin(k*R)<<std::endl;
        std::cout<<"sum_cos = "<<sum_cos<<std::endl;
        std::cout<<"sum_sin = "<<sum_sin<<std::endl;
#endif

        if (std::isnan(sum_cos) || std::isnan(sum_sin)) {
            std::cout<<"NAN processing triangle "<<std::endl;
        }
        std::cout<<std::endl<<std::endl;
    }

    return std::complex<mdouble>(sum_cos, sum_sin);
}

MMatrix
Processor::getCoordinatesTransformationMatrix(
        const Triangle &triangle) {

    MVector OY;
    OY << 0, 1, 0;
    MVector Y = projectOntoPlane(OY, triangle.faceNormal()).normalized();
//    std::cout<<"Triangle face normal: "<<triangle.faceNormal()<<std::endl;

    if (Y.squaredNorm() == 0 || std::isnan(Y.squaredNorm())) { //triangle is horizontal; picking random direction
        Y << 1, 0, 0;
    }

    MVector Z = triangle.faceNormal();

    MVector X = Y.cross(Z);
    X.normalize();

//    std::cout<<"X: "<<X<<std::endl<<"Y:"<<Y<<std::endl<<"Z:"<<Z<<std::endl;

    MMatrix result;

    result<<X.x(), Y.x(), Z.x(),
            X.y(), Y.y(), Z.y(),
            X.z(), Y.z(), Z.z();
    return result;
}
