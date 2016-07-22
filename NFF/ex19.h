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

struct FM1 {
  Object proto;
  float fs; // Fintersection if there is variance
  float m;
  float s;
};

typedef struct FM1 FM1;

float FM1_forwardpass (void *self, float *inputs);
float *FM1_backwardpass (void *self, float output_grad);


float *stager (float *a,float *b);

#endif
