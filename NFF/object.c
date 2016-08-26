#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "object.h"
#include <assert.h>



Wire Neuron_forwardpass(void *self)
{
    printf("Nothing to forward pass\n" );
    exit;
}

void Neuron_backwardpass(void *self)
{
  printf("Nothing to backpropagate pass\n" );
  exit;
}

int Neuron_init(void *self)
{
  return 1;
}

void Neuron_destroy(void *self)
{
    Neuron *obj = self;

    if(obj) {
        free(obj);
    }
    //return NULL;
}

void *Neuron_new(size_t size, Neuron proto, char *type,Bundle *inbun, Wire *outir)
{

    if(!proto.init) proto.init = Neuron_init;
    if(!proto.forwardpass) proto.forwardpass = Neuron_forwardpass;
    if(!proto.backwardpass) proto.backwardpass = Neuron_backwardpass;
    if(!proto.destroy) proto.destroy = Neuron_destroy;


    Neuron *el = calloc(1,size);
    *el= proto;
    el->inbun = inbun;
    el->outir = outir;
    if(!el->init(el)) {
      el->destroy(el);
      return NULL;
    } else {
      return el;
    }
}



void Wire_destroy(void *self){
  Wire *wr = self;
  if(wr) {
      free(wr);
  }
}

void *Wire_new( float value, float grad){
  //if(!proto.destroy) proto.destroy = Wire_destroy;
//  if(!proto.init) proto.init = Neuron_init;

  Wire *el = calloc(1,sizeof(Wire));
  el->value = value;
  el->destroy = Wire_destroy;
  el->init = Neuron_init;
  el->grad = grad;

  return el;

}

Wire newWire(float value, float grad)
{

Wire Gimp; Gimp.value = value; Gimp.grad= grad;

return Gimp;
}

Bundle newBundle( size_t N)
{
  Bundle bun;
bun.size = N;
bun.addr = calloc(N, sizeof(Wire));
return bun;
}

Neuron newNeuron(Bundle *inbun, Wire *outir)
{
  Neuron Neu;
  Neu.inbun = inbun;
  Neu.outir = outir;
  return Neu;

}
