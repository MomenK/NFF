 #ifndef _thing_h
 #define _thing_h
 #include<stdio.h>
 #include<stdlib.h>
 #include<string.h>
 #include <errno.h>
 #include <time.h>

#define MAX_DATA 200
#define MAX_ELEMENT 66 // or TEST_SET
#define TEST_SET 10



 struct Inputs{ // Child of one Line
     char lines[MAX_DATA];
 };

 struct Input { // Parent reads only one Coloum!
    FILE *file;
    struct Inputs I[MAX_ELEMENT]; // Rows !
 };


void shufflem(float *array,float *array1, size_t n);
void die(const char *message);
void InputsLoad(struct Input *inp);
struct Input *File_Open(const char *filename);
char* getfield(char * line, int num) ;
struct Input *File_Parse(struct Input *inp, int col);
void InputsPrint(struct Input *inp);
void Closefile(struct Input *inp);


 #endif
