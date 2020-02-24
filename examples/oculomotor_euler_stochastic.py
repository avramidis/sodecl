# ---------------------------------------------------------------------------//
#  Copyright (c) 2017 Eleftherios Avramidis <el.avramidis@gmail.com>
#
#  Distributed under The MIT License (MIT)
#  See accompanying file LICENSE
# ---------------------------------------------------------------------------//

import numpy
import sodecl
import matplotlib.pyplot as plt

if __name__ == '__main__':

    openclplatform = 0
    opencldevice = 0
    openclkernel = 'oculomotor.cl'
    solver = 0
    dt = 1e-8
    tspan = 1
    ksteps = 40000
    localgroupsize = 0

    orbits = 2
    nequat = 6
    nparams = 6
    nnoi = 2

    # Initial conditions
    initx = numpy.ndarray((orbits, nequat))
    for o in range(orbits):
        initx[o][0] = 0.0
        initx[o][1] = 0.0
        initx[o][2] = 0.0
        initx[o][3] = 0.0
        initx[o][4] = 0.0
        initx[o][5] = 2.0

    # Parameters values
    params = numpy.ndarray((orbits, nparams))
    for o in range(orbits):
        params[o][0] = 120
        params[o][1] = 1.5
        params[o][2] = 0.0045
        params[o][3] = 0.05
        params[o][4] = 600
        params[o][5] = 9

    # Noise parameters values
    noise = numpy.ndarray((orbits, nnoi))
    for o in range(orbits):
        noise[o][0] = 0.05
        noise[o][1] = 50

    params = numpy.concatenate((params, noise), axis=1)

    t, results = sodecl.sodecl(openclplatform, opencldevice, openclkernel,
                            initx, params, solver,
                            orbits, nequat, nnoi,
                            dt, tspan, ksteps, localgroupsize)

    plt.plot(t, results[0, :])
    plt.xlabel('Time')
    plt.ylabel('Value')
    plt.show()
