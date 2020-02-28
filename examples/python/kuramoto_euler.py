# ---------------------------------------------------------------------------//
#  Copyright (c) 2018 Eleftherios Avramidis <el.avramidis@gmail.com>
#
#  Distributed under The MIT License (MIT)
#  See accompanying file LICENSE
# ---------------------------------------------------------------------------//

import random
import numpy
import sodecl
import matplotlib.pyplot as plt

if __name__ == '__main__':

    openclplatform = 0
    opencldevice = 0
    openclkernel = 'kuramoto.cl'
    solver = 1
    dt = 0.05
    tspan = 50
    ksteps = 40
    localgroupsize = 0

    orbits = 2
    nequat = 3
    nparams = 4
    nnoi = 0

    # Initial conditions
    initx = numpy.ndarray((orbits, nequat))
    for o in range(orbits):
        for p in range(nequat):
            initx[o][p] = random.uniform(-3.1416, 3.1416)

    # Parameters values
    params = numpy.ndarray((orbits, nparams))
    for o in range(orbits):
        params[o][0] = 0.2
        for p in range(1,nequat+1):
            params[o][p] = random.uniform(0.2, 0.4)

    t, results = sodecl.sodecl(openclplatform, opencldevice, openclkernel,
                            initx, params, solver,
                            orbits, nequat, nnoi,
                            dt, tspan, ksteps, localgroupsize)

    plt.plot(t, results[0, :])
    plt.xlabel('Time')
    plt.ylabel('Value')
    plt.show()