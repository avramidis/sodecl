# ---------------------------------------------------------------------------//
#  Copyright (c) 2017 Eleftherios Avramidis <el.avramidis@gmail.com>
#
#  Distributed under The MIT License (MIT)
#  See accompanying file LICENSE.txt
# ---------------------------------------------------------------------------//

import numpy
import os
import subprocess
import platform

## Calls the SODECL executable.
#
#  Executes the SODECL executable.
#
#  @param openclplatform       	is the number of the OpenCL platform.
#  @param opencldevice       	is the is the number of the OpenCL device for the selected platform.
#  @param openclkernel       	is the path to the OpenCL kernel with the definition of the SODE system.
#  @param initx       			is the array with the initial state of the SODE system.
#  @param params       			is the array with the parameters of the SODE system.
#  @param sodesolver       		is the integration solver for the SODE system.
#  @param orbits       			is the number of orbits to integrate the SODE system.
#  @param nequat       			is the number of equations of the SODE system.
#  @param nnoiseprocesses       is the number of noise processes needed by the SODE system.
#  @param dt       				is the time step for the SODE solver.
#  @param tspan       			is the time span for the integration of the SODE system.
#  @param ksteps       			is the number of SODE solver step for each call to the OpenCL device.
#  @param localgroupsize       	is the size of the OpenCL local group size.
#
#  @return              		the code from the execution command. If the call of the executable is successful then 0 is returned.
#
#  @exception           		FileNotFoundError if the executable file is not found.
def sodecl(openclplatform, opencldevice, openclkernel,
			initx, params, sodesolver,
			orbits, nequat, nnoiseprocesses,
			dt, tspan, ksteps, localgroupsize):

	if os.path.exists('sodecloutput.bin'):
			os.remove('sodecloutput.bin')

	if os.path.exists('x_t0.bin'):
		os.remove('x_t0.bin')

	if os.path.exists('x_y0.bin'):
		os.remove('x_y0.bin')

	if os.path.exists('x_params.bin'):
		os.remove('x_params.bin')

	t0 = numpy.array([0])
	t0 = numpy.matlib.repmat(t0, orbits, 1)
	t0.tofile('x_t0.bin')
	initx.tofile('x_y0.bin')
	params.tofile('x_params.bin')

	# print(params.size)
	# print(params[0].size)

	nparams = params[0].size
	print(nparams)

	runcommandstr = ['sodeclexe.exe ' + str(openclplatform) + ' ' + str(opencldevice) + ' '
		+ openclkernel + ' x_y0.bin x_params.bin ' + str(sodesolver) + ' '
		+ str(orbits) + ' ' + str(nequat) + ' ' + str(nparams) + ' '
		+ str(nnoiseprocesses) + ' ' + str(dt) + ' ' + str(tspan) + ' '
		+ str(ksteps) + ' ' + str(localgroupsize)]

	try:
		#print(runcommandstr[0])

		## Run the exe without output
		# process = subprocess.Popen(runcommandstr[0], cwd='.', stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=True)
		# stdout, stderr = process.communicate()

		# Run the exe with output
		process = subprocess.Popen(runcommandstr[0], cwd='.')

	except FileNotFoundError:
		print("\n")
		print("Error: The executable was not found or cannot be executed!")
		print("Possible fix: Make sure that the executable path is correct and it has execute permision.")
		print("\n")
		raise
	else:
		process.wait()

	# try:
	#     #file_object = open('logs/logfile' + os.path.basename(runpath) + '.txt', 'w')
	#     with open(runpath + '/simulatorconsolelogfile.txt', 'w') as file_object:
	#         file_object.write(stdout)
	#     with open(runpath + '/simulatorerrorlogfile.txt', 'w') as file_object:
	#         file_object.write(stderr)
	# except FileNotFoundError:
	#     print("\n")
	#     print("Error: A logfile was not found or cannot be accessed!")
	#     print("Possible fix: Make sure that the path is correct.")
	#     print("\n")
	#     raise

	return process.returncode

# if __name__ ==  '__main__':
#
# 	openclplatform = 1
# 	opencldevice = 0
# 	openclkernel = 'kernels/broomhead.cl'
# 	solver = 0
# 	orbits = 1
# 	nequat = 6
# 	nparams = 9
# 	nnoi = 3
# 	dt = 1e-7
# 	tspan = 6
# 	ksteps = 4000
# 	localgroupsize = 0
#
# 	pop=1
# 	#initx = numpy.array([5,1], ndmin=2)
# 	# initx = numpy.zeros((1,1))
# 	# print(type(initx))
# 	# print(initx[0].size)
# 	# print(initx)
#
#
# 	initx = numpy.array([0, 0, 0, 0, 0, 0.7])
# 	#print(type(initx))
#
# 	params = numpy.array([120, 1.5, 0.0045, 0.05, 600, 9, 0, 0.002, 0])
# 	#pop=size(x_params,1)
#
# 	nnoiseprocesses = 3
#
# 	sodecl(openclplatform, opencldevice, openclkernel,
# 				initx, params, solver,
# 				orbits, nequat, nnoiseprocesses,
# 				dt, tspan, ksteps, localgroupsize)
#
# 	f = open("sodecloutput.bin", "r")
# 	a = numpy.fromfile(f, dtype=numpy.float)
#
# 	import matplotlib.pyplot as plt
# 	plt.plot(a)
# 	plt.ylabel('some numbers')
# 	plt.show()
