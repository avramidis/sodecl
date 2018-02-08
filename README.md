# SODECL

SODECL is a library of ordinary differential equation (ODE) and stochastic differential equation (SDE) solvers in OpenCL. 
Using SODECL the user can calculate the integration orbits of a ODE or SDE system for a large number of different parameter values.

One example of the usefullness of SODECL is the fitting an ODE system representing a biological model to experimental data.  When optimising an ODE biological model using for example genetic algorithms the ODE model has to be calculated for a large number of different parameter values combinations. ODECL accelerates the calculation of these ODE model orbit integrations by taking advantage of all the cores of a CPU or the parallel capabilities of a GPU. 

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

To get the code you will need git. To install git on Ubuntu use the following commands in a terminal

```
sudo apt update
sudo apt install git
```

To pull the repository from github use the following command in a terminal

```
git clone https://github.com/avramidis/sodecl.git
```

You will need lsb-core, to install it you the following command in a terminal

```
sudo apt install olsb-core
```

You will need the OpenCL Runtime for Intel Core and Intel Xeon Processors to run SODECL on an Intel CPU. The runtime can be downloaded from https://software.intel.com/en-us/articles/opencl-drivers#latest_CPU_runtime. In the folder with the downloaded tgz file use the following commands in a terminal

```
tar zxvf opencl_runtime_16.1.2_x64_sles_6.4.0.37.tgz
cd opencl_runtime_16.1.2_x64_sles_6.4.0.37
sudo ./install_GUI.sh
```

You will need to install the OpenCL library. To do that use the following commands in a terminal

```
sudo apt install ocl-icd-opencl-dev
```

To generate the makefile for compiling SODECL you will need cmake. To install cmake on Ubuntu use the following command in a terminal

```
sudo apt install cmake
```

To use the Python wrapper a number of module must be installed. To install these modules use the following commands in a terminal

```
sudo apt-get install python3-numpy
```

### Installing

A step by step series of examples that tell you have to get a development env running

To generate the makefiles use the following commands in a terminal in the sodecl folders

```
mkdir build
cd build
cmake ../src/.
```

To build SODECL use the following command

```
make
```

## Currect fixed step solvers

1. Euler
2. Runge-Kutta
3. Implicit Euler
4. Implicit midpoint
5. Euler-Maruyama

## Simple wrappers for

1. Python
2. MATLAB

## Future work

1. More ODE and SDE solvers
2. Variable step ODE solvers
3. Benchmarks
4. Support for using multiple devices (e.g. CPUs+GPUs)
5. Optimisation for AMD and NVIDIA GPUs
6. Tutorials
7. Doxygen documentation

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details

## Acknowledgments

* The Random123 library found at https://www.deshawresearch.com/resources_random123.html is used
* This readme file is based on the template found at https://gist.github.com/PurpleBooth/109311bb0361f32d87a2
* The initial work on this library was done as part of a PhD work presented in the publication Avramidis, Eleftherios, and Ozgur E. Akman. "Optimisation of an exemplar oculomotor model using multi-objective genetic algorithms executed on a GPU-CPU combination." BMC systems Biology 11.1 (2017): 40. DOI: 10.1186/s12918-017-0416-2 
