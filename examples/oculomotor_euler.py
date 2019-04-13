import numpy
import sodecl
import matplotlib.pyplot as plt

if __name__ == '__main__':

    openclplatform = 0
    opencldevice = 0
    openclkernel = 'oculomotor.cl'
    solver = 1
    dt = 1e-8
    tspan = 0.1
    ksteps = 40000
    localgroupsize = 0

    orbits = 2
    nequat = 6
    nparams = 6
    nnoi = 0

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

    t, results = sodecl.sodecl(openclplatform, opencldevice, openclkernel,
                            initx, params, solver,
                            orbits, nequat, nnoi,
                            dt, tspan, ksteps, localgroupsize)

    plt.plot(t, results[0, :])
    plt.xlabel('Time')
    plt.ylabel('Value')
    plt.show()
