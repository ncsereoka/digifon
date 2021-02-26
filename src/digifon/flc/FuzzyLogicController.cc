#include "FuzzyLogicController.h"
#include <string.h>
#include <omnetpp.h>
#include <set>
#include <string>
#include <iostream>
using namespace std;
using namespace omnetpp;

namespace digifon {

Define_Module(FuzzyLogicController);

FuzzyLogicController::~FuzzyLogicController() {
    for (int i = 0; i < MAX_NR_INP + 1; i++) {
        for (int j = 0; j < MAX_NR_TERMS; j++) {
            delete m_functions[i][j];
        }
    }

    for (int i = 0; i < n_rules; i++) {
        delete[] rules[i];
    }

    delete[] rules;
}

void FuzzyLogicController::initialize() {
    qtime.setName("delay vector");
    qtimew.setName("weight vector");
    result_dep.setName("result nescalat");
    res_dep.setName("res scalat");

    cXMLElement *rootelement = par("config").xmlValue();
    EV << "initialize FLC\n";
    cXMLElement *current = rootelement->getElementByPath("./n_bits");

    bits_n = atoi(current->getFirstChild()->getNodeValue());
    EV << "bits n:" << bits_n << ".\n";

    current = rootelement->getElementByPath("./m_bits");

    bits_m = atoi(current->getFirstChild()->getNodeValue());
    EV << "bits m:" << bits_m << ".\n";

    current = rootelement->getElementByPath("./n_inp");

    n_inp = atoi(current->getFirstChild()->getNodeValue());
    if (n_inp > MAX_NR_INP)
        EV << "error: too many inputs\n";

    EV << "nb inputs:" << n_inp << ".\n";

    current = rootelement->getElementByPath("./n_rules");
    n_rules = atoi(current->getFirstChild()->getNodeValue());
    EV << "nb rules:" << n_rules << ".\n";

    //read fuzzy sets for inputs and output
    cXMLElementList list = rootelement->getElementsByTagName("fuzzy_set");

    int i_input = 0;
    for (cXMLElementList::iterator it = list.begin(); it != list.end(); it++) {
        cXMLElement *el = *it;
        EV << "fuzzy set type, number of terms " << el->getAttribute("type")
                  << "," << el->getAttribute("nterms") << "\n";
        nb_terms[i_input] = atoi(el->getAttribute("nterms"));
        /* error situation */
        if (nb_terms[i_input] > MAX_NR_TERMS)
            EV << "error : too many terms\n";
        //read terms
        cXMLElement *term = el->getFirstChild();

        int x[4];
        string name;
        for (int i_term = 0; i_term < nb_terms[i_input]; i_term++) {
            cXMLElement *et = term->getFirstChild();
            name = et->getNodeValue();
            EV << "term name:" << name << "\n";
            for (int i = 0; i < 4; i++) {
                et = et->getNextSibling();
                x[i] = atoi(et->getNodeValue());
                EV << "x coordinate:" << x[i] << "\n";
            }
            Term *t = new Term(x[0], x[1], x[2], x[3], name);
            m_functions[i_input][i_term] = t;
            term = term->getNextSibling();
        }
        i_input++;
    }

    //read rules
    rules = new int*[n_rules];
    for (int k = 0; k < n_rules; k++)
        rules[k] = new int[n_inp + 1];

    list = rootelement->getElementsByTagName("rule");
    int i_rule = 0;
    for (cXMLElementList::iterator it = list.begin(); it != list.end(); it++) {
        cXMLElement *el = *it;
        cXMLElement *ef = el->getFirstChild();
        string term_name;

        //read premises
        for (int i = 0; ef != NULL && i < n_inp;
                i++, ef = ef->getNextSibling()) {
            term_name = ef->getNodeValue();
            EV << "premise: " << term_name;
            //find the index of the term in premise
            int found = 0;
            for (int j = 0; j < nb_terms[i]; j++) {
                Term *t = m_functions[i][j];
                if (term_name.compare(t->getName()) == 0) {
                    found = 1;
                    rules[i_rule][i] = j; /* remember index of term */
                    //cout<< "rules["<<i_rule<<"]["<<i<<"]="<<j<<"\n";
                }
            }
            if (found == 0)
                cout << "ERROR: term name(premise) not found\n";

        }
        //read conclusion
        int i = n_inp;
        EV << "conclusion: " << ef->getNodeValue() << "\n";
        term_name = ef->getNodeValue();
        int found = 0;
        for (int j = 0; j < nb_terms[n_inp]; j++) {
            Term *t = m_functions[i][j];
            if (term_name.compare(t->getName()) == 0) {
                found = 1;
                rules[i_rule][n_inp] = j;
            }
        }
        if (found == 0)
            cout << "ERROR: term name(conclusion) not found\n";

        i_rule++;
    }
}

Matrix* FuzzyLogicController::fuzzify(int crisp_in, int delta) {
    int i, j;
    Matrix *matrix = new Matrix(this->bits_m, this->bits_n);

    EV << "Fuzzify input\n";
    for (i = 0; i < this->bits_m; i++)
        for (j = 0; j < this->bits_n; j++)
            matrix->set(i, j, 0);

    if (crisp_in < 0 || crisp_in > this->bits_n - 1) {
        printf("invalid input\n");
        return NULL;
    }

    if (delta == 0) {
        for (i = 0; i < this->bits_m; i++)
            matrix->set(i, crisp_in, 1);
        return matrix;
    }

    float slope = (float) this->bits_m / (float) delta;
    for (i = 0; i < delta; i++) {
        int y = floor((delta - i) * slope);
        y = this->bits_m - y;
        if (crisp_in + i < this->bits_n) {
            for (j = 0; j < y; j++)
                matrix->set(j, crisp_in + i, 0);
            for (; j < this->bits_m; j++)
                matrix->set(j, crisp_in + i, 1);
        }
        if (crisp_in - i >= 0) {
            for (j = 0; j < y; j++)
                matrix->set(j, crisp_in - i, 0);
            for (; j < this->bits_m; j++)
                matrix->set(j, crisp_in - i, 1);

        }
    }

    return matrix;
}

int FuzzyLogicController::defuzify(Matrix *mat) {
    int result, suma, val, i, j;
    result = suma = 0;

    for (i = bits_n - 1; i >= 0; i--) {
        val = 0;
        for (j = 0; j < bits_m; j++)
            val += mat->get(j, i);
        suma += val;
        result += suma;
    }

    if (suma != 0)
        return result / suma;
    else
        return 0;
}

double FuzzyLogicController::scale(double n, double m, double a, double b,
        double x) {
    double res;
    if ((b <= a) || (m <= 0))
        exit(1);
    else if (x <= a)
        res = n;
    else if (x >= b)
        res = m;
    else
        res = ((x - a) / (b - a)) * (m - n) + n;
    return res;

}

int FuzzyLogicController::fuzzy_inference(int *inp, int nb_inp, int delta) {
    int i, j;

    if (nb_inp != this->n_inp)
        EV << "ERROR: number of inputs differs from expected\n";

    Matrix **inp_f = new Matrix*[this->n_inp];

    for (i = 0; i < n_inp; i++) {
        inp_f[i] = fuzzify(inp[i], delta); //delta = 4
        if (inp_f[i] == NULL) {
            EV << "Error fuzzifying crisp in\n";
            exit(1);
        }
    }

    Matrix minMat(this->bits_m, this->bits_n);
    Matrix maxMat(this->bits_m, this->bits_n);
    Vector maxVect(this->bits_m);
    Vector minMax(this->bits_m);

    Matrix *term = new Matrix(this->bits_m, this->bits_n);

    for (i = 0; i < this->bits_m; i++)
        for (j = 0; j < this->bits_n; j++)
            maxMat.set(i, j, 0);

    for (i = 0; i < this->n_rules; i++) {
        for (j = 0; j < this->bits_m; j++)
            minMax.set(j, 1);

        for (j = 0; j < this->n_inp; j++) {
            if (m_functions[j][this->rules[i][j]]->getTermRep(this->bits_m,
                    this->bits_n, term) < 0)
                EV << "ERROR: error creating term\n";
            term->min_matrix(inp_f[j], &minMat);
            minMat.max_column(&maxVect);
            minMax.min_vect(&maxVect, &minMax);

        }
        m_functions[n_inp][rules[i][n_inp]]->getTermRep(bits_m, bits_n, term);
        term->min_vector(&minMax, &minMat);
        minMat.max_matrix(&maxMat, &maxMat);

    }

    //maxMat.print();

    delete term;

    for (i = 0; i < n_inp; i++)
        delete inp_f[i];
    int result = defuzify(&maxMat);
    return result;
}

void FuzzyLogicController::handleMessage(cMessage *msg) {
    if (!strcmp(msg->getName(), "start_flc")) {

        EV << "Calculez nou HP" << endl;
        int wantedDelay = 10; //(int)getParentModule()->par("delayLimit");
        int currentDelay = 15; //round((double)getParentModule()->getSubmodule("netwrk")->par("meanDelayHP"));
        int W_HP = 4; //(int)getParentModule()->getSubmodule("hp_fifo")->par("weight");
        int B = 31; //(int)getParentModule()->getSubmodule("netwrk")->par("B");

        int new_W_HP = W_HP;
        int diff = wantedDelay - currentDelay;

        qtime.record(currentDelay);
        EV << " Dif nescalat = " << diff << "\n";

        diff = scale(0, 62, -10, 10, diff);
        W_HP = scale(0, 62, 0, B, W_HP);
        EV << " Dif scalat = " << diff << "\n";

        int delta = 0; //(int) getParentModule()->par("delta");
        int inp[2] = { diff, W_HP };

        int result = fuzzy_inference(inp, 2, delta);
        result_dep.record(result);

        int res = round(scale((B * -1) / 2, B / 2, 0, 62, result));
        EV << " Result = " << result << "\nRes= " << res << "\n";

        res_dep.record(res);

        new_W_HP = new_W_HP + res;

        if (new_W_HP > B)
            new_W_HP = B - 1;
        if (new_W_HP < 1)
            new_W_HP = 1;

        /* not for test
         cPar& W_HP_r = getParentModule()->getSubmodule("hp_fifo")->par("weight");
         W_HP_r.setIntValue(new_W_HP);
         */
        EV << "Pondere noua: " << new_W_HP << "\n\n";

        qtimew.record(new_W_HP);
        //cMessage *job = new cMessage("clear");
        //sendDirect(job, getParentModule()->getSubmodule("netwrk")->gate("in"));
        delete msg;
    }
}

}
