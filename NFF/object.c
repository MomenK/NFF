#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "object.h"
#include <assert.h>

void Object_destroy(void *self)
{
    Object *obj = self;

    if(obj) {
        free(obj);
    }
}


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

void *Object_new(size_t size, Object proto, char *type)
{

    if(!proto.init) proto.init = Object_init;
    if(!proto.forwardpass) proto.forwardpass = Object_forwardpass;
    if(!proto.backwardpass) proto.backwardpass = Object_backwardpass;

    Object *el = calloc(1,size);
    *el= proto;

    if(!el->init(el)) {
      el->destroy(el);
      return NULL;
    } else {
      return el;
    }




}
