#ifndef _ex19_h
#define _ex19_h

#include "object.h"

struct Add {
  Object proto;
  //int input_size;
};

typedef struct Add Add;
float Add_forwardpass (void *self, Wire *inputs);
float *Add_backwardpass (void *self);

struct Mult {
  Object proto;
  //int input_size;
  float c;
};

typedef struct Mult Mult;

float Mult_forwardpass (void *self, Wire *inputs);
float *Mult_backwardpass (void *self);

struct FM1 {
  Object proto;
  float fs; // Fintersection if there is variance
  float m;
  float s; // this is sigma
};

typedef struct FM1 FM1;

float FM1_forwardpass (void *self, Wire *inputs);
float *FM1_backwardpass (void *self);
void *Membership1_New(size_t size, Object proto, float m, float s);

float *stager (float *a,float *b);
#define NEWFM1(T, M ,V) Membership1_New(sizeof(T), T##Proto, M, V)

#endif
