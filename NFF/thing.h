#ifndef _thing_h
#define _thing_h

#include "object.h"
float step_size=0.01;
int epoch = 10;

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


struct LMS {
  Neuron proto;
  float *Weight;
};
typedef struct LMS LMS;
void LMS_forwardpass(void *self);
void LMS_backwardpass(void *self);
void *LMS_new(size_t size, Neuron proto, char *type,Bundle *inbun, Wire *outir, float *weight);

Neuron AddProto= {
  .forwardpass = Add_forwardpass,
  .backwardpass = Add_backwardpass
};
Neuron MultProto= {
  .forwardpass = Mult_forwardpass,
  .backwardpass = Mult_backwardpass
};
Neuron FM1Proto= {
  .forwardpass = FM1_forwardpass,
  .backwardpass = FM1_backwardpass
};
Neuron LMSProto= {
  .forwardpass = LMS_forwardpass,
  .backwardpass = LMS_backwardpass
};


#define NEWLMS(T, N,M,W,F) LMS_new(sizeof(T), T##Proto, N,M,W,F)

#define NEWFM1(T, M ,V,C,X,W) Membership1_New(sizeof(T), T##Proto, M, V,C,X,W)

#define AdRef(T,N) &(*(Wire*)T.addr[N])
#define Ref(T,N) (*(Wire*)T.addr[N])
#define Refp(T,N) (*(Wire*)T->addr[N])
#define Wrap(B,W,I) B.addr[I]=(unsigned long int)&W
#define fb(X) X->_(forwardpass)(X)
#define bb(X) X->_(backwardpass)(X)

#define gr(N) inputs_grad[N]
#define sqr(N) \
      ({  __typeof__ (N) _N = (N); \
          _N*_N; })
#define tri(N) \
      ({  __typeof__ (N) _N = (N); \
          _N*_N*_N; })
#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#endif
