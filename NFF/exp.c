#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define e(N) expf(N)
#define sqr(N) (N*N)
#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

float solve (float m1, float v1, float m2, float v2)
{
float s1 = v1*v1;
float s2 = v2*v2;

float a = 1/(2*s1) - 1/(2*s2);
float b = (m2/s2) - (m1/s1);
float c = sqr(m1)/(2*s1) - sqr(m2)/(2*s2) - logf(v2/v1);

float delta = b*b - 4*a*c;
//printf("a: %f\n", a);
//printf("b: %f\n", (m2/s1));
//printf("c :%f\n", c);
//printf("\tdelta %f\n", delta);
float o1 =0;
float o2 =0;
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

//printf("intersection :%s | at %f\n ", flag ? "true" : "false", max(o1,o2) );
return max(o1,o2);

}

int main(int argc, char const *argv[]){

solve(2.5, 3.0 , 5.0, 1.0);
//  printf("%f\n", e(atof(argv[1])) );
  return 0;
}
