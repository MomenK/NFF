#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "object.h"
#include <assert.h>
#include <time.h>
#include <math.h>
#include <errno.h>
extern float step_size;

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


void Neuron_forwardpass(void *self)
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
    el->c = type;
    if(!el->init(el)) {
      el->destroy(el);
      return NULL;
    } else {
      return el;
    }
}


Neuron newNeuron(Bundle *inbun, Wire *outir)
{
  Neuron Neu;
  Neu.inbun = inbun;
  Neu.outir = outir;
  return Neu;

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
if (N!=1)
{
bun.addr = calloc(N, sizeof(Wire));
}
else
{
bun.addr = calloc(2, sizeof(Wire));
Wire Zero = newWire(0,0);
Wrap(bun,Zero,1);
}
return bun;
}

void Bundleupdate(Bundle *bun)
{
for(int i=0; i< bun->size; i++)
  {
    Refp(bun,i).value += step_size*Refp(bun,i).grad;
  //  printf("%f\n",Refp(bun,i).grad );
  }

}



float e(float n){return expf(n);}

  float N (float x, float m, float v)
     {
       return e(-0.5 * sqr((x-m)/v));
     }


  float solve (float m1, float v1, float m2, float v2)
     {

     float s1 = v1*v1;
     float s2 = v2*v2;

     float a = 1/(2*s1) - 1/(2*s2);
     float b = (m2/s2) - (m1/s1);
     float c = sqr(m1)/(2*s1) - sqr(m2)/(2*s2);

     float delta = b*b - 4*a*c;
     float o1 =0;
     float o2 =0;
     float o=0;
     int flag =1;
if(b==0)
{
  o1 = m1;
  o2 = m2;
}
else{     if(a == 0)
     {
       o1 = -c/(b);
       o2 = -c/(b);
     }
     else {
         if(delta == 0){
          printf(" Here\n" );
            o1 = -b/(2*a);
            o2 = -b/(2*a);
         }
         else if(delta >0)
         {
            o1 = -b + sqrt(delta); o1 = o1/(2*a);
            o2 = -b - sqrt(delta);  o2 = o2/(2*a);
         }
         else
         {
           flag = 0;
         }
     }
   }
    // printf("here!!!!");
     o = e(-0.5 * sqr((o1-m1)/v1)) >  e(-0.5 * sqr((o2-m1)/v1))  ? o1 : o2 ;
    //printf("%f ,  %f \n", e(-0.5 * sqr((o1-m1)/v1)) ,  e(-0.5 * sqr((o2-m1)/v1)) );
  //  printf("%f ,  %f \n", N(o1,m1,v1) ,  N(o2,m1,v1) );
     printf("\t\t\t\t\t\t\tintersection :%s | at %f with value of : %f\n ", flag ? "true" : "false", o , N(o, m1,v1) );
     return  o;
     }

/////////////////////////////////////////ADD
void Add_forwardpass (void *self){
  Neuron *obj = self;
  obj->outir->value = 0;
//  obj->c = "haha";
//printf("\nGate :%s \n",obj->c );

   for (int i =0; i < obj->inbun->size; i++)
      {
  //  printf("\t\tinput %d :  %f\n",i, Refp(obj->inbun,i).value );
      obj->outir->value += Refp(obj->inbun,i).value;
        Refp(obj->inbun,i).grad =0;
      }
//printf("\t\t\toutput is: --> %f\n", obj->outir->value);
}
//
void Add_backwardpass (void *self){
Neuron *obj = self;

for (int i =0; i < obj->inbun->size; i++)
    {
  Refp(obj->inbun,i).grad += 1* obj->outir->grad;
//  printf("grad %d :  %f\n",i, Refp(obj->inbun,i).grad);

    }
}
//
//

//
// //
void Mult_forwardpass (void *self){
  Neuron *obj = self;
  obj->outir->value = 1;
  //printf("\nGate :%s \n",obj->c );
   for (int i =0; i < obj->inbun->size; i++)
      {
    //printf("\t\tinput %d :  %f\n",i, Refp(obj->inbun,i).value );
      obj->outir->value *= Refp(obj->inbun,i).value;
      Refp(obj->inbun,i).grad =0;
      }
//printf("\t\t\toutput is: --> %f\n", obj->outir->value);
}

void Mult_backwardpass (void *self){

  Neuron *obj = self;
  float x =1;
  //   //obj->inputs_grad = malloc(obj->input_size*sizeof(float));
  for (int i =0; i < obj->inbun->size; i++)
      {
  //  Refp(obj->inbun,i).grad +=  (obj->outir->value/Refp(obj->inbun,i).value)* obj->outir->grad; // this divison result in Zero
  Refp(obj->inbun,i).grad +=  (Refp(obj->inbun,1-i).value)* obj->outir->grad;
    printf("grad %d :  %f\n",i, Refp(obj->inbun,i).grad);
    if (isnan(Refp(obj->inbun,i).grad))
    {
      printf("grad %d :  %f\n",i, Refp(obj->inbun,i).grad);
    }
      }

}


// Neuron MultProto= {
//   .forwardpass = Mult_forwardpass,
//   .backwardpass = Mult_backwardpass
// };
//
void *Membership1_New(size_t size, Neuron proto, float m, float s,char *type,Bundle *inbun, Wire *outir){ // Need fixing

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
    FM1 *fl = calloc( 1 ,sizeof(FM1));
    fl-> proto = *el;
    fl->m = m;
    fl->s = s;
    free(el);
  //  printf("Initalization: %d\n",fl->_(init)(fl) );
    return fl;
  }
  }

void FM1_forwardpass (void *self){

  FM1 *obj = self; //all things are referenced annoynamsuley
  obj->_(outir)->value = 0;
if(obj->_(inbun)->size ==1)
{
  //printf("\t\t\t\t\t\t\tsingleton\n" );
  obj->fs = Refp(obj->_(inbun),0).value;
  printf("for %f",obj->fs );
}
else {
      if(Refp(obj->_(inbun),1).value)
      {
      obj->fs = solve(Refp(obj->_(inbun),0).value, Refp(obj->_(inbun),1).value , obj->m, obj->s);
      }
      else{
    //    printf("\t\t\t\t\t\t\tsingleton!!\n" );
        obj->fs = Refp(obj->_(inbun),0).value;
        printf("%f\n",obj->fs );
      }
}
obj->_(outir)->value =N(obj->fs, obj->m,obj->s);
printf(" --> %f\n", obj->_(outir)->value);



}

//
void FM1_backwardpass (void *self){

  FM1 *obj = self; //all things are referenced annoynamsuley

if(Refp(obj->_(inbun),1).value)
Refp(obj->_(inbun),1).grad= (N(obj->fs + 0.0001, obj->m,obj->s)-N(obj->fs, obj->m,obj->s))/0.0001; // Analytic gradient
printf("Previous membership m %f and variane %f\n",obj->m , obj->s );
obj->m +=  step_size*obj->_(outir)->grad * (obj->fs-obj->m) * obj->_(outir)->value/ sqr(obj->s);
//obj->s +=  step_size*(obj->_(outir)->grad * sqr(obj->fs-obj->m) * obj->_(outir)->value/ tri(obj->s)); // This expression in wrong


obj->s +=  step_size*obj->_(outir)->grad * (N(obj->fs , obj->m,obj->s+ 0.0001)-N(obj->fs , obj->m,obj->s))/0.0001;
printf("Upated membership m %f and variane %f\n",obj->m , obj->s );
if (isnan(obj->m))
{
  printf(" output gradient !%f output value %f \n", obj->_(outir)->grad  , obj->_(outir)->value);
  exit(1);
}

}


// Neuron FM1Proto= {
//   .forwardpass = FM1_forwardpass,
//   .backwardpass = FM1_backwardpass
// };

void *LMS_new(size_t size, Neuron proto, char *type,Bundle *inbun, Wire *outir, float *weight)
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
    LMS *fl = calloc( 1 ,sizeof(LMS));
    fl-> proto = *el;
    fl->Weight = weight;
    free(el);
  //  printf("Initalization: %d\n",fl->_(init)(fl) );
    return fl;
  }
  }
void LMS_forwardpass(void *self)
{
  LMS *obj = self;
  obj->sum = 0;
  obj->_(outir)->value = 0;
printf("___________________________Aggregation\n" );
  for(int i =0; i<obj->_(inbun)->size; i++)
  {
    obj->sum += Refp(obj->_(inbun),i).value;
    obj->_(outir)->value += obj->Weight[i]*  Refp(obj->_(inbun),i).value;
    //printf("%f *  %f = %f \n", obj->Weight[i],  Refp(obj->_(inbun),i).value,obj->Weight[i]*  Refp(obj->_(inbun),i).value);
  }
obj->_(outir)->value = obj->_(outir)->value/obj->sum;
  printf("\t\t\t\t\t                  output  %f\n", obj->_(outir)->value);
}

void LMS_backwardpass(void *self)
{

  LMS *obj = self;
  float N= obj->_(outir)->value*obj->sum;
  for(int i =0; i<obj->_(inbun)->size; i++)
  {
    // Refp(obj->_(inbun),i).grad = obj->Weight[i]*  obj->_(outir)->grad; //backpassing the error Here is the problem
    // obj->Weight[i] += step_size*  Refp(obj->_(inbun),i).value*obj->_(outir)->grad;


    Refp(obj->_(inbun),i).grad = (obj->Weight[i] - obj->_(outir)->value)*  obj->_(outir)->grad/obj->sum; //backpassing the error Here is the problem
    obj->Weight[i] += step_size*  Refp(obj->_(inbun),i).value*obj->_(outir)->grad/obj->sum;

    printf("Updated weight%f\n",obj->Weight[i] );
  }
}
// Neuron LMSProto= {
//   .forwardpass = LMS_forwardpass,
//   .backwardpass = LMS_backwardpass
// };

////
