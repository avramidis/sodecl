## Calls the SODECL executable.
#
#  Executes the SODECL executable in the simulator folder.
#
#  @param runpath       is the path to the folder the executable will run.
#  @param outputpath    is the path to the folder that the output files will be moves to.
#  @param exepath       is the path to the mpem executable.
#
#  @return              the code from the execution command. If the call of the executable is successful then 0 is returned.
#
#  @exception           FileNotFoundError if the executable file is not found.

import numpy
import os

def callsodecl(runpath, outputpath = None, exepath = None ):
	import subprocess
	import platform

	if outputpath is None:
		outputpath = runpath

	if exepath is None:
		if platform.system() == 'Windows':
			exepath = 'sodeclexe.exe'
		else:
			exepath = 'sodeclexe.exe'

	try:
		## Run the exe without output
		#process = subprocess.Popen(exepath+" "+str(runid), cwd=runpath, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
		# process = subprocess.Popen(exepath, cwd=runpath, stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=True)
		# stdout, stderr = process.communicate()

		platform = 1
		device = 0
		kernel = 'kernels/broomhead.cl'
		odesolver = 0
		orbits = 1
		nequat = 6
		nparams = 9
		nnoi = 3
		dt = 1e-7
		tspan = 1
		ksteps = 4000
		localgroupsize = 0

		runcommandstr = ['sodeclexe.exe ' + str(platform) + ' ' + str(device) + ' '
			+ kernel + ' x_y0.bin x_params.bin ' + str(odesolver) + ' '
			+ str(orbits) + ' ' + str(nequat) + ' ' + str(nparams) + ' '
			+ str(nnoi) + ' ' + str(dt) + ' ' + str(tspan) + ' '
			+ str(ksteps) + ' ' + str(localgroupsize)]

		print(runcommandstr[0])
		# Run the exe with output
		process = subprocess.Popen(runcommandstr[0], cwd=runpath)
		#process = subprocess.Popen(exepath, cwd=runpath)

		# process = subprocess.Popen(runcommandstr[0], cwd=runpath, stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=True)
		# stdout, stderr = process.communicate()

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

	# print(process.returncode)
	return process.returncode


def sodecl(openclplatform, opencldevice, openclkernel,
			initx, params, solver,
			orbits, nequat, nnoiseprocesses,
			dt, tspan, ksteps, localgroupsize):
	pass

	# Save binary files with the parameters and initial states.
	# x_params.bin
	# x_t0.bin
	# x_y0.bin


	if os.path.exists('sodecloutput.bin'):
			os.remove('sodecloutput.bin')

	if os.path.exists('x_t0.bin'):
		os.remove('x_t0.bin')

	if os.path.exists('x_y0.bin'):
		os.remove('x_y0.bin')

	if os.path.exists('x_params.bin'):
		os.remove('x_params.bin')

	t0 = numpy.array([0])
	t0.tofile('x_t0.bin')
	initx.tofile('x_y0.bin')
	params.tofile('x_params.bin')

if __name__ ==  '__main__':
	openclplatform = 1
	opencldevice = 0
	openclkernel = 'kernels/broomhead.cl'
	solver = 0
	orbits = 1
	nequat = 6
	nparams = 6
	nnoi = 3
	dt = 1e-7
	tspan = 0.1
	ksteps = 4000
	localgroupsize = 0

	pop=1
	#initx = numpy.array([5,1], ndmin=2)
	# initx = numpy.zeros((1,1))
	# print(type(initx))
	# print(initx[0].size)
	# print(initx)


	initx = numpy.array([0, 0, 0, 0, 0, 0.7])
	#print(type(initx))

	params = numpy.array([120, 1.5, 0.0045, 0.05, 600, 9, 0, 0.002, 0])
	#pop=size(x_params,1)

	nnoiseprocesses = 3

	sodecl(openclplatform, opencldevice, openclkernel,
				initx, params, solver,
				orbits, nequat, nnoiseprocesses,
				dt, tspan, ksteps, localgroupsize)

	callsodecl('.')

	f = open("sodecloutput.bin", "r")
	a = numpy.fromfile(f, dtype=numpy.float)

	import matplotlib.pyplot as plt
	plt.plot(a)
	plt.ylabel('some numbers')
	plt.show()
	#
	# print(a)
