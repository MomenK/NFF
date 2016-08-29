#ifndef _ex19_h
#define _ex19_h

#include "object.h"

struct Add {
  Neuron proto;
};

typedef struct Add Add;
void Add_forwardpass (void *self);
void Add_backwardpass (void *self);

struct Mult {
  Neuron proto;
};

typedef struct Mult Mult;
void Mult_forwardpass (void *self);
void Mult_backwardpass (void *self);


struct FM1 {
  Neuron proto;
  float fs; // Fintersection POINT (NOT VALUE)if there is variance
  float m; // this mean
  float s; // this is sigma
};

typedef struct FM1 FM1;

void FM1_forwardpass (void *self);
void FM1_backwardpass (void *self);

void *Membership1_New(size_t size, Neuron proto, float m, float s,char *type,Bundle *inbun, Wire *outir);

float *stager (float *a,float *b);
#define NEWFM1(T, M ,V,C,X,W) Membership1_New(sizeof(T), T##Proto, M, V,C,X,W)

#endif
