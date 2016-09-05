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


#define NEW(T, N,M,W) Neuron_new(sizeof(T), T##Proto, N,M,W)
#define _(N) proto.N



#define AdRef(T,N) &(*(Wire*)T.addr[N])
#define Ref(T,N) (*(Wire*)T.addr[N])
#define Refp(T,N) (*(Wire*)T->addr[N])
#define Wrap(B,W,I) B.addr[I]=(unsigned long int)&W


///
#endif
