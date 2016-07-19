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

struct Mult {
  Object proto;
  //int input_size;
};

typedef struct Mult Mult;

float Mult_forwardpass (void *self, float *inputs);
float *Mult_backwardpass (void *self, float output_grad);

struct Fuzzier {
  Object proto;
  float s;
};

typedef struct Fuzzier Fuzzier;

float Fuzzier_forwardpass (void *self, float *inputs);
float *Fuzzier_backwardpass (void *self, float output_grad);


float *stager (float *a,float *b);

#endif
