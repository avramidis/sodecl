# ---------------------------------------------------------------------------//
#  Copyright (c) 2017 Eleftherios Avramidis <el.avramidis@gmail.com>
#
#  Distributed under The MIT License (MIT)
#  See accompanying file LICENSE.txt
# ---------------------------------------------------------------------------//

import random
import time
import numpy
import sodecl

if __name__ == '__main__':

    start_time = time.time()

    orbits = 25600
    openclplatform = 1
    opencldevice = 0
    openclkernel = 'kuramoto.cl'
    solver = 0
    nequat = 3
    nparams = nequat
    nnoi = nequat
    dt = 5e-2
    tspan = 200
    ksteps = 40
    localgroupsize = 256

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

    params = numpy.concatenate((params, noise), axis=1)
    results = sodecl.sodecl(openclplatform, opencldevice, openclkernel,
                              initx, params, solver,
                              orbits, nequat, nnoi,
                              dt, tspan, ksteps, localgroupsize)

    end_time = time.time()
    print("Simulation execution time: ", end_time - start_time, " seconds.")

    if numpy.isnan(numpy.sum(numpy.sum(results))):
        raise RuntimeError("NaN present!")

    import matplotlib.pyplot as plt
    plt.plot(results[0, :])
    plt.xlabel('Time')
    plt.ylabel('Value')
    plt.show()
