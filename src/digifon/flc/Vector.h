#ifndef __VECTOR_H_
#define __VECTOR_H_

namespace digifon {

class Vector {
public:
    Vector(int size);
    ~Vector();
    int get_m() const;
    int get(int j) const;
    void set(int pos, int val);
    void print();
    void min_vect(Vector *v2, Vector *result);
private:
    int _m;
    int *vect;
};

}

#endif
