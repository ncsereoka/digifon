#ifndef __FUZZY_LOGIC_CONTROLLER_H_
#define __FUZZY_LOGIC_CONTROLLER_H_

#include <omnetpp.h>
#include "Matrix.h"
#include "Term.h"

#define MAX_NR_TERMS 16
#define MAX_NR_INP 20

using namespace omnetpp;

namespace digifon {

class FuzzyLogicController: public cSimpleModule {
private:
    int bits_n; /* number of binary vectors representing a membership function */
    int bits_m; /* number of bits in each vector */
    int n_inp; /* number of inputs */
    int n_rules; /* number of rules */
    int nb_terms[MAX_NR_INP + 1];
    Term *m_functions[MAX_NR_INP + 1][MAX_NR_TERMS]; /* membership functions for input and output*/
    int **rules;
    cOutVector qtime, qtimew;
    cOutVector res_dep, result_dep;
public:
    ~FuzzyLogicController();
    int fuzzy_inference(int *inp, int nb_inp, int delta);
protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
private:
    Matrix* fuzzify(int crisp_in, int delta);
    int defuzify(Matrix *mat);
    double scale(double n, double m, double a, double b, double x);
};

}

#endif
