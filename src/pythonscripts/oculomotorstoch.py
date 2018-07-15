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

    start_time = time.time()

    orbits = 8
    openclplatform = 0
    opencldevice = 0
    openclkernel = 'occulomotor.cl'
    solver = 0
    nequat = 6
    nparams = 8
    nnoi = 2
    dt = 1e-7
    tspan = 1
    ksteps = 4000
    localgroupsize = 0

    initx = numpy.array([0, 0, 0, 0, 0, 0.7])
    initx = numpy.matlib.repmat(initx, orbits, 1)

    # low_bounds = [1, 0.1, 0.1, 0, 1, 0.1, 0, 0, 0]
    # upper_bounds = [1000, 60, 0.0001, 12, 1000, 60, 0.5, 0.1, 100]

    # low_bounds = [1, 0.1, 0.1, 0, 1, 0.1, 0, 0, 0]
    # upper_bounds = [1000, 60, 0.0001, 12, 1000, 60, 0, 0, 0]

    # low_bounds = [120, 1.5, 0.0045, 0.05, 600, 9, 0, 0, 0]
    # upper_bounds = [120, 1.5, 0.0045, 0.05, 600, 9, 0.5, 0.1, 100]

    low_bounds = [120, 1.5, 0.0045, 0.05, 600, 9, 0, 0]
    upper_bounds = [120, 1.5, 0.0045, 0.05, 600, 9, 0.5, 0]

    params = numpy.ndarray((orbits, nparams))
    for o in range(orbits):
        for p in range(nparams):
            params[o][p] = random.uniform(low_bounds[p], upper_bounds[p])

    results = sodecl.sodecl(openclplatform, opencldevice, openclkernel,
                              initx, params, solver,
                              orbits, nequat, nnoi,
                              dt, tspan, ksteps, localgroupsize)

    end_time = time.time()
    print("Simulation execution time: ", end_time - start_time, " seconds.")

    # print("NaN present: " + str(numpy.isnan(numpy.sum(numpy.sum(a)))))

    if numpy.isnan(numpy.sum(numpy.sum(results))):
        raise RuntimeError("NaN present!")

    import matplotlib.pyplot as plt
    plt.plot(results[0, :])
    plt.xlabel('Time')
    plt.ylabel('Value')
    plt.show()
