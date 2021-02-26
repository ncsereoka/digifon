#include "Term.h"
#include <math.h>

namespace digifon {

Term::Term(int a, int b, int c, int d, std::string name) :
        _a(a), _b(b), _c(c), _d(d), _name(name) {
}

std::string Term::getName() const {
    return _name;
}

int Term::getTermRep(int m, int n, Matrix *mat) {
    int i, j;

    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++)
            mat->set(i, j, 0);

    if (_a < 0 || _a > n - 1 || _b < 0 || _b > n - 1 || _c < 0 || _c > n - 1
            || _d < 0 || _d > n - 1) {
        std::cout << "invalid term\n";
        return -1;
    }

    if (_a > _b || _b > _c || _c > _d) {
        std::cout << "coordinates are not ordered\n";
        return -1;
    }

    if (_a == _b) {
        for (i = 0; i < m; i++)
            mat->set(i, _a, 1);
    } else {
        float slope = (float) m / (float) (_b - _a);
        for (i = _a; i <= _b; i++) {
            int y = floor(slope * (i - _a));
            y = m - y;
            for (j = 0; j < y; j++)
                mat->set(j, i, 0);
            for (; j < m; j++)
                mat->set(j, i, 1);
        }
    }

    if (_b == _c) {
        for (i = 0; i < m; i++)
            mat->set(i, _b, 1);
    } else {
        for (i = _b; i <= _c; i++)
            for (j = 0; j < m; j++)
                mat->set(j, i, 1);
    }
    if (_c == _d) {
        for (i = 0; i < m; i++)
            mat->set(i, _c, 1);
    } else {
        float slope = (float) m / (float) (_d - _c);
        for (i = _c; i <= _d; i++) {
            int y = floor(slope * (_d - i));
            y = m - y;
            for (j = 0; j < y; j++)
                mat->set(j, i, 0);
            for (; j < m; j++)
                mat->set(j, i, 1);
        }
    }

    return 0;
}

}
