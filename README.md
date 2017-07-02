# NFF

The goal of library is to allow programmers the ability to build fuzzy (Type-I and Type-II) neural netwworks with the ability to control the network architecture deeply.

## Architecture
--------

networks development. These tools like theano, caffe and TensorFlow don’t allow the building of neural network on neurons level but rather on layers level. There is a need for a tool that’s based on the philosophy of neurons based architecture that allows construction of highly specialized neural networks like Neuro-fuzzy networks for example. Such a library has been developed – and named NFF- based on C and its main components are introduced with rational in the next paragraph. 
Naturally, neural networks can have neurons connected to a multiple of other neurons. This neuron connection is referred to as “Wire”. Neurons, even though have different functions, they can be defined by one class “Neuron”. However, different neurons can have different inputs “Wires” passed to them, this leads to complications in defining the class. In addition, wires are not unique that means changes or passing a value through one wire doesn’t affect one neuron but rather all the neurons it’s associated with; this lead to special problem in C since pointers – suitable choice for data class with varying length- cannot be passed uniquely to functions or in other words the pointers passed to functions becomes local variables only.

“Wrap” function was used to solve this problem each wires is referenced by a memory location and each neuron input is “Bundle” of these “Wires”.
To create two wires from the input “Biceps” this is done as follows:

```
#define Wrap(B,W,I) B.addr[I]=(unsigned long int)&W

Wire Bicep_Z
Bundle ZZ = newBundle(2);
Wrap(ZZ,Bicep_Z,0);
Wrap(ZZ,Tricep_Z,1);
Bundle ZM = newBundle(2);
Wrap(ZM,Bicep_Z,0);
Wrap(ZM,Tricep_M,1);
```
