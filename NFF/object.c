#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "object.h"
#include <assert.h>



float Object_forwardpass(void *self, float *inputs)
{
    printf("Nothing to forward pass\n" );
    exit;
}

float *Object_backwardpass(void *self, float output_grad)
{
  printf("Nothing to backpropagate pass\n" );
  exit;
}

int Object_init(void *self)
{
  return 1;
}

void Object_destroy(void *self)
{
    Object *obj = self;

    if(obj) {
        free(obj);
    }
    //return NULL;
}

void *Object_new(size_t size, Object proto, char *type)
{

    if(!proto.init) proto.init = Object_init;
    if(!proto.forwardpass) proto.forwardpass = Object_forwardpass;
    if(!proto.backwardpass) proto.backwardpass = Object_backwardpass;
    if(!proto.destroy) proto.destroy = Object_destroy;
    if(!proto.input_size) proto.input_size = 2;

    Object *el = calloc(1,size);
    *el= proto;

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
//  if(!proto.init) proto.init = Object_init;

  Wire *el = calloc(1,sizeof(Wire));
  el->value = value;
  el->destroy = Wire_destroy;
  el->init = Object_init;
  el->grad = grad;

  return el;

}
