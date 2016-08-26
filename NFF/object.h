#ifndef _object_h
#define _object_h

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

typedef struct{
Wire **inj; // MAke this crap wires to keep the pointers addresses
Wire *inputs;
float output;
int input_size;
float *inputs_grad;
float output_grad;

int (*init)(void *self);
Wire (*forwardpass) (void *self, Wire **inputs);
void (*backwardpass) (void *self);
void (*destroy)(void *self);

}Object;


void Object_destroy(void *self);
int Object_init(void *self);
Wire Object_forwardpass (void *self, Wire **inputs);
void Object_backwardpass (void *self);
void *Object_new(size_t size, Object proto, char *type); // TO DO : Kill and replace




#define NEW(T, N) Object_new(sizeof(T), T##Proto, N)
#define _(N) proto.N
#endif
