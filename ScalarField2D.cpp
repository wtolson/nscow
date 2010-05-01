/*
 * ScalarField2D.cpp
 *
 *  Created on: Apr 24, 2010
 *      Author: wto
 */

#include <iostream>
#include <cmath>
#include "ScalarField2D.h"

using namespace std;

ScalarField2D::ScalarField2D(int xdim, int ydim, double boundryScale, double* data) :
        xdim(xdim), ydim(ydim), itsData(data), ownsData(false), itsBoundryScale(boundryScale) {
    if (itsData == 0) {
        ownsData = true;
        itsData = new double[xdim * ydim];
    }
}

ScalarField2D::~ScalarField2D() {
    if (ownsData) {
        delete itsData;
    }
}

ScalarField2D::ScalarField2D(const ScalarField2D& other) :
        xdim(other.xdim), ydim(other.ydim), ownsData(true), itsBoundryScale(other.itsBoundryScale)  {
    itsData = new double[xdim * ydim];
    for(int i = 0; i < xdim*ydim; i++) {
        itsData[i] = other.itsData[i];
    }
}

ScalarField2D& ScalarField2D::operator=(const ScalarField2D& other) {
    itsBoundryScale = other.itsBoundryScale;
    for(int i = 0; i < xdim*ydim; i++) {
        itsData[i] = other.itsData[i];
    }
    return *this;
}

double ScalarField2D::min() const {
    double ans = itsData[0];
    for(int i = 1; i < (xdim*ydim); i++)
        ans = std::min(ans, itsData[i]);

    return ans;
}

double ScalarField2D::max() const {
    double ans = itsData[0];
    for(int i = 1; i < (xdim*ydim); i++)
        ans = std::max(ans, itsData[i]);

    return ans;
}

void ScalarField2D::print() const {
    cout << "[";
    cout << value(0, 0);
    for (int x = 1; x < xdim; x++) {
        cout << " " << value(x, 0);
    }
    for (int y = 1; y < ydim; y++) {
        cout << endl << " " << value(0, y);
        for (int x = 1; x < xdim; x++) {
            cout << " " << value(x, y);
        }
    }
    cout << "]" << endl;
}

double ScalarField2D::checkBoundry(int x, int y) const {
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
    return scale*value(x,y);
}

void ScalarField2D::fill(double val) {
    for(double* ittr = itsData; ittr < itsData+xdim*ydim; ittr++)
        *ittr = val;
}
