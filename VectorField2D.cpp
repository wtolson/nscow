/*
 * VectorField2D.cpp
 *
 *  Created on: Apr 24, 2010
 *      Author: wto
 */

#include "VectorField2D.h"
#include <iostream>
#include <cmath>
using namespace std;

VectorField2D::VectorField2D(int xdim, int ydim, double boundryScale, double* data) :
        xdim(xdim), ydim(ydim), itsData(data), ownsData(false), itsBoundryScale(boundryScale) {
    if (itsData == 0) {
        ownsData = true;
        itsData = new double[2 * xdim * ydim];
    }
}

VectorField2D::~VectorField2D() {
    if (ownsData) {
        delete itsData;
    }
}

VectorField2D::VectorField2D(const VectorField2D& other) :
        xdim(other.xdim), ydim(other.ydim), ownsData(true), itsBoundryScale(other.itsBoundryScale)  {
    itsData = new double[2 * xdim * ydim];
    for(int i = 0; i < 2*xdim*ydim; i++) {
        itsData[i] = other.itsData[i];
    }
}

VectorField2D& VectorField2D::operator=(const VectorField2D& other) {
    itsBoundryScale = other.itsBoundryScale;
    for(int i = 0; i < 2*xdim*ydim; i++) {
        itsData[i] = other.itsData[i];
    }
    return *this;
}

void VectorField2D::print() const {
    for (int k = 0; k < 2; k++) {
        cout << "[";
        cout << value(0, 0, k);
        for (int x = 1; x < xdim; x++) {
            cout << " " << value(x, 0, k);
        }
        for (int y = 1; y < ydim; y++) {
            cout << endl << " " << value(0, y, k);
            for (int x = 1; x < xdim; x++) {
                cout << " " << value(x, y, k);
            }
        }
        cout << "]" << endl << flush;
    }
}

double VectorField2D::checkBoundry(int x, int y, int k) const {
    double scale = 1;

    if (x < 0) {
        scale = itsBoundryScale;
        x = 0;
    }
    if (x > xdim-1) {
        scale = itsBoundryScale;
        x = xdim-1;
    }
    if (y < 0) {
        scale = itsBoundryScale;
        y = 0;
    }
    if (y > ydim-1) {
        scale = itsBoundryScale;
        y = ydim-1;
    }
    return scale*value(x,y,k);
}

double VectorField2D::min() const {
    double ans = itsData[0];
    for(int i = 1; i < (2*xdim*ydim); i++)
        ans = std::min(ans, itsData[i]);

    return ans;
}

double VectorField2D::max() const {
    double ans = itsData[0];
    for(int i = 1; i < (2*xdim*ydim); i++)
        ans = std::max(ans, itsData[i]);

    return ans;
}

double VectorField2D::mean() const {
    double ans = 0;
    for (int x = 0; x < xdim; x++) {
        for (int y = 0; y < ydim; y++) {
            ans += sqrt(value(x,y,0)*value(x,y,0) + value(x,y,1)*value(x,y,1));
        }
    }
    return ans / (xdim*ydim);
}

void VectorField2D::fill(double val) {
    for(double* ittr = itsData; ittr < itsData+2*xdim*ydim; ittr++)
        *ittr = val;
}
