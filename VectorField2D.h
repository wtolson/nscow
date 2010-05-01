/*
 * VectorField2D.h
 *
 *  Created on: Apr 25, 2010
 *      Author: wto
 */

#ifndef VECTORFIELD2D_H_
#define VECTORFIELD2D_H_

class VectorField2D {
public:
        VectorField2D(int xdim, int ydim, double boundryScale = 1, double* data = 0);
	~VectorField2D();        

        VectorField2D(const VectorField2D&);
        VectorField2D& operator=(const VectorField2D&);

	double& value(int x, int y, int k);
	double value(int x, int y, int k) const;
        double checkBoundry(int x, int y, int k) const;

        void print() const;

        double *begin() const;
        double *end() const;
        int size() const;

        double min() const;
        double max() const;
        double mean() const;

        void fill(double val);

	const int xdim;
	const int ydim;

private:
	double *itsData;
	bool ownsData;
        double itsBoundryScale;
};

inline double *VectorField2D::begin() const {
    return itsData;
}

inline double *VectorField2D::end() const {
    return itsData+size();
}

inline int VectorField2D::size() const {
    return 2*xdim*ydim;
}

inline double& VectorField2D::value(int x, int y, int k) {
        return itsData[(k * xdim * ydim) + (y * xdim) + x];
}

inline double VectorField2D::value(int x, int y, int k) const {
        return itsData[(k * xdim * ydim) + (y * xdim) + x];
}

#endif /* VECTORFIELD2D_H_ */
