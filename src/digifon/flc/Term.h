#ifndef __TERM_H_
#define __TERM_H_

#include <string.h>
#include <iostream>
#include "Matrix.h"

namespace digifon {

class Term {
private:
    std::string _name;
    int _a, _b, _c, _d;

public:
    Term(int a, int b, int c, int d, std::string name);
    std::string getName() const;
    int getTermRep(int m, int n, Matrix *mat);

};

}

#endif
