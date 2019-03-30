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
import scipy.io

def kuramotostoch(Kval, orbits, openclplatform, opencldevice, nequat, dt, ksteps, localgroupsize):
    start_time = time.time()

    openclkernel = 'kuramoto.cl'
    solver = 0
    nparams = nequat+1
    nnoi = nequat
    tspan = 200

    # Initialise initial state of the system
    initx = numpy.ndarray((orbits, nequat))
    
    for p in range(nequat):
        par = random.uniform(-3.1416, 3.1416)
        for o in range(orbits):
            initx[o][p] = par

    # Initialise parameters values of the system
    params = numpy.ndarray((orbits, nparams))
    
    for o in range(orbits):
        params[o][0] = Kval
    for p in range(1,nequat+1):
        par = random.uniform(0.2, 0.4)
        for o in range(orbits):
            params[o][p] = par

    # Initialise noise values of the system
    noise = numpy.ndarray((orbits, nequat))
    for p in range(nequat):
        par = random.uniform(0.01, 0.03)
        for o in range(orbits):
            noise[o][p] = par

    params = numpy.concatenate((params, noise), axis=1)

    results = sodecl.sodecl(openclplatform, opencldevice, openclkernel,
                              initx, params, solver,
                              orbits, nequat, nnoi,
                              dt, tspan, ksteps, localgroupsize)

    end_time = time.time()
    print("Simulation execution time: ", end_time - start_time, " seconds.")

    if numpy.isnan(numpy.sum(numpy.sum(results))):
        raise RuntimeError("NaN present!")


    mdict = {   'Nval': nequat,
                'Kval': Kval,
                'wdist': initx,
                'pdist': params,
                'inity': initx,
                'delt': dt,
                'tmax': 400,
                'numsamps': ksteps,
                'nsims': orbits,
                'routine': '@kuramotosimstochmult',
                'ymat': results}

    return mdict

if __name__ == '__main__':

    #start_time = time.time()

    orbits = 64
    openclplatform = 0
    opencldevice = 0
    nequat = 3
    dt = 0.05
    ksteps = 4
    localgroupsize = 8
    
    Kval = 0.02
    bsmat1 = kuramotostoch(Kval, orbits, openclplatform, opencldevice, nequat, dt, ksteps, localgroupsize)
    scipy.io.savemat('bsmat1.mat', {'bsmat1':bsmat1})

    Kval = 0.2
    bsmat2 = kuramotostoch(Kval, orbits, openclplatform, opencldevice, nequat, dt, ksteps, localgroupsize)
    scipy.io.savemat('bsmat2.mat', {'bsmat2':bsmat2})

    results = bsmat1['ymat']
    import matplotlib.pyplot as plt
    plt.plot(results[0, :])
    plt.xlabel('Time')
    plt.ylabel('Value')
    plt.show()


