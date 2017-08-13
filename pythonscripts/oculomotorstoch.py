# ---------------------------------------------------------------------------//
#  Copyright (c) 2017 Eleftherios Avramidis <el.avramidis@gmail.com>
#
#  Distributed under The MIT License (MIT)
#  See accompanying file LICENSE.txt
# ---------------------------------------------------------------------------//

import numpy, numpy.matlib
import sodecl

if __name__ ==  '__main__':

	orbits = 10

	openclplatform = 1
	opencldevice = 0
	openclkernel = 'kernels/broomhead.cl'
	solver = 0
	nequat = 6
	nparams = 9
	nnoi = 3
	dt = 1e-7
	tspan = 6
	ksteps = 4000
	localgroupsize = 0

	initx = numpy.array([0, 0, 0, 0, 0, 0.7])
	initx = numpy.matlib.repmat(initx, orbits, 1)

	params = numpy.array([120, 1.5, 0.0045, 0.05, 600, 9, 0, 0.002, 0])
	params = numpy.matlib.repmat(params, orbits, 1)

	nnoiseprocesses = 3

	sodecl.sodecl(openclplatform, opencldevice, openclkernel,
				initx, params, solver,
				orbits, nequat, nnoiseprocesses,
				dt, tspan, ksteps, localgroupsize)

	f = open("sodecloutput.bin", "r")
	a = numpy.fromfile(f, dtype=numpy.float)

	import matplotlib.pyplot as plt
	plt.plot(a)
	plt.ylabel('some numbers')
	plt.show()
