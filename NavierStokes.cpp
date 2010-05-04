/*
 * NavierStokes.cpp
 *
 *  Created on: Apr 24, 2010
 *      Author: William Olson
 */

#include <cmath>
using namespace std;

#include "ScalarField2D.h"
#include "VectorField2D.h"
#include "NavierStokes.h"

#include "VectorOperators.h"

double NavierStokes::nu = 1;
double NavierStokes::t = 0.0;
double NavierStokes::dt = 1.0;
double NavierStokes::dx = 1.0;
double NavierStokes::e = 1.0;
double NavierStokes::dC = 1.0;
double NavierStokes::rTool = 1.0;
double NavierStokes::magTool = 1.0;

void NavierStokes::advect(VectorField2D& u) {
    VectorField2D oldU(u);
    double rdx = 1.0/dx;
    for (int x = 0; x < u.xdim; x++) {
        for (int y = 0; y < u.ydim; y++) {
            double xpos = x - dt * rdx * oldU.value(x,y,0);
            double ypos = y - dt * rdx * oldU.value(x,y,1);
            int x0 = floor(xpos);
            int y0 = floor(ypos);
            xpos -= x0;
            ypos -= y0;
            for (int k = 0; k < 2; k++) {
                double b[4] = { oldU.checkBoundry(x0,y0,k),
                                oldU.checkBoundry(x0+1,y0,k) - oldU.checkBoundry(x0,y0,k),
                                oldU.checkBoundry(x0,y0+1,k) - oldU.checkBoundry(x0,y0,k),
                                oldU.checkBoundry(x0,y0,k) - oldU.checkBoundry(x0+1,y0,k) - oldU.checkBoundry(x0,y0+1,k) + oldU.checkBoundry(x0+1,y0+1,k),
                            };
                u.value(x,y,k) = b[0] + b[1]*xpos + b[2]*ypos + b[3]*xpos*ypos;
            }
        }
    }
}

void NavierStokes::advect(const VectorField2D& u, ScalarField2D &X) {
    double rdx = 1.0/dx;
    ScalarField2D oldX(X);
    for (int x = 0; x < X.xdim; x++) {
        for (int y = 0; y < X.ydim; y++) {
            double xpos = x - dt * rdx * u.value(x,y,0);
            double ypos = y - dt * rdx * u.value(x,y,1);
            int x0 = floor(xpos);
            int y0 = floor(ypos);
            xpos -= x0;
            ypos -= y0;
            double b[4] = { oldX.checkBoundry(x0,y0),
                            oldX.checkBoundry(x0+1,y0) - oldX.checkBoundry(x0,y0),
                            oldX.checkBoundry(x0,y0+1) - oldX.checkBoundry(x0,y0),
                            oldX.checkBoundry(x0,y0) - oldX.checkBoundry(x0+1,y0) - oldX.checkBoundry(x0,y0+1) + oldX.checkBoundry(x0+1,y0+1),
                        };
            X.value(x,y) = b[0] + b[1]*xpos + b[2]*ypos + b[3]*xpos*ypos;
        }
    }
}

void NavierStokes::diffuse(VectorField2D& u) {
    double alpha = dx*dx/(nu*dt);
    double rBeta = 1.0/(4.0+alpha);
    VectorField2D oldU(u);
    for (int x = 0; x < u.xdim; x++) {
        for (int y = 0; y < u.ydim; y++) {
            for (int k = 0; k < 2; k++){
                double xL = oldU.checkBoundry(x-1,y,k);
                double xR = oldU.checkBoundry(x+1,y,k);
                double xB = oldU.checkBoundry(x,y-1,k);
                double xT = oldU.checkBoundry(x,y+1,k);
                double bC = oldU.checkBoundry(x,y,k);

                u.value(x,y,k) = (xL + xR + xB + xT + alpha * bC) * rBeta;
            }
        }
    }
}

void NavierStokes::diffuse(const VectorField2D& u, ScalarField2D& X) {
    double alpha = dx*dx/(dC*dt);
    double rBeta = 1.0/(4.0+alpha);
    ScalarField2D oldX(X);
    for (int x = 0; x < u.xdim; x++) {
        for (int y = 0; y < u.ydim; y++) {
            double xL = oldX.checkBoundry(x-1,y);
            double xR = oldX.checkBoundry(x+1,y);
            double xB = oldX.checkBoundry(x,y-1);
            double xT = oldX.checkBoundry(x,y+1);
            double bC = oldX.checkBoundry(x,y);

            X.value(x,y) = (xL + xR + xB + xT + alpha * bC) * rBeta;
        }
    }
}

void NavierStokes::addForces(VectorField2D &u, double x1, double y1, double x2, double y2) {
    double rr = 0.3*magTool/(rTool*rTool);
    double a = (y1-y2);
    double b = (x2-x1);
    double c = -a*x1 - b*y1;
    for (int x = 0; x < u.xdim; x++) {
        for (int y = 0; y < u.ydim; y++) {
            double l = a*a+b*b;
            double d = (a*x + b*y + c);
            d = d*d/(a*a + b*b);
            double r = (x-x1)*(x-x1) + (y-y1)*(y-y1);
            if (d+r>l) {
                d = (x-x2)*(x-x2) + (y-y2)*(y-y2);
            } else if (d+(x-x2)*(x-x2)+(y-y2)*(y-y2) > l) {
                d = r;
            }
            l = exp(-d*rr);
            u.value(x,y,0) += b*l*rr;
            u.value(x,y,1) -= a*l*rr;
        }
    }
}


void NavierStokes::computePressure(const VectorField2D& u, ScalarField2D& p){
    double alpha = -dx*dx;
    double rBeta = 1.0/4.0;
    ScalarField2D divW(p.xdim,p.ydim);
    divergence(divW,u);
    ScalarField2D oldP(p);
    for (int x = 0; x < u.xdim; x++) {
        for (int y = 0; y < u.ydim; y++) {
            double xL = oldP.checkBoundry(x-1,y);
            double xR = oldP.checkBoundry(x+1,y);
            double xB = oldP.checkBoundry(x,y-1);
            double xT = oldP.checkBoundry(x,y+1);
            double bC = divW.value(x,y);
            p.value(x,y) = (xL + xR + xB + xT + alpha * bC) * rBeta;
        }
    }
}

void NavierStokes::subtractPressureGradient(VectorField2D &u, const ScalarField2D &p) {
    double halfrdx = 1.0/(2.0*dx);
    for (int x = 0; x < u.xdim; x++) {
        for (int y = 0; y < u.ydim; y++) {
            for (int k = 0; k < 2; k++){
                double pL = p.checkBoundry(x-1,y);
                double pR = p.checkBoundry(x+1,y);
                double pB = p.checkBoundry(x,y-1);
                double pT = p.checkBoundry(x,y+1);
                u.value(x,y,0) -= halfrdx * (pR - pL);
                u.value(x,y,1) -= halfrdx * (pT - pB);
            }
        }
    }    
}

void NavierStokes::vorticityConfinement(VectorField2D &u) {
    if (e==0) return;
    ScalarField2D omega(u.xdim,u.ydim);
    DelCross(omega,u);
    VectorField2D Psi(u.xdim,u.ydim);
    Del(omega,Psi);
    for (int x = 0; x < u.xdim; x++) {
        for (int y = 0; y < u.ydim; y++) {
            double mag = sqrt(Psi.value(x,y,0)*Psi.value(x,y,0) + Psi.value(x,y,1)*Psi.value(x,y,1));
            if (mag != 0) {
                Psi.value(x,y,0) /= mag;
                Psi.value(x,y,1) /= mag;
            }
        }
    }
    double scale = e*dx*dt;
    for (int x = 0; x < u.xdim; x++) {
        for (int y = 0; y < u.ydim; y++) {
            u.value(x,y,0) += scale*Psi.value(x,y,1)*omega.value(x,y);
            u.value(x,y,1) -= scale*Psi.value(x,y,0)*omega.value(x,y);
        }
    }
}

void NavierStokes::divergence(ScalarField2D& divW, const VectorField2D& w) {
    double halfrdx = 1.0/(2.0*dx);
    for (int x = 0; x < w.xdim; x++) {
        for (int y = 0; y < w.ydim; y++) {
            double wLx = w.checkBoundry(x-1,y,0);
            double wRx = w.checkBoundry(x+1,y,0);
            double wBy = w.checkBoundry(x,y-1,1);
            double wTy = w.checkBoundry(x,y+1,1);
            divW.value(x, y) = halfrdx * ((wRx - wLx) + (wTy - wBy));
        }
    }
}

void NavierStokes::addDye(ScalarField2D &rho, double x0, double y0) {
    for (int x = 0; x < rho.xdim; x++) {
        for (int y = 0; y < rho.ydim; y++) {
            if (sqrt((x-x0)*(x-x0)+(y-y0)*(y-y0)) <= rTool)
                rho.value(x,y) += magTool;
        }
    }
}

void NavierStokes::addInOutForce(VectorField2D &u, double x0, double y0, int inout) {
    inout *= 10;
    double rr = 1.0/(rTool*rTool);
    double r,b;
    for (int x = 0; x < u.xdim; x++) {
        for (int y = 0; y < u.ydim; y++) {
            r = (x-x0)*(x-x0) + (y-y0)*(y-y0);
            if(r!=0.0) {
                b = exp(-r*rr);
                u.value(x,y,0) += inout*(x-x0)*b*rr;
                u.value(x,y,1) += inout*(y-y0)*b*rr;
            }
        }
    }
}
