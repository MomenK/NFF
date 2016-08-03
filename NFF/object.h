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

typedef struct{
float *inputs;
float output;
int input_size;
float *inputs_grad;
float output_grad;

int (*init)(void *self);
float (*forwardpass) (void *self, float *inputs);
float *(*backwardpass) (void *self, float output_grad);
void (*destroy)(void *self);

}Object;


void Object_destroy(void *self);
int Object_init(void *self);
float Object_forwardpass (void *self, float *inputs);
float *Object_backwardpass (void *self, float output_grad);
void *Object_new(size_t size, Object proto, char *type); // TO DO : Kill and replace




#define NEW(T, N) Object_new(sizeof(T), T##Proto, N)
#define _(N) proto.N
#endif

/*

float forwardpass_add(void *self)
{
Object *obj = self;
float output = 0;
  for (int i =0; i < obj.input_size; i++)
    {
    ouput += inputs[i];
    }
    obj.output = output;
    return output;
}

should be in implementation part
float *backwardpass_add(void *self, float output_grad)
{
Object *obj = self;
for (int i =0; i < obj.input_size; i++)
  {
  obj.input_grad[i] = 1*output_grad;
  }
}


void *Object_new(size_t size, Object proto,float *inputs, char type)
{
  switch (type)
  {
    case 'a':
    if(!proto.forwardpass) proto.forwardpass = forwardpass_add;
  }
}
*/
