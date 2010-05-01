/*
 * VectorOperators.cpp
 *
 *  Created on: Apr 25, 2010
 *      Author: wto
 */
#include <iostream>
using namespace std;
#include "VectorOperators.h"

void Del(const VectorField2D& f, VectorField2D& delF) {
    for (int y = 0; y < f.ydim; y++) {
        for (int x = 0; x < f.xdim; x++) {
            delF.value(x, y, 0) = (f.checkBoundry(x+1, y, 0) - f.checkBoundry(x-1, y, 0)) / 2.0;
            delF.value(x, y, 1) = (f.checkBoundry(x, y+1, 1) - f.checkBoundry(x, y-1, 1)) / 2.0;
        }
    }
}

void Del(const ScalarField2D& f, VectorField2D& delF) {
    for (int y = 0; y < f.ydim; y++) {
        for (int x = 0; x < f.xdim; x++) {
            delF.value(x, y, 0) = (f.checkBoundry(x+1, y) - f.checkBoundry(x-1, y)) / 2.0;
            delF.value(x, y, 1) = (f.checkBoundry(x, y+1) - f.checkBoundry(x, y-1)) / 2.0;
        }
    }
}

void DelDot(const VectorField2D& f, ScalarField2D& delDotF) {

    for (int y = 0; y < f.ydim; y++) {
        for (int x = 0; x < f.xdim; x++) {
            delDotF.value(x, y) = (f.checkBoundry(x + 1, y, 0) - f.checkBoundry(x - 1, y, 0)) / 2;            
            delDotF.value(x, y) += (f.checkBoundry(x, y+1, 1) - f.checkBoundry(x, y-1, 1)) / 2;
        }
    }

}

void DelCross(ScalarField2D& curlF, const VectorField2D& f) {
    for(int x = 0; x < f.xdim; x++) {
        for(int y = 0; y < f.ydim; y++) {
            curlF.value(x,y) = f.checkBoundry(x+1,y,1) - f.checkBoundry(x-1,y,1) - f.checkBoundry(x,y+1,0) + f.checkBoundry(x,y-1,0);
        }
    }

}
