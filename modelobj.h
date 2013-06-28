#ifndef MODELOBJ_H
#define MODELOBJ_H

#include "base.h"

struct color_type{
    int start;
    int mid1;
    int mid2;
    int end;
    int edge;
};

class ModelObj
{
public:
    ModelObj();
private:
    Base * theBase;
    float ambient;
    int shadeMode, fillMode;
    color_type color;

};

#endif // MODELOBJ_H
