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
	solver = 'ie'
	nequat = 6
	nnoi = 0
	dt = 5e-6
	tspan = 6
	ksteps = 80
	localgroupsize = 0

	initx = numpy.array([0, 0, 0, 0, 0, 0.7])
	initx = numpy.matlib.repmat(initx, orbits, 1)

	params = numpy.array([120, 1.5, 0.0045, 0.05, 600, 9])
	params = numpy.matlib.repmat(params, orbits, 1)

	nnoiseprocesses = 0

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
