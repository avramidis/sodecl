# sodecl

**Refactoring: many stuff probably don't work.**

SODECL is a library of ordinary differential equation (ODE) and stochastic differential equation (SDE) solvers in OpenCL. 
Using SODECL the user can calculate the integration orbits of a ODE or SDE system for a large number of different parameter values.  

One example of the usefullness of SODECL is the fitting an ODE system representing a biological model to experimental data.  When optimising an ODE biological model using for example genetic algorithms the ODE model has to be calculated for a large number of different parameter values combinations. ODECL accelerates the calculation of these ODE model orbit integrations by taking advantage of all the cores of a CPU or the parallel capabilities of a GPU. 

**If you use this library you agree to cite the following article:**
Avramidis, Eleftherios, and Ozgur E. Akman. "Optimisation of an exemplar oculomotor model using multi-objective genetic algorithms executed on a GPU-CPU combination." BMC systems Biology 11.1 (2017): 40. DOI: 10.1186/s12918-017-0416-2

**If you use this library you agree to cite the following article:**
Avramidis, Eleftherios, and Ozgur E. Akman. "Optimisation of an exemplar oculomotor model using multi-objective genetic algorithms executed on a GPU-CPU combination." BMC Systems Biology 11.1 (2017): 40. DOI: 10.1186/s12918-017-0416-2

The library is currently at an early stage of development.

Currect fixed step solvers:

1. Euler
2. Runge-Kutta
3. Implicit Euler
4. Implicit midpoint
5. Euler-Maruyama

Current interfaces:

1. MATLAB script interface.

Future work:

1. MATLAB mex interface. There is an issue with Intel Threading Building Blocks when calling OpenCL for an Intel OpenCL device. However, the latest MATLAB version (R2016a) seems not to have this issue.
2. More ODE and SDE solvers.
3. Variable step ODE solvers.
4. Benchmarks.
5. Support for using multiple devices (e.g. CPUs+GPUs).
6. Optimisation for AMD and NVIDIA GPUs.
7. Tutorials.
8. Doxygen documentation