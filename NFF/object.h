#ifndef _object_h
#define _object_h

extern float step_size;
typedef struct{
  float value;
  float grad;
  int (*init)(void *self);
  void (*destroy)(void *self);
} Wire;


void Wire_destroy(void *self);
void *Wire_new( float value, float grad);
Wire newWire(float value, float grad);


typedef struct{
  unsigned long int *addr;
  size_t size;
} Bundle;

Bundle newBundle(size_t N);
void Bundleupdate(Bundle * bun);


typedef struct{
  Bundle *inbun; // MAke this crap wires to keep the pointers addresses
  Wire *outir;
  char * c;

int (*init)(void *self);
void (*forwardpass) (void *self);
void (*backwardpass) (void *self);
void (*destroy)(void *self);

}Neuron;

Neuron newNeuron(Bundle *inbun, Wire *outir);

void Neuron_destroy(void *self);
int Neuron_init(void *self);
void Neuron_forwardpass (void *self);
void Neuron_backwardpass (void *self);
void *Neuron_new(size_t size, Neuron proto, char *type,Bundle *inbun, Wire *outir); // TO DO : Kill and replace





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

extern Neuron AddProto;
extern Neuron MultProto;
extern Neuron FM1Proto;
extern Neuron LMSProto;

#define NEW(T, N,M,W) Neuron_new(sizeof(T), T##Proto, N,M,W)
#define _(N) proto.N



#define AdRef(T,N) &(*(Wire*)T.addr[N])
#define Ref(T,N) (*(Wire*)T.addr[N])
#define Refp(T,N) (*(Wire*)T->addr[N])
#define Wrap(B,W,I) B.addr[I]=(unsigned long int)&W

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
