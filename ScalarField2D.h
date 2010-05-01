/*
 * ScalarField2D.h
 *
 *  Created on: Apr 25, 2010
 *      Author: wto
 */

#ifndef SCALARFIELD2D_H_
#define SCALARFIELD2D_H_

class ScalarField2D {
public:
        ScalarField2D(int xdim, int ydim, double boundryScale = 1, double* data = 0);
	~ScalarField2D();


        ScalarField2D(const ScalarField2D&);
        ScalarField2D& operator=(const ScalarField2D&);

	double& value(int x, int y);
	double value(int x, int y) const;
        double checkBoundry(int x, int y) const;

        void print() const;
        operator double*() const;
        double min() const;
        double max() const;
        void fill(double val);

	const int xdim;
	const int ydim;

private:
	double *itsData;
	bool ownsData;
        double itsBoundryScale;
};

inline ScalarField2D::operator double*() const {
    return itsData;
}

inline double& ScalarField2D::value(int x, int y) {
	return itsData[(y * xdim) + x];
}

inline double ScalarField2D::value(int x, int y) const {
	return itsData[(y * xdim) + x];
}

#endif /* SCALARFIELD2D_H_ */
