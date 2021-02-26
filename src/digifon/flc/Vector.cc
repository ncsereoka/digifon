#include "Vector.h"
#include <iostream>

namespace digifon {

Vector::Vector(int m) :
        _m(m) {
    vect = new int[m];
}

Vector::~Vector() {
    delete[] vect;
}

int Vector::get_m() const {
    return _m;
}

int Vector::get(int j) const {
    return vect[j];
}

void Vector::set(int pos, int val) {
    vect[pos] = val;
}

void Vector::print() {
    for (int i = 0; i < _m; i++) {
        std::cout << vect[i] << " ";
    }
    std::cout << std::endl;
}

void Vector::min_vect(Vector *v2, Vector *result) {
    if (v2->_m != this->_m) {
        std::cout << "ERROR!!! vectors have different sizes\n";
    }

    for (int i = 0; i < this->_m; i++) {
        result->vect[i] =
                (this->vect[i] <= v2->vect[i]) ? this->vect[i] : v2->vect[i];
    }
}

}
