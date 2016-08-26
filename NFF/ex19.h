#ifndef _ex19_h
#define _ex19_h

#include "object.h"

struct Add {
  Neuron proto;
};

typedef struct Add Add;
Wire Add_forwardpass (void *self);
void Add_backwardpass (void *self);
/*
struct Mult {
  Neuron proto;
  //int input_size;
  float c;
};

typedef struct Mult Mult;

Wire Mult_forwardpass (void *self, Wire *inputs);
float *Mult_backwardpass (void *self);

struct FM1 {
  Neuron proto;
  float fs; // Fintersection if there is variance
  float m; // this mean
  float s; // this is sigma
};

typedef struct FM1 FM1;

float FM1_forwardpass (void *self, Wire *inputs);
float *FM1_backwardpass (void *self);
void *Membership1_New(size_t size, Neuron proto, float m, float s);
*/
float *stager (float *a,float *b);
#define NEWFM1(T, M ,V) Membership1_New(sizeof(T), T##Proto, M, V)

#endif
