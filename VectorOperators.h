/*
 * VectorOperators.h
 *
 *  Created on: Apr 25, 2010
 *      Author: wto
 */

#ifndef VECTOROPERATORS_H_
#define VECTOROPERATORS_H_

#include "VectorField2D.h"
#include "ScalarField2D.h"

void Del(const VectorField2D& f, VectorField2D& delF);
void Del(const ScalarField2D& f, VectorField2D& delF);

void DelDot(const VectorField2D& f, ScalarField2D& delDotF);
void DelCross(ScalarField2D& curlF, const VectorField2D& f);

#endif /* VECTOROPERATORS_H_ */
