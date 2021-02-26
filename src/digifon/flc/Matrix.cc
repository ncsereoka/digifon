#include "Matrix.h"
#include <iostream>

namespace digifon {

Matrix::Matrix(int m, int n) :
        _m(m), _n(n) {
    mat = new int*[_m];
    for (int i = 0; i < _m; i++) {
        mat[i] = new int[_n];
    }
}

Matrix::~Matrix() {
    for (int i = 0; i < _m; i++) {
        delete[] mat[i];
    }

    delete[] mat;
}

int Matrix::get_m() const {
    return _m;
}

int Matrix::get_n() const {
    return _n;
}

void Matrix::set(int i, int j, int val) {
    mat[i][j] = val;
}

int Matrix::get(int i, int j) const {
    return mat[i][j];
}

void Matrix::print() {
    for (int i = 0; i < _m; i++) {
        for (int j = 0; j < _n; j++) {
            std::cout << mat[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void Matrix::min_matrix(Matrix *m2, Matrix *result) {
    //cout<<"min matrix>>\n";
    if (this->_m != m2->_m || this->_n != m2->_n) {
        std::cout << "ERROR!!! matrices have different dimensions\n";
    }

    for (int i = 0; i < this->_m; i++) {
        for (int j = 0; j < this->_n; j++) {
            result->mat[i][j] =
                    (this->mat[i][j] <= m2->mat[i][j]) ?
                            this->mat[i][j] : m2->mat[i][j];
        }
    }
}

void Matrix::max_matrix(Matrix *m2, Matrix *result) {
    //cout<<"max matrix\n";
    if (this->_m != m2->_m || this->_n != m2->_n) {
        std::cout << "ERROR!!! matrices have different dimensions\n";
    }
    for (int i = 0; i < this->_m; i++)
        for (int j = 0; j < this->_n; j++)
            result->mat[i][j] =
                    (this->mat[i][j] >= m2->mat[i][j]) ?
                            this->mat[i][j] : m2->mat[i][j];
}

void Matrix::max_column(Vector *v) {
    int i;
    if (this->_m != v->get_m()) {
        std::cout << "ERROR!!!dimensions not matching\n";
    }

    for (i = 0; i < this->_m; i++)
        v->set(i, 0);

    for (i = 0; i < this->_n; i++)
        for (int j = 0; j < this->_m; j++)
            v->set(j,
                    (this->mat[j][i] >= v->get(j)) ?
                            this->mat[j][i] : v->get(j));
}

void Matrix::min_vector(Vector *v, Matrix *result) {
    int i, j;
//cout<<"min vector\n";
    for (i = 0; i < this->_n; i++)
        for (j = 0; j < this->_m; j++)
            result->mat[j][i] =
                    (v->get(j) <= this->mat[j][i]) ?
                            v->get(j) : this->mat[j][i];
}

}
