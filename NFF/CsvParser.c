#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <errno.h>
#include <time.h>
//
// #define MAX_DATA 200
// #define MAX_ELEMENT 50
// #define TEST_SET 10
// #define epoch

#include "thing.h"




// Parser functions ****************************************************

//****************************************************

int main(int argc, char* argv[])
{
  if(argc < 2 || argc > 4) die("USAGE: ex17 <dbfile> <colm> <colm>"); //./CsvParser du.csv 1 2
  char *filename = argv[1];

  //////////////////////////////////////////// Reading and parsing .csv File
  int biceps_col = atoi(argv[2]);
  struct Input *inp = File_Open(filename); // Open the file and loads the data base
  inp = File_Parse(inp ,biceps_col); // Parses the database into coloums
  InputsPrint(inp);


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


 //////////////////////////////////////////// Shuffling
  srand(time(NULL));
  shufflem(biceps,force, MAX_ELEMENT);

  for(int i =0; i< MAX_ELEMENT ; i++)
    {
    printf("B %d :%f\n",i,biceps[i] );
    printf("O %d :%f\n",i,force[i] );
    }


  ///////////////////////////////////////// Dividing the training and the testing
  float b_train[MAX_ELEMENT-TEST_SET];
  float f_train[MAX_ELEMENT-TEST_SET];
  float b_test[TEST_SET];
  float f_test[TEST_SET];
  printf("\nTraining Sets\n" );
  for(int i =0; i< MAX_ELEMENT-TEST_SET ; i++)
    {
    b_train[i]=biceps[i];
    printf("%f\n",b_train[i] );
    f_train[i]=force[i];
    printf("%f\n",f_train[i] );
    }
  printf("Testing Sets\n" );
  for(int i =0; i< TEST_SET ; i++)
    {
    b_test[i]=biceps[i+MAX_ELEMENT-TEST_SET];
    printf("%f\n",b_test[i] );
    f_test[i]=force[i+MAX_ELEMENT-TEST_SET];
    printf("%f\n",f_test[i] );
    }





  return 0;
  }
