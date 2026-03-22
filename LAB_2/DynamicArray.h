#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

#include "Date.h"

class DynamicArray {
private:
    Date** data;      
    int    size;
    int    capacity;

    void resize();

public:
    DynamicArray();
    ~DynamicArray();

    void  push(Date* d);
    Date* get(int index) const;
    int   getSize() const;
};

#endif