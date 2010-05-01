/*
 * ScalarField2D.cpp
 *
 *  Created on: Apr 24, 2010
 *      Author: wto
 */

#include <algorithm>
#include <iostream>
#include <cmath>
using namespace std;

#include "ScalarField2D.h"

ScalarField2D::ScalarField2D(int xdim, int ydim, double boundryScale, double* data) :
        xdim(xdim), ydim(ydim), itsData(data), ownsData(false), itsBoundryScale(boundryScale) {
    if (itsData == 0) {
        ownsData = true;
        itsData = new double[size()];
    }
}

ScalarField2D::~ScalarField2D() {
    if (ownsData) {
        delete[] itsData;
    }
}

ScalarField2D::ScalarField2D(const ScalarField2D& other) :
        xdim(other.xdim), ydim(other.ydim), itsData(new double[size()]), ownsData(true), itsBoundryScale(other.itsBoundryScale)  {
    copy(other.begin(),other.end(),itsData);
}

ScalarField2D& ScalarField2D::operator=(const ScalarField2D& other) {
    if(other.xdim != xdim && other.ydim != ydim) {
        cerr << "Warning: Mismatched vector field dimensions." << endl;
        return *this;
    }

    itsBoundryScale = other.itsBoundryScale;
    copy(other.begin(),other.end(),itsData);
    return *this;
}

double ScalarField2D::min() const {
    return *min_element(begin(),end());
}

double ScalarField2D::max() const {
    return *max_element(begin(),end());
}

double ScalarField2D::mean() const {
    double ans = 0.0;
    for (int x = 0; x < xdim; x++) {
        for (int y = 0; y < ydim; y++) {
            ans += value(x,y);
        }
    }
    return ans / size();
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
    } else if (x > xdim-1) {
        scale = itsBoundryScale;
        x = xdim-1;
    }

    if (y < 0) {
        scale = itsBoundryScale;
        y = 0;
    } else if (y > ydim-1) {
        scale = itsBoundryScale;
        y = ydim-1;
    }

    return scale*value(x,y);
}

void ScalarField2D::fill(double val) {
    fill_n(itsData,size(),val);
}
