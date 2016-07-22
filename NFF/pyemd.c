#include <python2.7/Python.h>  //pulls in the Python API

#include<stdio.h>
#include <math.h>

#define e(N) expf(N)
#define sqr(N) (N*N)
#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

float solve1 (float m1, float v1, float m2, float v2)
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
o = e(-0.5 * sqr((o1-m1)/v1)) >  e(-0.5 * sqr((o2-m1)/v1))  ? o1 : o2 ;
printf("%f ,  %f \n", e(-0.5 * sqr((o1-m1)/v1)) ,  e(-0.5 * sqr((o2-m1)/v1)) );
printf("intersection :%s | at %f\n ", flag ? "true" : "false", o );
return o;

}
int main()
{
  float m1 = 2.5;
  float   std1 = 0.01;
  float   m2 = 5.0;
  float   std2 = 1.0;
  // Intialize plot
  float result = solve1(m1, std1 , m2, std2);
  Py_Initialize();
   PyRun_SimpleString("import pylab");
   PyRun_SimpleString("import numpy as np");
   PyRun_SimpleString("import matplotlib.pyplot as plt");
   PyRun_SimpleString("def gaussian(x, mu, sig):\n\treturn np.exp(-np.power(x - mu, 2.) / (2 * np.power(sig, 2.)))");
   PyRun_SimpleString("x = np.linspace(-5,9,10000)");

   //PyRun_SimpleString("pylab.plot(x, pylab.normpdf(x, 2.5,3.0 ))");

   static char command[500];
 ///snprintf(command, 500, "pylab.plot(x, pylab.normpdf(x, %f,%f ))\n", m1, std1);
  //PyRun_SimpleString(command);


snprintf(command, 500, "pylab.plot(x, gaussian(x, %f,%f ))\n", m1, std1);
 PyRun_SimpleString(command);

  // Two lines for plotting !
  snprintf(command, 500, "pylab.plot(x, gaussian(x, %f,%f ))\n", m2, std2);
   PyRun_SimpleString(command);

   snprintf(command, 500 ,"pylab.plot(%f,gaussian(%f,%f,%f),'o')\n", result,result, m1, std1);
    PyRun_SimpleString(command);


   // Show plot
   PyRun_SimpleString("pylab.show()");
   Py_Exit(0);



   return 0;
}
