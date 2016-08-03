#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "ex19.h"

#define gr(N) inputs_grad[N]
#define sqr(N) \
      ({  __typeof__ (N) _N = (N); \
          _N*_N; })
#define tri(N) \
      ({  __typeof__ (N) _N = (N); \
          _N*_N*_N; })
#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

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
     //printf("a: %f\n", a);
     //printf("b: %f\n", (m2/s1));
     //printf("c :%f\n", c);
     //printf("\tdelta %f\n", delta);
     float o1 =0;
     float o2 =0;
     float o=0;
     int flag =1;

     if(a == 0)
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
     printf("here!!!!");
     o = e(-0.5 * sqr((o1-m1)/v1)) >  e(-0.5 * sqr((o2-m1)/v1))  ? o1 : o2 ;
    //printf("%f ,  %f \n", e(-0.5 * sqr((o1-m1)/v1)) ,  e(-0.5 * sqr((o2-m1)/v1)) );
    printf("%f ,  %f \n", N(o1,m1,v1) ,  N(o2,m1,v1) );
     printf("intersection :%s | at %f with value of : %f\n ", flag ? "true" : "false", o , N(o, m1,v1) );
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
//
float *stager(float *a, float *b) //DOGSHIT: only combines 2 each are 2 array
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
    obj->inputs_grad[i] = obj->inputs[1-i]*output_grad;
    }
    return obj->inputs_grad;
}


Object MultProto= {
  .forwardpass = Mult_forwardpass,
  .backwardpass = Mult_backwardpass
};

void *Membership1_New(size_t size, Object proto, float m, float s){ // Need fixing

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
    FM1 *fl = calloc( 1 ,sizeof(FM1));
    fl-> proto = *el;
    fl->m = m;
    fl->s = s;
    printf("Initalization: %d\n",fl->_(init)(fl) );
    return fl;
  }
  }

float FM1_forwardpass (void *self, float *inputsww){
  FM1 *obj = self;
//  obj->input_size = sizeof(inputsww)/sizeof(float) ;
//  printf("size I received %zd\n",  sizeof(inputsww)/sizeof(float));
// THIS DOESN"T WORK BECAUSE IT"S ALWAYS THE SIZE OF THE POINT -> 16 bit "addrees" !
  float output = 0;
  obj->_(inputs) = malloc(2*sizeof(float));
  obj->_(inputs_grad) = malloc(2*sizeof(float));

      obj->_(inputs[0])=inputsww[0];
      obj->_(inputs[1]) = inputsww[1];
   if(obj->_(inputs[1])) // None-singleton
   {
   obj->fs = solve(obj->_(inputs[0]), obj->_(inputs[1]) , obj->m, obj->s);
   obj->_(inputs_grad[1]) = N(solve(obj->_(inputs[0]), obj->_(inputs[1]) , obj->m, obj->s) + 0.0001, obj->m,obj->s)/0.0001; // Analytic gradient
    }
  else
  {
    printf("singleton!!\n" );
    obj->fs = obj->_(inputs[0]);
  //  printf("Input is --> %f\n", obj->fs);
  }
      obj->_(output) =N(obj->fs, obj->m,obj->s);
  //    printf("output is: --> %f, mean is -> %f , variance is -> %f | Input %f\n", N(obj->fs, obj->m,obj->s), obj->m, obj->s , obj->fs);
      return obj->_(output) ;
}


float *FM1_backwardpass (void *self, float output_grad){
  FM1 *obj = self;
  if(!obj->_(inputs_grad))  obj->_(inputs_grad) = malloc(2*sizeof(float));
      obj->m +=  output_grad * (obj->fs-obj->m) * obj->_(output)/ sqr(obj->s);
    obj->s +=  output_grad * sqr(obj->fs-obj->m) * obj->_(output) / tri(obj->s);

    // real difficulties with update of obj->fs
    return obj->_(inputs_grad);
}


Object FM1Proto= {
  .forwardpass = FM1_forwardpass,
  .backwardpass = FM1_backwardpass
};



int main(int argc, char *argv[])
{
  //TODO all gates Must recieved wires!
  //*************************************Initialization ***********************************
  float step_size = 0.01;
  float g1[] = {-2, 5};
  float g2[2];
        g2[1]= -4;
  float g3[2] = {.25, .20};

  float b[2]= {1,0};
  float t[2]= {0,0};


// HOW TO CREATE GATES
  //Add *gate1 = NEW(Add, "Layer 1 gate");
  //Mult *gate2 = NEW(Mult, "Layer 2 gate");
//  Wire *wr  = Wire_new(5.7, 6.7);
//  printf("%f, %f", wr->value , wr->grad);
//FM1 *Bi_Z = NEW(FM1, "Layer 2 gate");
//Bi_Z -> m = 5.0;
//Bi_Z-> s = 1.0;

//********************************Layer 1: Fuzzy Memebership

  FM1 *Bi_Z = NEWFM1(FM1,0, 0.2);
  FM1 *Bi_L = NEWFM1(FM1, 0.5, 0.2);
  FM1 *Bi_H = NEWFM1(FM1, 1, 0.2);
  //printf("%f, %f", Bi_Z->m , Bi_Z->s);
  //FM1 *Bi_M =

  FM1 *Tr_Z = NEWFM1(FM1,0, 0.2);
  FM1 *Tr_L = NEWFM1(FM1, 0.5, 0.2);
  FM1 *Tr_H = NEWFM1(FM1, 1, 0.2);

//********************************Layer 2: Rules
Mult *R1 = NEW(Mult, "Z & Z"); //Z
float r1[2]; // TODO: add this as a property of eac
Mult *R2 = NEW(Mult, "Z & L"); //NL
float r2[2];
Mult *R3 = NEW(Mult, "Z & H"); //NH
float r3[2];

Mult *R4 = NEW(Mult, "L & Z"); //PL
float r4[2];
Mult *R5 = NEW(Mult, "L & L"); //Z
float r5[2];
Mult *R6 = NEW(Mult, "L & H"); //NL
float r6[2];

Mult *R7 = NEW(Mult, "H & Z"); //PH
float r7[2];
Mult *R8 = NEW(Mult, "H & L"); //PL
float r8[2];
Mult *R9 = NEW(Mult, "H & H"); //Z
float r9[2];
// TODO: MUST BE ABLE TO BETTER CONNECT THESE WITH THE RULES OUTPUT.. THE RULES
// MUST NOT BE PRE-DETERMINED, LAYER 3 IS DOGSHIT
//**************************************Layer 3: Pre-gates
Add *NH = NEW(Add, "R3");
float nh[2]; nh[1]=0;
Add *NL = NEW(Add, "R2 & R6");
float nl[2];
Add *Z  = NEW(Add, "R1 & R5 & R9");
float z[3];
Add *PL = NEW(Add, "R4 & R8");
float pl[2];
Add *PH = NEW(Add, "R7");
float ph[2]; ph[1]=0;
//***********************************Layer 4: gates output

//***********************************defuzzification


    /* code */


  //*********************************************forwardpass


  r1[0] =Bi_Z->_(forwardpass)(Bi_Z, b); r1[1]=Tr_Z->_(forwardpass)(Tr_Z, t);
  r2[0] =Bi_Z->_(forwardpass)(Bi_Z, b); r2[1]=Tr_L->_(forwardpass)(Tr_L, t);
  r3[0] =Bi_Z->_(forwardpass)(Bi_Z, b); r3[1]=Tr_H->_(forwardpass)(Tr_H, t);

  r4[0] =Bi_L->_(forwardpass)(Bi_L, b); r4[1]=Tr_Z->_(forwardpass)(Tr_Z, t);
  r5[0] =Bi_L->_(forwardpass)(Bi_L, b); r5[1]=Tr_L->_(forwardpass)(Tr_L, t);
  r6[0] =Bi_L->_(forwardpass)(Bi_L, b); r6[1]=Tr_H->_(forwardpass)(Tr_H, t);

  r7[0] =Bi_H->_(forwardpass)(Bi_H, b); r7[1]=Tr_Z->_(forwardpass)(Tr_Z, t);
  r8[0] =Bi_H->_(forwardpass)(Bi_H, b); r8[1]=Tr_L->_(forwardpass)(Tr_L, t);
  r9[0] =Bi_H->_(forwardpass)(Bi_H, b); r9[1]=Tr_H->_(forwardpass)(Tr_H, t);

  z[0]  = R1->_(forwardpass)(R1,r1);
  nl[0] = R2->_(forwardpass)(R2,r2);
  nh[0] = R3->_(forwardpass)(R3,r3);
  pl[0] = R4->_(forwardpass)(R4,r4);
  z[1]  = R5->_(forwardpass)(R5,r5);
  nl[1] = R6->_(forwardpass)(R6,r6);
  ph[0] = R7->_(forwardpass)(R7,r7);
  pl[1] = R8->_(forwardpass)(R8,r8);
  z[2] = R9->_(forwardpass)(R9,r9);


printf("NH \t\t%f\n",NH->_(forwardpass)(NH, nh));
printf("NL \t\t%f\n",NL->_(forwardpass)(NL, nl));
printf("Z  \t\t%f\n",Z->_(forwardpass)( Z, z));
printf("PL \t\t%f\n",PL->_(forwardpass)(PL, pl));
printf("PH \t\t%f\n",PH->_(forwardpass)(PH, ph));

Wire wr[3];
Wire ww;
wr[0].value = 5;
wr[1].grad=8;

printf(" %f , %f, %zu \n",wr[0].value, wr[1].grad, sizeof(wr)/sizeof(Wire) ); // yes! Input can always be wire TODO



//***************************************Learning
/*
for (size_t i = 0; i < 50; i++) {
printf("Fuzzy mmber output \t\t%f\n",Bi_Z->_(forwardpass)(Bi_Z, g3));
Bi_Z->_(backwardpass)(Bi_Z,1);
}
*/


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

      }/*




      /////////gate1
      //  g2[0]= gate1->_(forwardpass)(gate1, g1);
      //  printf("output %f\n",g2[0]);



      //////////////////////////////gate2

      //printf("output %f\n",gate2->_(forwardpass)(gate2, g2));


      //////////////////////////////////////////////gate3






      ///******************************************************** backprogate
      //gate2->_(backwardpass)(gate2,1);
      //gate1->_(backwardpass)(gate1, gate2->_(inputs_grad[0]));


      /*
      ///***************************************************************************************update
      g2[1]+= gate2->_(inputs_grad[1])* step_size;
      g1[0]+= gate1->_(inputs_grad[0]) * step_size;
      g1[1]+= gate1->_(inputs_grad[0]) * step_size;

      printf("\tupdate x[0] %f\n",g1[0]);
      printf("\tupdate x[1]%f\n",g1[1]);
      printf("\tupdate y[1] %f\n",g2[1]);

      //printf("output %f\n",g2[1]*(g1[0]+g1[1]));
      */
