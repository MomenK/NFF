#ifndef _ex19_h
#define _ex19_h

#include "object.h"

struct Add {
  Object proto;
  //int input_size;
};

typedef struct Add Add;

float Add_forwardpass (void *self, float *inputs);
float *Add_backwardpass (void *self, float output_grad);

#endif
