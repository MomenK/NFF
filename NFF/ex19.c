#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ex19.h"


float Add_forwardpass (void *self, float *inputs){
  Object *obj = self;
  //printf("%zd\n",sizeof(inputs)/sizeof(float) );
  obj->input_size = sizeof(inputs)/sizeof(float) ;
  float output = 0;
    for (int i =0; i < obj->input_size; i++)
      {
      output += inputs[i];
      }
      obj->output = output;
      return output;
}

float *Add_backwardpass (void *self, float output_grad){
  Object *obj = self;
  obj->input_grad = malloc(obj->input_size*sizeof(float));
  for (int i =0; i < obj->input_size; i++)
    {
    obj->input_grad[i] = 1*output_grad;
    }
    return obj->input_grad;
}

Object AddProto= {
  .forwardpass = Add_forwardpass,
  .backwardpass = Add_backwardpass
};

int main(int argc, char *argv[])
{

Add *gate1 = NEW(Add,"gate done");

printf("Gate created\n" );

float intial_inputs[2] = { 1.3, 3.5};
//gate1->_(inputs) = intial_inputs;
//ii[1]= 7.0;
//printf("%f\n",gate1->_(inputs[1]));
printf("output %f\n",gate1->_(forwardpass)(gate1, intial_inputs));
gate1->_(backwardpass)(gate1,1);
for (size_t i = 0; i < gate1->_(input_size); i++) {
  printf("Gradint for the %zd input is %f\n", i, gate1->_(input_grad[i]) );
}



}
