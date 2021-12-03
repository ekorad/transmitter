#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

#include "rtwtypes.h"

struct DataTransferObject
{
    DataTransferObject() {}
    DataTransferObject(real_T xcp, real_T zcp, real_T yrp)
        : xl{xcp}, zl{zcp}, yr{yrp} {}

    real_T xl;
    real_T zl;
    real_T yr;
};

#endif