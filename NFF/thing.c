#include "thing.h"


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
