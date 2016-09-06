#define epoch  500
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "object.h"
#include "thing.h"

float step_size = 0.1;
float SSE= 0;
int main(int argc, char *argv[])
{
  //TODO all gates Must recieved wires!


//////////////////////////////// Parsing
if(argc < 2 || argc > 4) die("USAGE: ./ex17 <dbfile> <colm> <colm>"); //./CsvParser du.csv 1 2
char *filename = argv[1];

//////////////////////////////////////////// Reading and parsing .csv File
int biceps_col = atoi(argv[2]);
struct Input *inp = File_Open(filename); // Open the file and loads the data base
inp = File_Parse(inp ,biceps_col); // Parses the database into coloums
InputsPrint(inp);

printf("Moahahahaha\n" );
//if(argc == 4)
//  {
  int force_col = atoi(argv[3]);
  struct Input *inp1 = File_Open(filename);
  inp1 = File_Parse(inp1 ,force_col);
  InputsPrint(inp1);
//    }

////////////////////////////////////////////// Converting to float
float biceps[MAX_ELEMENT];
float force[MAX_ELEMENT];

for(int i =0; i< MAX_ELEMENT ; i++)
  {
  biceps[i]=atof(inp->I[i].lines);
//  printf("bbbbbbbbbbbbb%f\n",biceps[i] );
  force[i]=atof(inp1->I[i].lines);
//  printf("fffffffffff%f\n",force[i] );
  }
  Closefile(inp);
  Closefile(inp1);


// //////////////////////////////////////////// Shuffling
// srand(time(NULL));
// shufflem(biceps,force, MAX_ELEMENT);
//
// for(int i =0; i< MAX_ELEMENT ; i++)
//   {
//   printf("B %d :%f\n",i,biceps[i] );
//   printf("O %d :%f\n",i,force[i] );
//   }
//
//
// ///////////////////////////////////////// Dividing the training and the testing
// float b_train[MAX_ELEMENT-TEST_SET];
// float f_train[MAX_ELEMENT-TEST_SET];
// float b_test[TEST_SET];
// float f_test[TEST_SET];
// printf("\nTraining Sets\n" );
// for(int i =0; i< MAX_ELEMENT-TEST_SET ; i++)
//   {
//   b_train[i]=biceps[i];
//   printf("%f\n",b_train[i] );
//   f_train[i]=force[i];
//   printf("%f\n",f_train[i] );
//   }
// printf("Testing Sets\n" );
// for(int i =0; i< TEST_SET ; i++)
//   {
//   b_test[i]=biceps[i+MAX_ELEMENT-TEST_SET];
//   printf("%f\n",b_test[i] );
//   f_test[i]=force[i+MAX_ELEMENT-TEST_SET];
//   printf("%f\n",f_test[i] );
//   }



/////////////////////////////////////////////////////////////////////////////////
Wire Bicep = newWire(0.7,0);
Wire Tricep = newWire(0,0);

/////////////////////////////////////////////////////////////////////////////////
Bundle Bicep_bun = newBundle(1);
Wrap(Bicep_bun,Bicep,0);

Bundle Tricep_bun=newBundle(1);
Wrap(Tricep_bun,Tricep,0);
///
Wire Bicep_Z, Bicep_M ,Bicep_H;

Wire Tricep_Z,Tricep_M,Tricep_H;
///
FM1 *_Bicep_Z = NEWFM1(FM1,0,0.2,"layer 1: Gate 1- FM1", &Bicep_bun,&Bicep_Z);
FM1 *_Bicep_M = NEWFM1(FM1,0.5,0.2,"layer 1: Gate 2- FM1", &Bicep_bun,&Bicep_M);
FM1 *_Bicep_H = NEWFM1(FM1,1,0.2,"layer 1: Gate 3- FM1", &Bicep_bun,&Bicep_H);

FM1 *_Tricep_Z = NEWFM1(FM1,0,0.2,"layer 1: Gate 4- FM1", &Tricep_bun,&Tricep_Z);
FM1 *_Tricep_M = NEWFM1(FM1,0.5,0.2,"layer 1: Gate 5- FM1", &Tricep_bun,&Tricep_M);
FM1 *_Tricep_H = NEWFM1(FM1,1,0.2,"layer 1: Gate 6- FM1", &Tricep_bun,&Tricep_H);
/////////////////////////////////////////////////////////////////////////////////
Bundle ZZ = newBundle(2);
Wrap(ZZ,Bicep_Z,0);
Wrap(ZZ,Tricep_Z,1);
Bundle ZM = newBundle(2);
Wrap(ZM,Bicep_Z,0);
Wrap(ZM,Tricep_M,1);
Bundle ZH = newBundle(2);
Wrap(ZH,Bicep_Z,0);
Wrap(ZH,Tricep_H,1);

Bundle MZ = newBundle(2);
Wrap(MZ,Bicep_M,0);
Wrap(MZ,Tricep_Z,1);
Bundle MM = newBundle(2);
Wrap(MM,Bicep_M,0);
Wrap(MM,Tricep_M,1);
Bundle MH = newBundle(2);
Wrap(MH,Bicep_M,0);
Wrap(MH,Tricep_H,1);

Bundle HZ = newBundle(2);
Wrap(HZ,Bicep_H,0);
Wrap(HZ,Tricep_Z,1);
Bundle HM = newBundle(2);
Wrap(HM,Bicep_H,0);
Wrap(HM,Tricep_M,1);
Bundle HH = newBundle(2);
Wrap(HH,Bicep_H,0);
Wrap(HH,Tricep_H,1);

///
Wire RZZ, RZM, RZH,  RMZ, RMM, RMH,  RHZ, RHM, RHH;
///
Mult *_RZZ = NEW(Mult,"Layer 2: Gate 1- Mult",&ZZ,&RZZ); //ZE
Mult *_RZM = NEW(Mult,"Layer 2: Gate 2- Mult",&ZM,&RZM);
Mult *_RZH = NEW(Mult,"Layer 2: Gate 3- Mult",&ZH,&RZH);

Mult *_RMZ = NEW(Mult,"Layer 2: Gate 4- Mult",&MZ,&RMZ);
Mult *_RMM = NEW(Mult,"Layer 2: Gate 5- Mult",&MM,&RMM);
Mult *_RMH = NEW(Mult,"Layer 2: Gate 6- Mult",&MH,&RMH);

Mult *_RHZ = NEW(Mult,"Layer 2: Gate 7- Mult",&HZ,&RHZ);
Mult *_RHM = NEW(Mult,"Layer 2: Gate 8- Mult",&HM,&RHM);
Mult *_RHH = NEW(Mult,"Layer 2: Gate 9- Mult",&HH,&RHH);
////////////////////////////////////////////////////////////////////////////////
Bundle NH = newBundle(1);//
Wrap(NH,RZH,0);

Bundle NL = newBundle(2);
Wrap(NL,RZM,0);
Wrap(NL,RMH,1);

Bundle ZE = newBundle(3);
Wrap(ZE,RZZ,0);
Wrap(ZE,RMM,1);
Wrap(ZE,RHH,2);

Bundle PL = newBundle(2);
Wrap(PL,RMZ,0);
Wrap(PL,RHM,1);

Bundle PH = newBundle(1);//
Wrap(PH,RHZ,0);

Wire ONH,ONL,OZE,OPL,OPH;

Add *_ONH = NEW(Add,"Layer 3: Gate 1- Add",&NH,&ONH);
Add *_ONL = NEW(Add,"Layer 3: Gate 2- Add",&NL,&ONL);
Add *_OZE = NEW(Add,"Layer 3: Gate 3- Add",&ZE,&OZE);
Add *_OPL = NEW(Add,"Layer 3: Gate 4- Add",&PL,&OPL);
Add *_OPH = NEW(Add,"Layer 3: Gate 5- Add",&PH,&OPH);
///////////////////////////////////////////////////////////////////////////////
Bundle gard = newBundle(5);
Wrap(gard,ONH,0);
Wrap(gard,ONL,1);
Wrap(gard,OZE,2);
Wrap(gard,OPL,3);
Wrap(gard,OPH,4);

Wire torque;
float a[] = {0,0.25, 0.5,0.75,1};


LMS *_gard = NEWLMS(LMS,"Layer 4: Aggregation",&gard,&torque,a);

float perf[epoch];
int i;
int j;
for(i =0; i<epoch; i++)
{
  perf[i] = 0;
printf("\n\n__________________________Generation____________________________ :%d\n",i );
   for(j = 0; j<MAX_ELEMENT;j++ )
   {
  printf("\t\t\t\t\t Training Pair %d : (%f , %f )\n",j,biceps[j],force[j] );
  Bicep.value = biceps[j]; // There reaches nan after the first iteration for big numbers
fb(_Bicep_Z);
fb(_Bicep_M);
fb(_Bicep_H);// Here is your problem
printf("\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t Bicep_H Gradient !!!!!!!!!!!%f\n", Bicep_H.grad);
fb(_Tricep_Z);
fb(_Tricep_M);
fb(_Tricep_H);

fb(_RZZ);
fb(_RZM);
fb(_RZH);

fb(_RMZ);
fb(_RMM);
fb(_RMH);

fb(_RHZ);
fb(_RHM);
fb(_RHH);
printf("\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t Bicep_H Gradient 00000%f\n", Bicep_H.grad);

fb(_ONH);
fb(_ONL);
fb(_OZE);
fb(_OPL);
fb(_OPH);

fb(_gard);

//torque.value -= 0.5;
torque.grad = -(torque.value - force[j]);
perf[i]+= sqr(torque.grad);
//torque.grad = -1;
//printf("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO\n");
  printf("\t\t\t\t\t                  error (%f - %f )  %f\n", force[j],torque.value , torque.grad);
bb(_gard);


bb(_ONH);
bb(_ONL);
bb(_OZE);
bb(_OPL);
bb(_OPH);


bb(_RZZ);
bb(_RZM);
bb(_RZH);

bb(_RMZ);
bb(_RMM);
bb(_RMH);

bb(_RHZ);// This dude can generate nan!!!
bb(_RHM);
bb(_RHH);


bb(_Bicep_Z);
bb(_Bicep_M);
bb(_Bicep_H);
bb(_Tricep_Z);
bb(_Tricep_M);
bb(_Tricep_H);
}
}


                                                  //// Testing
FILE *stream = fopen("Result.csv","w");
                                                  printf("\n\n__________________________Generation____________________________ :%d\n",i );
                                                     for(j = 0; j<MAX_ELEMENT;j++ )
                                                     {
                                                    printf("\t\t\t\t\t Training Pair %d : (%f , %f )\n",j,biceps[j],force[j] );
                                                    Bicep.value = biceps[j]; // There reaches nan after the first iteration for big numbers
                                                  fb(_Bicep_Z);
                                                  fb(_Bicep_M);
                                                  fb(_Bicep_H);// Here is your problem
                                                  printf("\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t Bicep_H Gradient !!!!!!!!!!!%f\n", Bicep_H.grad);
                                                  fb(_Tricep_Z);
                                                  fb(_Tricep_M);
                                                  fb(_Tricep_H);

                                                  fb(_RZZ);
                                                  fb(_RZM);
                                                  fb(_RZH);

                                                  fb(_RMZ);
                                                  fb(_RMM);
                                                  fb(_RMH);

                                                  fb(_RHZ);
                                                  fb(_RHM);
                                                  fb(_RHH);
                                                  printf("\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t Bicep_H Gradient 00000%f\n", Bicep_H.grad);

                                                  fb(_ONH);
                                                  fb(_ONL);
                                                  fb(_OZE);
                                                  fb(_OPL);
                                                  fb(_OPH);

                                                  fb(_gard);
                                              //    torque.value -= 0.5;
                                                  torque.grad = -(torque.value - force[j]);
                                                  fprintf(stream, "%f,%f,\n", torque.value ,force[j]);
                                                  SSE+= sqr(torque.grad);
                                                  //torque.grad = -1;
                                                  //printf("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO\n");

                                                  }
                                                  fclose(stream);
//Bundleupdate(&Bicep_bun); should be inside that back

printf("******************************************\n");
for(int k = 0; k< epoch; k++)
{
printf("*  %d    : %f \n",k,perf[k] );
}
printf("*  SSE    : %f(MAX_ELEMENT %d) MSE  :%f \n",SSE,MAX_ELEMENT,SSE/MAX_ELEMENT);
}
