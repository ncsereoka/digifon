#ifndef __MATRIX_H_
#define __MATRIX_H_

#include "Vector.h"

namespace digifon {

class Matrix {

private:
    int _m;
    int _n;
    int **mat;
public:
    Matrix(int m, int n);
    ~Matrix();
    int get_m() const;
    int get_n() const;
    void set(int i, int j, int val);
    int get(int i, int j) const;
    void print();
    //function that determines the maximum column of a matrix
    void max_column(Vector *v);
    //function that determines the minimum of 2 matrices
    void min_matrix(Matrix *m2, Matrix *result);
    //function that determines the maximum of 2 matrices
    void max_matrix(Matrix *m2, Matrix *result);
    //function that mins a matrix with a column vector
    void min_vector(Vector *v, Matrix *result);
};

}

#endif
