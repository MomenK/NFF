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
//
float *stager(float *a, float *b) //DOGSHIT: only combines 2 each are 2 array
{
  float* total = malloc(4*( sizeof(float))); // array to hold the result

  memcpy(total,     a,  2*sizeof(float)); // copy 4 floats from x to total[0]...total[3]
  memcpy(total + 2, b,  2*sizeof(float)); // copy 4 floats from y to total[4]...total[7]
printf("%zd\n",sizeof(total)/sizeof(float));
  return total;
}

float Add_forwardpass (void *self, Wire *inputsww){
  Object *obj = self;
  float *r;
   //r[0]=  5.8;
//  obj->input_size = sizeof(inputsww)/sizeof(Wire) ;
//printf("size I received %d\n",  r[i]);
// THIS DOESN"T WORK BECAUSE IT"S ALWAYS THE SIZE OF THE POINT -> 16 bit "addrees" !
  float output = 0;
   obj->inputs = malloc(obj->input_size*sizeof(float));
    for (int i =0; i < obj->input_size; i++)
      {
      obj->inputs[i]=inputsww[i].value;
      printf("input %d :  %f\n",i, inputsww[i].value );
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


float Mult_forwardpass (void *self, Wire *inputsww){
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
      obj->inputs[i]=inputsww[i].value;
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
  //  printf("Initalization: %d\n",fl->_(init)(fl) );
    return fl;
  }
  }

float FM1_forwardpass (void *self, Wire *inputsww){
  FM1 *obj = self;
//  obj->input_size = sizeof(inputsww)/sizeof(float) ;
//  printf("size I received %zd\n",  sizeof(inputsww)/sizeof(float));
// THIS DOESN"T WORK BECAUSE IT"S ALWAYS THE SIZE OF THE POINT -> 16 bit "addrees" !
  float output = 0;
  obj->_(inputs) = malloc(2*sizeof(float));
  obj->_(inputs_grad) = malloc(2*sizeof(float));
      obj->_(inputs[0])=inputsww[0].value;
      obj->_(inputs[1]) = inputsww[1].value;
   if(obj->_(inputs[1])) // None-singleton
   {
   obj->fs = solve(obj->_(inputs[0]), obj->_(inputs[1]) , obj->m, obj->s);
   obj->_(inputs_grad[1]) = N(obj->fs + 0.0001, obj->m,obj->s)/0.0001; // Analytic gradient
    }
  else
  {
    printf("\t\t\t\t\t\t\tsingleton!!\n" );
    obj->fs = obj->_(inputs[0]);
  //  printf("Input is --> %f\n", obj->fs);
  }
      obj->_(output) =N(obj->fs, obj->m,obj->s);
  printf("output is: --> %f\n", obj->_(output));
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



void *Wirejoin( Wire *a, Wire *b, size_t n)
{
Wire *obj = malloc(2*n *sizeof(Wire));
int i;
if (n==1)
{
  obj[0].value =  a->value;
  obj[0].grad = a->grad;
  obj[1].value = b->value;
  obj[1].grad = b->grad;
}
else{
for( i=0; i<n; i++){
obj[i].value =  a[i].value;
obj[i].grad = a[i].grad;
}

for(int j=0; j< n; j++){
obj[i+j].value = b[i].value;
obj[i+j].grad = b[i].grad;
}
}
return obj;

}



int main(int argc, char *argv[])
{
  //TODO all gates Must recieved wires!
  //*************************************Initialization ***********************************
  float step_size = 0.01;


  Wire b[2];  b[0].value = 1;  b[1].value = 0;
  Wire t[2]; t[0].value =0; t[1].value = 0;


// HOW TO CREATE GATES
  //Add *gate1 = NEW(Add, "Layer 1 gate");
  //Mult *gate2 = NEW(Mult, "Layer 2 gate");
//  Wire *wr  = Wire_new(5.7, 6.7);
//  printf("%f, %f", wr->value , wr->grad);
//FM1 *Bi_Z = NEW(FM1, "Layer 2 gate");
//Bi_Z -> m = 5.0;
//Bi_Z-> s = 1.0;

//********************************Layer 1: Fuzzy Memebership

  FM1 *Bi_Z = NEWFM1(FM1,  0, 0.2); //TODO: Relate with input size;
  FM1 *Bi_L = NEWFM1(FM1,0.5, 0.2);
  FM1 *Bi_H = NEWFM1(FM1,  1, 0.2);
  //printf("%f, %f", Bi_Z->m , Bi_Z->s);
  //FM1 *Bi_M =

  FM1 *Tr_Z = NEWFM1(FM1,  0, 0.2);
  FM1 *Tr_L = NEWFM1(FM1,0.5, 0.2);
  FM1 *Tr_H = NEWFM1(FM1,  1, 0.2);

  Wire *bz = Wire_new(0,0);
  Wire *bl = Wire_new(0,0);
  Wire *bh = Wire_new(0,0);

  Wire *tz = Wire_new(0,0);
  Wire *tl = Wire_new(0,0);
  Wire *th = Wire_new(0,0);

//********************************Layer 2: Rules
Mult *R1 = NEW(Mult, "Z & Z"); //Z
Wire * r1; // TODO: add this as a property of eac
Mult *R2 = NEW(Mult, "Z & L"); //NL
Wire * r2;
Mult *R3 = NEW(Mult, "Z & H"); //NH
Wire * r3;

Mult *R4 = NEW(Mult, "L & Z"); //PL
Wire * r4;
Mult *R5 = NEW(Mult, "L & L"); //Z
Wire * r5;
Mult *R6 = NEW(Mult, "L & H"); //NL
Wire * r6;

Mult *R7 = NEW(Mult, "H & Z"); //PH
Wire *r7;
Mult *R8 = NEW(Mult, "H & L"); //PL
Wire *r8;
Mult *R9 = NEW(Mult, "H & H"); //Z
Wire *r9;
// TODO: MUST BE ABLE TO BETTER CONNECT THESE WITH THE RULES OUTPUT.. THE RULES
// MUST NOT BE PRE-DETERMINED, LAYER 3 IS DOGSHIT
/*
//**************************************Layer 3: Pre-gates
Add *NH = NEW(Add, "R3");
Wire nh[2]; NH->_(input_size) =1;
Add *NL = NEW(Add, "R2 & R6");
Wire nl[2]; NL->_(input_size);
Add *Z  = NEW(Add, "R1 & R5 & R9");
Wire z[3]; Z->_(input_size) = 3;
Add *PL = NEW(Add, "R4 & R8");
Wire pl[2]; PL->_(input_size) = 2;
Add *PH = NEW(Add, "R7");
Wire ph[2]; PH->_(input_size) =1;
*/
//***********************************Layer 3: gates output
Wire *o1 = Wire_new(0,0);
Wire *o2 = Wire_new(0,0);
Wire *o3 = Wire_new(0,0);
Wire *o4 = Wire_new(0,0);
Wire *o5 = Wire_new(0,0);
Wire *o6 = Wire_new(0,0);
Wire *o7 = Wire_new(0,0);
Wire *o8 = Wire_new(0,0);
Wire *o9 = Wire_new(0,0);

float O1 = 0.5;
float O2 = 0.25;
float O3 = 0;
float O4 = 0.75;
float O5 = 0.5;
float O6 = 0.25;
float O7 = 1;
float O8 = 0.75;
float O9 = 0.5;


Wire * Def = Wire_new(0,0);
float N;

//Wire *rr;

    /* code */
  //*********************************************forwardpass
  printf("\t\t Gate B\n" );
bz->value =Bi_Z->_(forwardpass)(Bi_Z, b);
bl->value  =Bi_L->_(forwardpass)(Bi_L, b);
bh->value  =Bi_H->_(forwardpass)(Bi_H, b);
  printf("\t\t Gate T\n" );
tz->value=Tr_Z->_(forwardpass)(Tr_Z, t);
tl->value=Tr_L->_(forwardpass)(Tr_L, t);
th->value=Tr_H->_(forwardpass)(Tr_H, t);



 r1 = Wirejoin(bz,tz,1);
 r2 = Wirejoin(bz,tl,1);
 r3 = Wirejoin(bz,th,1);
 r4 = Wirejoin(bl,tz,1);
 r5 = Wirejoin(bl,tl,1);
 r6 = Wirejoin(bl,th,1);
 r7 = Wirejoin(bh,tz,1);
 r8 = Wirejoin(bh,tl,1);
 r9 = Wirejoin(bh,th,1);
//printf("Value %f\n" ,r1[9].value);


o1->value  = R1->_(forwardpass)(R1,r1);
o2->value  = R2->_(forwardpass)(R2,r2);
o3->value  = R3->_(forwardpass)(R3,r3);
o4->value  = R4->_(forwardpass)(R4,r4);
o5->value  = R5->_(forwardpass)(R5,r5);
o6->value  = R6->_(forwardpass)(R6,r6);
o7->value  = R7->_(forwardpass)(R7,r7);
o8->value  = R8->_(forwardpass)(R8,r8);
o9->value  = R9->_(forwardpass)(R9,r9);

//***********************************defuzzification


Def->value =  o1->value * O1 +
              o2->value * O2 +
              o3->value * O3 +
              o4->value * O4 +
              o5->value * O5 +
              o6->value * O6 +
              o7->value * O7 +
              o8->value * O8 +
              o9->value * O9;

N =           o1->value +
              o2->value +
              o3->value +
              o4->value +
              o5->value +
              o6->value +
              o7->value +
              o8->value +
              o9->value ;

printf("OUTPUT %f\n" ,Def->value);


Def->grad = sqr(Def->value- 1.0);

printf("LSE %f\n" ,Def->grad);

//***********************************backwardpropagating

/*

  nh[0].value  = R3->_(forwardpass)(R3,r3);
  pl[0].value  = R4->_(forwardpass)(R4,r4);
  z[1].value   = R5->_(forwardpass)(R5,r5);
  nl[1].value  = R6->_(forwardpass)(R6,r6);
  ph[0].value  = R7->_(forwardpass)(R7,r7);
  pl[1].value  = R8->_(forwardpass)(R8,r8);
  z[2].value = R9->_(forwardpass)(R9,r9);


printf("NH \t\t%f\n",NH->_(forwardpass)(NH, nh));
printf("NL \t\t%f\n",NL->_(forwardpass)(NL, nl));
printf("Z  \t\t%f\n",Z->_(forwardpass)( Z, z));
printf("PL \t\t%f\n",PL->_(forwardpass)(PL, pl));
printf("PH \t\t%f\n",PH->_(forwardpass)(PH, ph));


Wire wr[3];
Wire ww;
wr[0].value = 5;
wr[1].value=8;
wr[2].value=7;
//wr = {.value = 3, .grad = 5}; TODO: Figure This out!
Z->_(input_size) = 3;
printf("Z  \t\t%f\n",Z->_(forwardpass)( Z, wr));
printf(" %f , %f, %zu \n",wr[0].value, wr[1].grad, sizeof(wr));


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
float b[] = {5.0, 0.0};obj[0].value =  a.value;
  obj[0].grad = a.grad;
  obj[1].value = b.value;
  obj[1].grad = b.grad;
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
