#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <errno.h>
#include <time.h>

#define MAX_DATA 200
#define MAX_ELEMENT 50
#define TEST_SET 10
#define epoch


// Parser functions ****************************************************


void shufflem(float *array,float *array1, size_t n)
{
    if (n > 1)
    {
        size_t i;
        for (i = 0; i < n - 1; i++)
        {

          size_t j = rand() % n;
          size_t i= rand() % n;
        //  printf("%zd and %zd \n",i,j );
          float t = array[j];
          array[j] = array[i];
          array[i] = t;

          float t1= array1[j];
          array1[j] = array1[i];
          array1[i] = t1;
        }
    }
}

void die(const char *message)
{
    if(errno) {
        perror(message);
    } else {
        printf("ERROR: %s\n", message);
    }

    exit(1);
}

struct Inputs{ // Child
    char lines[MAX_DATA];
};

struct Input { // Parent
   FILE *file;
   struct Inputs I[MAX_ELEMENT];
};

void InputsLoad(struct Input *inp)
{
int i;
  for (i=0;i<MAX_ELEMENT; i++)
  {
    fgets(inp->I[i].lines, MAX_DATA,inp->file);
    //printf("current line is: %s\n",inp->I[i].lines);
  }
}


struct Input *File_Open(const char *filename)
{
struct Input *inp = malloc(sizeof(struct Input ));
if(!inp) die("Failed to create Input Structure");
inp->file = fopen(filename,"r+");
if(inp->file) InputsLoad(inp);
else die("Failed to open the file");
return inp;
}


char* getfield(char * line, int num) // replace w your own
{
   char* tok;
      for (tok = strtok(line, ",");  tok;  tok = strtok(NULL, ","))
      {
          if (!--num)
              return tok;
      }
      return NULL;
  }


struct Input *File_Parse(struct Input *inp, int col)
{
  int i;
  for(i = 0; i< MAX_ELEMENT; i++)
  {
    char *str ;
    char *m;
    m = malloc(MAX_DATA);
    str = strdup(inp->I[i].lines);
    m = getfield(str,col);
  //  printf("%s\n",m); // error due to size here
  m = strtok(m,"\n");
    strcpy(inp->I[i].lines ,m);
    if(str)free(str);
  }
  return inp;
}


void InputsPrint(struct Input *inp)
{
  int i;
  for( i = 0; i< MAX_ELEMENT; i++)
  {
printf("line %d : %s\n",i,inp->I[i].lines);
  }
}


void Closefile(struct Input *inp)
{

  if(inp) {
          if(inp->file) fclose(inp->file);
          free(inp);
      }
}
//****************************************************

int main(int argc, char* argv[])
{
  if(argc < 2 || argc > 4) die("USAGE: ex17 <dbfile>");
  char *filename = argv[1];

  //////////////////////////////////////////// Reading and parsing .csv File
  int biceps_col = atoi(argv[2]);
  struct Input *inp = File_Open(filename);
  inp = File_Parse(inp ,biceps_col);
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
