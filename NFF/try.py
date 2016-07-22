import numpy as np
import matplotlib.pyplot as plt

def solve(m1,m2,std1,std2):
  a = 1/(2*std1**2) - 1/(2*std2**2)
  print(a)
  b = m2/(std2**2) - m1/(std1**2)
  print(m2/(std2**2))
  c = m1**2 /(2*std1**2) - m2**2 / (2*std2**2) - np.log(std2/std1)
  print(c)
  return np.roots([a,b,c])

m1 = 2.5
std1 = 3.0
m2 = 5.0
std2 = 1.0

result = solve(m1,m2,std1,std2)
print(result)

x = np.linspace(-5,9,10000)
plot1=plt.plot(x,plt.mlab.normpdf(x,m1,std1))
plot2=plt.plot(x,plt.mlab.normpdf(x,m2,std2))
plot3=plt.plot(result,plt.mlab.normpdf(result,m1,std1),'o')

plt.show()
