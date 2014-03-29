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
#include <eigen3/Eigen/Geometry>
#include <eigen3/Eigen/Eigen>

#include "Processor.h"
#include "ecalculator.h"
#include <math.h>

QDebug operator<< (QDebug d, RightTriangle &triangle) {
    d<<triangle.vertex()<<triangle.p1()<<triangle.p2();
    return d;
}

const double ANGLE_PRECISION = 0.01;

QList<RightTriangle> 
Processor::getVisibleTriangles(
        QList<RightTriangle> &triangles,
        QVector3D viewpoint) {
    QList<RightTriangle> result;
    
    for (auto t = triangles.begin(); t != triangles.end(); t++) {
        if (Processor::isTriangleVisible(*t, viewpoint))
            result.push_back(*t);
    }
    
    return result;
}

bool 
Processor::isTriangleVisible(
        RightTriangle triangle, 
        QVector3D viewpoint) {
    QVector3D viewVector =  viewpoint - triangle.vertex();
    viewVector.normalize();
    float viewAngleCos = QVector3D::dotProduct(viewVector, triangle.normal());
    return viewAngleCos > 0;
}

Processor::VIEWPOINT_SUMS
Processor::calculateViewpointSums(
        QList<RightTriangle> &triangles,
        QVector3D viewpoint, 
        double wavelength) {
    
    VIEWPOINT_SUMS result;
    result.sum_cos = 0;
    result.sum_sin = 0;
    double k = 2 * M_PI / wavelength;
    
    for (auto t = triangles.begin(); t != triangles.end(); t++) {
        TRIANGLE_ANGLES angles = calculateTriangleAngles(*t, viewpoint);
        QVector3D shortLeg = t->shortLeg(), 
            longLeg = t->longLeg();
        double En = calculateEn(angles, shortLeg, longLeg, wavelength);
        double R = (viewpoint - t->vertex()).length();  
        
//        std::cout<<"En for "<<*t<<" = "<<En;
        
        result.sum_cos += En * cos(k*R);
        result.sum_sin += En * sin(k*R);
    }
        
    return result;
}

Processor::TRIANGLE_ANGLES
Processor::calculateTriangleAngles(
        RightTriangle &triangle, 
        QVector3D &viewpoint) {
    
    QVector3D viewVector = viewpoint - triangle.vertex();    
    QVector3D triangleNormal = triangle.normal();
    
    TRIANGLE_ANGLES result;
    
    if (abs(triangle.leg_1().lengthSquared() - 
            triangle.leg_2().lengthSquared()) > 0.01) {
        QVector3D shortLegPlaneNormal = triangle.longLeg().normalized();
        result = calculateTriangleAngles(triangleNormal, shortLegPlaneNormal, viewVector);
    } else {
        QVector3D normal_1 = triangle.leg_1().normalized(),
                normal_2 = triangle.leg_2().normalized();
        TRIANGLE_ANGLES result_1 = calculateTriangleAngles(triangleNormal,
                        normal_1, viewVector),
                result_2 = calculateTriangleAngles(triangleNormal,
                        normal_2, viewVector);
        
//        std::cout<<"Alphas:"<<result_1.alpha<<result_2.alpha<<std::endl;
//        std::cout<<"betas:"<<result_1.beta<<result_2.beta<<std::endl;
        
        result.cos_alpha = fmin(result_1.cos_alpha, result_2.cos_alpha);
        result.cos_beta = fmin(result_1.cos_beta, result_2.cos_beta);
    }    
    return result;
}

Processor::TRIANGLE_ANGLES
Processor::calculateTriangleAngles (
        QVector3D& triangleNormal,
        QVector3D planeNormal, 
        QVector3D& viewVector) {
    
//    QVector3D vv_projection = projectOntoPlane(viewVector,
//            planeNormal);
//    QVector3D vv_projection_unit = vv_projection.normalized();
    
//    double cos_alpha = QVector3D::dotProduct(vv_projection_unit,
//            triangleNormal);
//    if (cos_alpha < 0)
//        cos_alpha = QVector3D::dotProduct(vv_projection_unit,
//            -triangleNormal);
//    double cos_beta = QVector3D::dotProduct(viewVector.normalized(),
//            vv_projection_unit);
//    if (cos_beta < 0)
//        cos_beta = QVector3D::dotProduct(-viewVector.normalized(),
//            vv_projection_unit);
    
//    double alpha = acos(cos_alpha),
//            beta = acos(cos_beta);
//    TRIANGLE_ANGLES result;
//    result.cos_alpha = cos_alpha;
//    result.cos_beta = cos_beta;
//    result.sin_alpha = sin(alpha);
//    result.sin_beta = sin(beta);
    
//    return result;
}

Vector3d
Processor::projectOntoPlane(
        const Vector3d &vector,
        Vector3d plane_normal) {
    
    plane_normal.normalize();
    
    Vector3d projection_onto_normal = plane_normal *
            plane_normal.dot(vector);
    
    return vector - projection_onto_normal;
}

double 
Processor::calculateEn(
        TRIANGLE_ANGLES& angles, 
        QVector3D& shortLeg, 
        QVector3D& longLeg,
        double wavelength) {
    
    double result;
    
    double cos_alpha = angles.cos_alpha, cos_beta = angles.cos_beta,
            sin_alpha = angles.sin_alpha, sin_beta = angles.sin_beta;
//    if (sin_alpha < ANGLE_PRECISION) {
//        sin_alpha = 0;
//        cos_alpha = 1;
//    }
//    if (abs(sin_beta) < ANGLE_PRECISION) {
//        sin_beta = 0;
//        cos_beta = 1;
//    }
    double a = shortLeg.length();
    double b = longLeg.length();
    
    double sigma = 4 * M_PI * 
                shortLeg.lengthSquared() * longLeg.lengthSquared() / (
                pow(wavelength, 2));
    double k = 2 * M_PI / wavelength;
    
//    std::cout<<alpha<<beta<<a<<b<<sigma<<k<<wavelength;
    
    if (sin_alpha == 0 && sin_beta == 0) {
        result = sigma;
    } else if (sin_beta == 0) {
        result = sigma * pow(cos_alpha, 2) * 
                pow((sin(k*a*cos_alpha)) / (k*a*sin_alpha), 4);
    } else if (sin_alpha == 0) {
        double kbsinb = k * b * sin_beta;
        result = sigma * pow(cos_beta, 2) * 
                (
                    pow(sin(kbsinb)/kbsinb, 4) + 
                    pow(((1 - (sin(2*kbsinb))/(kbsinb)) / (kbsinb)), 2)
                );
    } else {
        double e1_top = sigma * pow(cos_alpha*cos_beta, 2);
        double e1_bottom_1 = pow(k*a*sin_alpha*cos_beta, 2);
        double e1_bottom_2 = pow(k*b*sin_beta, 2);
        double e1_bottom = pow((e1_bottom_1 - e1_bottom_2), 2);
        double e1 = e1_top/e1_bottom;
////        std::cout<<sigma<<pow(cos(alpha)*cos(beta), 2);
//        std::cout<<"e1_bottom_1="<<e1_bottom_1<<",\te1_bottom_2="<<e1_bottom_2<<std::endl;
//        std::cout<<"e1_top="<<e1_top<<",\te1_bottom="<<e1_bottom<<",\te1="<<e1<<std::endl;

        double e2_1 = pow(pow(sin(k*a*sin_alpha*cos_beta), 2) - pow(sin(k*b*sin_beta), 2), 2);
        double e2_1_1 = sin(k*a*sin_alpha*cos_beta);
        double e2_1_2 = sin(k*b*sin_beta);
//        std::cout<<"k="<<k<<",\ta="<<a<<",\tsin(alpha)="<<sin(alpha)<<",\t"<<"cos(beta)="<<cos(beta)<<std::endl;
//        std::cout<<"Ridiculously large:"<<k*a*sin(alpha)*cos(beta)<<std::endl;
        e2_1_1 *= e2_1_1;
        e2_1_2 *= e2_1_2;
        
//        std::cout<<"e2_1 parts: "<<e2_1_1<<e2_1_2<<std::endl;
        
        e2_1 = pow(e2_1_1 - e2_1_2, 2);
        double e2_2 = pow(k*b*sin_beta, 2);
        double e2_3 = sin(2*k*a*sin_alpha*cos_beta)/(2*k*a*sin_alpha*cos_beta);
        double e2_4 = sin(2*k*b*sin_beta)/(2*k*b*sin_beta);
        
//        std::cout<<"e2_1="<<e2_1<<"e2_2="<<e2_2;

        double e2_03 = pow(e2_3 - e2_4, 2);

        double e2 = e2_1 + e2_2 * e2_03;
//        std::cout<<"e2 = "<<e2;
        double e3 = e1 * e2;
        result = e3;
    }
    
    return result;
}

QList<Processor::CALCULATION_RESULT>
Processor::analyzeModel(QList<RightTriangle> &triangles,
        Processor::PARAMS params) {
    QList<Processor::CALCULATION_RESULT> result;
    
    double wavelength = Processor::LIGHTSPEED / params.frequency;
    
    for (double viewpointAzimuth = 0;
            viewpointAzimuth < 2*M_PI;
            viewpointAzimuth += params.viewpointRotationStep) {
        
        double x, y, z;
        y = params.viewpointHeight;
        z = params.viewpointDistance * cos(viewpointAzimuth);
        x = params.viewpointDistance * sin(viewpointAzimuth);
        
        QVector3D viewpoint(x, y, z);
//        std::cout<<"viewpoint: "<<viewpoint;
        
        QList<RightTriangle> visibleTriangles = Processor::getVisibleTriangles(
                triangles, viewpoint);

        Processor::VIEWPOINT_SUMS sums = Processor::calculateViewpointSums(
                visibleTriangles, viewpoint, wavelength);

        double E = sqrt(pow(sums.sum_cos, 2) + pow(sums.sum_sin, 2));
        
        Processor::CALCULATION_RESULT localResult;        
        localResult.azimuth = viewpointAzimuth;
        localResult.E = E;
        
        result.push_back(localResult);
    }
    
    return result;
}

std::complex<double>
Processor::getE(
        const Vector3d &viewPoint,
        QList<Triangle> &model,
        const double wavelength) {    

//    qDebug()<<"GetE invoked";

    double k = 2*M_PI / wavelength;
    std::complex<double> e;

//    std::cout<<"Viewpoint: "<<viewPoint<<std::endl<<std::endl;

    int i = 0;
    Vector3d eViewpoint;
    eViewpoint<<viewPoint.x(), viewPoint.y(), viewPoint.z();
    for (auto triangle = model.begin(); triangle != model.end(); triangle++) {
        i++;
        if (!isTriangleVisible(*triangle, model, eViewpoint)) {
//            std::cout<<"("<<triangle->p()<<","<<triangle->q()<<","<<triangle->r()<<
//                      ") is invisible, skipping.";
            continue;
        }


//        std::cout<<"GetE: processing triangle "<<
//                   triangle->p()<<std::endl<<"-"<<std::endl<<
//                   triangle->q()<<std::endl<<"-"<<std::endl<<
//                   triangle->r()<<std::endl;

        if (i % 100 == 0)
            qDebug()<<"Processing triangle "<<i<<"out of "<<model.count();

        double R = (eViewpoint - triangle->center()).norm();
        std::complex<double> local_e;
        local_e.real(cos(k*R));
        local_e.imag(sin(k*R));

//        qDebug()<<"Local E is "<<local_e.real()<<"+ i"<<local_e.imag();

        double sigma = getSigma(eViewpoint, *triangle, R, wavelength);

//        qDebug()<<"Sigma = "<<sigma;

        local_e *= sigma;
        local_e /= pow(R, 2);

        e += local_e;

//        qDebug()<<"So far e is "<<abs(e);

    }
    return e;
}

bool
Processor::isTriangleVisible(
        const Triangle &triangle,
        const QList<Triangle> &model,
        const Vector3d &viewPoint) {

    Vector3d viewVector = (viewPoint - triangle.center());
//    std::cout<<"View vector: "<<viewVector<<std::endl<<std::endl;
    viewVector.normalize();


    double dot = viewVector.dot(triangle.faceNormal());


//    std::cout<<"Dot: "<<dot<<std::endl;
    if (dot < 0 || isnan(dot))
        return false;

    return true;
}

double
Processor::getSigma(
        const Vector3d &observationPoint,
        const Triangle &triangle,
        const double R,
        const double wavelength) {

    double u = getU(observationPoint, triangle, wavelength);

//    qDebug()<<"U = "<<u;

    return sqrt(pow(R, 2) * u);
}

long double
Processor::getU(
        const Vector3d &observationPoint,
        const Triangle &triangle,
        const double wavelength) {

    Eigen::Matrix3d basis = getCoordinatesTransformationMatrix(triangle);

    Eigen::Vector3d eCenter = - Processor::switchCoordinates(triangle.center(), basis);

    Eigen::Vector3d newViewpoint = Processor::switchCoordinates(observationPoint, basis, eCenter);
    Eigen::Vector3d p, q, r;
    p = Processor::switchCoordinates(triangle.p(), basis, eCenter);
    q = Processor::switchCoordinates(triangle.q(), basis, eCenter);
    r = Processor::switchCoordinates(triangle.r(), basis, eCenter);
    Triangle newTriangle(p, q, r);

    ECalculator calculator(newViewpoint, newTriangle, wavelength);
    std::complex<double> integral = calculator.calculateIntegral();

//    qDebug()<<"Integral calculated: "<<integral.real()<<" + i"<<integral.imag();

    integral /= pow(2*M_PI, 2);

//    qDebug()<<"Integral transformed: "<<integral.real()<<" + i"<<integral.imag();

    double integralAbs = std::abs(integral);

//    qDebug()<<"Integral abs: "<<integralAbs;

    double result = integralAbs * pow(2*M_PI, 4) / pow(wavelength * newViewpoint[2], 2);

    return result;
}

Eigen::Vector3d
Processor::switchCoordinates(
        const Vector3d &vector,
        const Eigen::Matrix3d &matrix,
        const Eigen::Vector3d &dCenter) {

    Eigen::ColPivHouseholderQR<Eigen::Matrix3d> solver(matrix);
    Eigen::Vector3d eResult = solver.solve(vector);

    eResult += dCenter;

    return eResult;
}

Eigen::Vector3d
Processor::switchCoordinates(
        const Vector3d &vector,
        const Eigen::Matrix3d &matrix) {

    Eigen::Vector3d zeroCenter;
    zeroCenter<<0., 0., 0.;
    return switchCoordinates(vector, matrix, zeroCenter);
}

Eigen::Matrix3d
Processor::getCoordinatesTransformationMatrix(
        const Triangle &triangle) {

    Vector3d OY;
    OY << 0, 1, 0;
    Vector3d Y = projectOntoPlane(OY, triangle.faceNormal()).normalized();
//    std::cout<<"Triangle face normal: "<<triangle.faceNormal()<<std::endl;

    if (Y.squaredNorm() == 0 || isnan(Y.squaredNorm())) { //triangle is horizontal; picking random direction
        Y << 1, 0, 0;
    }

    Vector3d Z = triangle.faceNormal();

    Vector3d X = Y.cross(Z);
    X.normalize();

//    std::cout<<"X: "<<X<<std::endl<<"Y:"<<Y<<std::endl<<"Z:"<<Z<<std::endl;

    Eigen::Matrix3d result;

    result<<X.x(), Y.x(), Z.x(),
            X.y(), Y.y(), Z.y(),
            X.z(), Y.z(), Z.z();
    return result;
}
