#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ex19.h"

void die(const char *message)
{
  if(errno)
  perror(message);
  else
  printf("ERROR: %s\n", message);

  exit(1);
}

float *stager(float *a, float *b)
{
  float* total = malloc(4*( sizeof(float))); // array to hold the result

  memcpy(total,     a,  2*sizeof(float)); // copy 4 floats from x to total[0]...total[3]
  memcpy(total + 2, b,  2*sizeof(float)); // copy 4 floats from y to total[4]...total[7]
printf("%zd\n",sizeof(total)/sizeof(float));
  return total;
}

float Add_forwardpass (void *self, float *inputsww){
  Object *obj = self;
//  obj->input_size = sizeof(inputsww)/sizeof(float) ;
//  printf("size I received %zd\n",  sizeof(inputsww)/sizeof(float));
// THIS DOESN"T WORK BECAUSE IT"S ALWAYS THE SIZE OF THE POINT -> 16 bit "addrees" !
  float output = 0;
   obj->inputs = malloc(obj->input_size*sizeof(float));
    for (int i =0; i < obj->input_size; i++)
      {
      obj->inputs[i]=inputsww[i];
      output += obj->inputs[i];
      }
      obj->output = output;
      return output;
}

float *Add_backwardpass (void *self, float output_grad){
  Object *obj = self;
  obj->inputs_grad = malloc(obj->input_size*sizeof(float));
  for (int i =0; i < obj->input_size; i++)
    {
    obj->inputs_grad[i] = 1*output_grad;
    }
    return obj->inputs_grad;
}


Object AddProto= {
  .forwardpass = Add_forwardpass,
  .backwardpass = Add_backwardpass
};


float Mult_forwardpass (void *self, float *inputsww){
  Object *obj = self;
//  obj->input_size = sizeof(inputsww)/sizeof(float) ;
//  printf("size I received %zd\n",  sizeof(inputsww)/sizeof(float));
// THIS DOESN"T WORK BECAUSE IT"S ALWAYS THE SIZE OF THE POINT -> 16 bit "addrees" !
  float output = 1;
   obj->inputs = malloc(obj->input_size*sizeof(float));
   if(obj->input_size != 2)
   {
     die("Not Valid input number for a multiply gate!");
   }
    for (int i =0; i < obj->input_size; i++)
      {
      obj->inputs[i]=inputsww[i];
    //  printf("%d :%f\n",i, inputsww[i] );
      output *= obj->inputs[i];
      }
      obj->output = output;
      return output;
}

float *Mult_backwardpass (void *self, float output_grad){
  Object *obj = self;
  obj->inputs_grad = malloc(obj->input_size*sizeof(float));
  for (int i =0; i < obj->input_size; i++)
    {
    obj->inputs_grad[i] = obj->inputs[1-i];
    }
    return obj->inputs_grad;
}


Object MultProto= {
  .forwardpass = Mult_forwardpass,
  .backwardpass = Mult_backwardpass,
  .input_size = 2
};


int main(int argc, char *argv[])
{
  float step_size = 0.01;
  float g1[] = {-2, 5};
  float g2[2];
        g2[1]= -4;

  for (size_t i = 0; i < 5; i++) {
    /* code */

  //*********************************************forwardpass
  /////////gate1
  Add *gate1 = NEW(Add, "Layer 1 gate");
  gate1->_(input_size) =2;
  ////Connection
  g2[0]= gate1->_(forwardpass)(gate1, g1);
  //  printf("output %f\n",g2[0]);



  //////////////////////////////gate2
  Mult *gate2 = NEW(Mult, "Layer 2 gate");
  printf("output %f\n",gate2->_(forwardpass)(gate2, g2));




  ///******************************************************** backprogate
  gate2->_(backwardpass)(gate2,1);
  gate1->_(backwardpass)(gate1, gate2->_(inputs_grad[0]));



  ///***************************************************************************************update
  g2[1]+= gate2->_(inputs_grad[1])* step_size;
  g1[0]+= gate1->_(inputs_grad[0]) * step_size;
  g1[1]+= gate1->_(inputs_grad[0]) * step_size;

  printf("update x[0] %f\n",g1[0]);
  printf("update x[1]%f\n",g1[1]);
  printf("update y[1] %f\n",g2[1]);

  //printf("output %f\n",g2[1]*(g1[0]+g1[1]));
  }
}




/*
Example of Usage:

float step_size = 0.01;
Add *gate1 = NEW(Add,"gate done");

printf("Gate created\n" );
//float intial_inputs[] = {1.2,1.0,5};

float a[] ={1.0, 2.4};
float b[] = {5.0, 0.0};
gate1->_(input_size) =4; // need a better way man
float *intial_inputs = stager(a,b);

//gate1->_(inputs) = ii;
//ii[1]= 7.0;
//printf("%f\n",gate1->_(inputs[1]));
printf("output %f\n",gate1->_(forwardpass)(gate1, intial_inputs));
gate1->_(backwardpass)(gate1,1);
for (size_t i = 0; i < gate1->_(input_size); i++) {
  printf("Gradint for the %zd input is %f\n", i, gate1->_(inputs_grad[i]) );
      }

gate1->_(destroy)(gate1);

float x[] = {-2.0,3.0};
Mult *gate2 = NEW(Mult, "gate at layer 1");
printf("Gate created\n" );

//gate2->_(input_size) =2;

printf("output %f\n",gate2->_(forwardpass)(gate2, x));

gate2->_(backwardpass)(gate2,1);

for (size_t i = 0; i < gate2->_(input_size); i++) {
  printf("Gradint for the %zd input is %f\n", i, gate2->_(inputs_grad[i]) );

  printf("GUdated vale for the %zd input is %f\n", i, gate2->_(inputs[i]) + step_size * gate2->_(inputs_grad[i]) );

      }/**/
