#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "ex19.h"
float step_size = 0.01;

#define AdRef(T,N) &(*(Wire*)T.addr[N])
#define Ref(T,N) (*(Wire*)T.addr[N])
#define Refp(T,N) (*(Wire*)T->addr[N])
#define Wrap(B,W,I) B.addr[I]=(unsigned long int)&W


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

/////////////////////////////////////////ADD
void Add_forwardpass (void *self){
  Neuron *obj = self;
  obj->outir->value = 0;
   for (int i =0; i < obj->inbun->size; i++)
      {
    printf("input %d :  %f\n",i, Refp(obj->inbun,i).value );
      obj->outir->value += Refp(obj->inbun,i).value;
      }

}
//
void Add_backwardpass (void *self){
Neuron *obj = self;
for (int i =0; i < obj->inbun->size; i++)
    {
  Refp(obj->inbun,i).grad += 1* obj->outir->grad;
  printf("grad %d :  %f\n",i, Refp(obj->inbun,i).grad);
    Refp(obj->inbun,i).grad =0;
    }
}
//
//
Neuron AddProto= {
  .forwardpass = Add_forwardpass,
  .backwardpass = Add_backwardpass
};
//
//
// //
void Mult_forwardpass (void *self){
  Neuron *obj = self;
  obj->outir->value = 1;
   for (int i =0; i < obj->inbun->size; i++)
      {
    printf("Multple gate :input %d :  %f\n",i, Refp(obj->inbun,i).value );
      obj->outir->value *= Refp(obj->inbun,i).value;
      Refp(obj->inbun,i).grad =0;
      }

}

void Mult_backwardpass (void *self){

  Neuron *obj = self;
  float x =1;
  //   //obj->inputs_grad = malloc(obj->input_size*sizeof(float));
  for (int i =0; i < obj->inbun->size; i++)
      {
    Refp(obj->inbun,i).grad +=  (obj->outir->value/Refp(obj->inbun,i).value)* obj->outir->grad;
    printf("grad %d :  %f\n",i, Refp(obj->inbun,i).grad);
      }

}


Neuron MultProto= {
  .forwardpass = Mult_forwardpass,
  .backwardpass = Mult_backwardpass
};
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
if(Refp(obj->_(inbun),1).value)
{
obj->fs = solve(Refp(obj->_(inbun),0).value, Refp(obj->_(inbun),1).value , obj->m, obj->s);
}
else{
  printf("\t\t\t\t\t\t\tsingleton!!\n" );
  obj->fs = Refp(obj->_(inbun),0).value;
  printf("%f\n",obj->fs );
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

obj->m +=  obj->_(outir)->grad * (obj->fs-obj->m) * obj->_(outir)->value/ sqr(obj->s);
obj->s +=  step_size*(obj->_(outir)->grad * sqr(obj->fs-obj->m) * obj->_(outir)->value/ tri(obj->s));


//
// obj->m +=  obj->_(output_grad) * (obj->fs-obj->m) * obj->_(output)/ sqr(obj->s);
//     obj->s +=  obj->_(output_grad) * sqr(obj->fs-obj->m) * obj->_(output) / tri(obj->s);
//
//     // real difficulties with update of obj->fs
//     return obj->_(inputs_grad);
}


Neuron FM1Proto= {
  .forwardpass = FM1_forwardpass,
  .backwardpass = FM1_backwardpass
};

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




int main(int argc, char *argv[])
{
  //TODO all gates Must recieved wires!



//*************************************Initialization **************************
// This is How to create Wires
Wire _0 = newWire(0.5,0);
Wire _1 = newWire(1,0);
Wire _2 = newWire(2,0);
Wire _3 = newWire(3,0);
Wire _4 = newWire(4,0);

// This is how to bundle Wires
Bundle bun = newBundle(5);
Wrap(bun,_0,0);
Wrap(bun,_1,1);
Wrap(bun,_2,2);
Wrap(bun,_3,3);
Wrap(bun,_4,4);

// This is How to reference bundles
Wire *Gimp = AdRef(bun,2);
// Gimp->grad = 200;
// // or
// Ref(bun,2).grad = 100;



// This is how they all point to the same data
printf("grad %f\n", _2.grad);
printf("Gimp grad %f\n", Gimp->grad);
printf("Bundle access grad %f\n",Ref(bun,2).value);


//This is how you create gates

Wire eat = newWire(0,1);
Neuron NN = newNeuron(&bun,&eat);
Add_forwardpass(&NN); // not needed!
Add_backwardpass(&NN);
printf("Change  %zu\n", NN.inbun->size);
printf("I'm hungry for  %f neurons\n", eat.value);

Wire drink = newWire(0,1);
Add *NM = NEW(Add,"Somecrap",&bun,&drink);
NM->_(forwardpass)(NM);
NM->_(backwardpass)(NM);
printf("Change  for pointer%zu\n", NM->_(inbun)->size);
printf("I'm thristy for  %f neurons Pointers\n", drink.value);

printf("Testing Multiple gate \n");
Wire sleep = newWire(0,1);
Mult *FM = NEW(Mult,"first multiplication",&bun,&sleep);
FM->_(forwardpass)(FM);
FM->_(backwardpass)(FM);
printf("I'm dizzy for  %f neurons Pointers\n", sleep.value);

Wire _bicep = newWire(0.7,0);
Wire _bicn = newWire(0,0);
 Bundle bunf = newBundle(2);
 Wrap(bunf,_bicep,0);
 Wrap(bunf,_bicn,1);

 Wire run = newWire(0,1);

FM1 *B = NEWFM1(FM1,0.5,0.2,"big",&bunf,&run);
for(int i=0;i<10;i++)
{
B->_(forwardpass)(B);
B->_(backwardpass)(B);
}





//printf("OUTPUT %f\n" ,j[1].value);

//TODO: for the next part to work you must have stored poniters to the damn inputs
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
