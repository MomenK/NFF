
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "thing.h"

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

void die(const char *message)
{
  if(errno)
  perror(message);
  else
  printf("ERROR: %s\n", message);

  exit(1);
}

/////////////////////////////////////////ADD
void Add_forwardpass (void *self){
  Neuron *obj = self;
  obj->outir->value = 0;
//  obj->c = "haha";
printf("\nGate :%s \n",obj->c );

   for (int i =0; i < obj->inbun->size; i++)
      {
    printf("\t\tinput %d :  %f\n",i, Refp(obj->inbun,i).value );
      obj->outir->value += Refp(obj->inbun,i).value;
        Refp(obj->inbun,i).grad =0;
      }
printf("\t\t\toutput is: --> %f\n", obj->outir->value);
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
// Neuron AddProto= {
//   .forwardpass = Add_forwardpass,
//   .backwardpass = Add_backwardpass
// };
//
//
// //
void Mult_forwardpass (void *self){
  Neuron *obj = self;
  obj->outir->value = 1;
  printf("\nGate :%s \n",obj->c );
   for (int i =0; i < obj->inbun->size; i++)
      {
    printf("\t\tinput %d :  %f\n",i, Refp(obj->inbun,i).value );
      obj->outir->value *= Refp(obj->inbun,i).value;
      Refp(obj->inbun,i).grad =0;
      }
printf("\t\t\toutput is: --> %f\n", obj->outir->value);
}

void Mult_backwardpass (void *self){

  Neuron *obj = self;
  float x =1;
  //   //obj->inputs_grad = malloc(obj->input_size*sizeof(float));
  for (int i =0; i < obj->inbun->size; i++)
      {
    Refp(obj->inbun,i).grad +=  (obj->outir->value/Refp(obj->inbun,i).value)* obj->outir->grad;
  //  printf("grad %d :  %f\n",i, Refp(obj->inbun,i).grad);
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
  printf("\t\t\t\t\t\t\tsingleton!!\n" );
  obj->fs = Refp(obj->_(inbun),0).value;
  printf("%f\n",obj->fs );
}
else {
      if(Refp(obj->_(inbun),1).value)
      {
      obj->fs = solve(Refp(obj->_(inbun),0).value, Refp(obj->_(inbun),1).value , obj->m, obj->s);
      }
      else{
        printf("\t\t\t\t\t\t\tsingleton!!\n" );
        obj->fs = Refp(obj->_(inbun),0).value;
        printf("%f\n",obj->fs );
      }
}
obj->_(outir)->value =N(obj->fs, obj->m,obj->s);
printf("output is: --> %f\n", obj->_(outir)->value);



//
//   FM1 *obj = self;
// //  obj->input_size = sizeof(inputsww)/sizeof(float) ;
// //  printf("size I received %zd\n",  sizeof(inputsww)/sizeof(float));
// // THIS DOESN"T WORK BECAUSE IT"S ALWAYS THE SIZE OF THE POINT -> 16 bit "addrees" !
//   float output = 0;
//   obj->_(inputs) = malloc(2*sizeof(float));
//   obj->_(inputs_grad) = malloc(2*sizeof(float));
//       obj->_(inputs[0].value)=inputsww[0].value;
//       obj->_(inputs[1].value) = inputsww[1].value;
//    if(obj->_(inputs[1].value)) // None-singleton
//    {
//    obj->fs = solve(obj->_(inputs[0].value), obj->_(inputs[1].value) , obj->m, obj->s);
//    obj->_(inputs_grad[1]) = N(obj->fs + 0.0001, obj->m,obj->s)/0.0001; // Analytic gradient why HERE??
//     }
//   else
//   {
//     printf("\t\t\t\t\t\t\tsingleton!!\n" );
//     obj->fs = obj->_(inputs[0].value);
//   //  printf("Input is --> %f\n", obj->fs);
//   }
//       obj->_(output) =N(obj->fs, obj->m,obj->s);
//   printf("output is: --> %f\n", obj->_(output));
//       return obj->_(output) ;
}

//
void FM1_backwardpass (void *self){

  FM1 *obj = self; //all things are referenced annoynamsuley

if(Refp(obj->_(inbun),1).value)
Refp(obj->_(inbun),1).grad= N(obj->fs + 0.0001, obj->m,obj->s)/0.0001; // Analytic gradient

obj->m +=  step_size*obj->_(outir)->grad * (obj->fs-obj->m) * obj->_(outir)->value/ sqr(obj->s);
obj->s +=  step_size*(obj->_(outir)->grad * sqr(obj->fs-obj->m) * obj->_(outir)->value/ tri(obj->s));


//
// obj->m +=  obj->_(output_grad) * (obj->fs-obj->m) * obj->_(output)/ sqr(obj->s);
//     obj->s +=  obj->_(output_grad) * sqr(obj->fs-obj->m) * obj->_(output) / tri(obj->s);
//
//     // real difficulties with update of obj->fs
//     return obj->_(inputs_grad);
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
  obj->_(outir)->value = 0;
printf("///////////////////////////////Aggregation/////////////////////\n" );
  for(int i =0; i<obj->_(inbun)->size; i++)
  {
    obj->_(outir)->value += obj->Weight[i]*  Refp(obj->_(inbun),i).value;
    printf("%f *  %f = %f \n", obj->Weight[i],  Refp(obj->_(inbun),i).value,obj->Weight[i]*  Refp(obj->_(inbun),i).value);
  }
  printf("Torque output is: --> %f\n", obj->_(outir)->value);
}

void LMS_backwardpass(void *self)
{

  LMS *obj = self;
  for(int i =0; i<obj->_(inbun)->size; i++)
  {
    Refp(obj->_(inbun),i).grad = obj->Weight[i]*  obj->_(outir)->grad; //backpassing the error
    obj->Weight[i] += step_size*  Refp(obj->_(inbun),i).value*obj->_(outir)->grad;

      //obj->Weight[i] += step_size*  (s *obj->Weight[i]*obj->_(outir)->grad*Refp(obj->_(inbun),i).value;
    printf("Updated weight%f\n",obj->Weight[i] );
  }
}
// Neuron LMSProto= {
//   .forwardpass = LMS_forwardpass,
//   .backwardpass = LMS_backwardpass
// };


// */

void *Wirejoin( Wire ** a, Wire ** b, size_t n,size_t m) // recieves pp from p addrss
{
  size_t step = sizeof(Wire);
Wire *obj = calloc(m+n ,step);
//Wire ** obj;
int i;
for (i = 0; i<n ; i++)
{
obj[i]= **(a+i);
printf("%f\n", obj[i].value );
* (a) = &obj[i]; // changes the value pointed to by p ( since pp point to that value)
}


//for(int j=0; j<m ;j++)
//{
obj[n]= **(b);


* (b) = &obj[n];
//}j1[0].grad

return obj;
//TODO: currently NOT working for more than one!
}


void Cha( Wire *self)
{
  Wire * m =self;
  m->value=9;
}
