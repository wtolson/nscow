/*
 * NavierStokes.h
 *
 *  Created on: Apr 25, 2010
 *      Author: wto
 */

#ifndef NAVIERSTOKES_H_
#define NAVIERSTOKES_H_

#include "VectorField2D.h"
#include "ScalarField2D.h"

class NavierStokes {
public:

    static double nu;
    static double t;
    static double dt;
    static double dx;
    static double e;
    static double dC;
    static double rTool;
    static double magTool;


    static void advect(VectorField2D& u);
    static void advect(const VectorField2D& u, ScalarField2D &X);
    static void diffuse(VectorField2D& u);
    static void diffuse(const VectorField2D& u, ScalarField2D& X);
    static void addForces(VectorField2D &u, double x1, double y1, double x2, double y2);
    static void computePressure(const VectorField2D& u, ScalarField2D& p);
    static void subtractPressureGradient(VectorField2D& u, const ScalarField2D& p);
    static void vorticityConfinement(VectorField2D& u);

    static void divergence(ScalarField2D& divW, const VectorField2D& w);
    static void addDye(ScalarField2D& rho, double x0, double y0);
    static void addInOutForce(VectorField2D &u, double x0, double y0, int inout = 1);

};

#endif /* NAVIERSTOKES_H_ */
