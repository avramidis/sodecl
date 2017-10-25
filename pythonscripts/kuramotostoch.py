# ---------------------------------------------------------------------------//
#  Copyright (c) 2017 Eleftherios Avramidis <el.avramidis@gmail.com>
#
#  Distributed under The MIT License (MIT)
#  See accompanying file LICENSE.txt
# ---------------------------------------------------------------------------//

import numpy
import numpy.matlib
import random
import sodecl
import time

if __name__ == '__main__':
#def runbench(orbits):

    start = time.time()

    #orbits=8
    #orbits=512
    #orbits=5120
    #orbits=25600
    #orbits=40960
    orbits=81920
    #orbits=163840
    #orbits = 327680

    openclplatform = 0
    opencldevice = 1
    openclkernel = 'kernels/kuramoto.cl'
    solver = 0
    nequat = 20
    nparams = nequat
    nnoi = nequat
    dt = 5e-2
    tspan = 200
    ksteps = 40
    localgroupsize = 8

    # Initialise initial state of the system
    initx = numpy.ndarray((orbits, nequat))
    for o in range(orbits):
        for p in range(nequat):
            initx[o][p] = random.uniform(-3.1416, 3.1416)

    # Initialise parameters values of the system
    params = numpy.ndarray((orbits, nequat))
    for o in range(orbits):
        for p in range(nequat):
            params[o][p] = random.uniform(0.01, 0.03)

    # Initialise noise values of the system
    noise = numpy.ndarray((orbits, nequat))
    for o in range(orbits):
        for p in range(nequat):
            noise[o][p] = random.uniform(0.01, 0.03)

    nparams = numpy.concatenate((params, noise), axis=1)

    sodecl.sodecl(openclplatform, opencldevice, openclkernel,
                  initx, params, solver,
                  orbits, nequat, nnoi,
                  dt, tspan, ksteps, localgroupsize)

    f = open("sodecloutput.bin", "r")
    a = numpy.fromfile(f, dtype=numpy.float)
    a = a.reshape(orbits*20, int(a.shape[0] / (orbits*20)), order='F')

    end = time.time()
    print("Simulation execution time: ", end - start, " seconds.")

    # print("NaN present: " + str(numpy.isnan(numpy.sum(numpy.sum(a)))))

    if numpy.isnan(numpy.sum(numpy.sum(a))):
    	raise RuntimeError("NaN present!")

    # import matplotlib.pyplot as plt
    # plt.plot(a[0, :])
    # plt.xlabel('Time')
    # plt.ylabel('Value')
    # plt.show()

    # return end - start
