# ---------------------------------------------------------------------------//
#  Copyright (c) 2018 Eleftherios Avramidis <el.avramidis@gmail.com>
#
#  Distributed under The MIT License (MIT)
#  See accompanying file LICENSE.txt
# ---------------------------------------------------------------------------//

import random
import time
import numpy
import sodecl

if __name__ == '__main__':
    orbit_set = [512, 5120, 25600, 40960, 81920, 163840, 327680]
    #orbit_set = [512, 5120]
    nequat_set = [3, 10, 20]
    repetitions = 3

    openclplatform = 0
    opencldevice = 0
    openclkernel = 'kuramoto.cl'
    solver = 0    
    dt = 5e-2
    tspan = 200
    ksteps = 40
    localgroupsize = 8

    runtimes = numpy.zeros((len(nequat_set), len(orbit_set), repetitions+1))

    orbits = 512
    nequat_count = 0
    for nequat in nequat_set:
        orbits_count = 0
        for orbits in orbit_set:
            reps_count = 0
            for reps in range(repetitions):
                time.sleep(1)
                start_time = time.time()
                
                nparams = nequat
                nnoi = nequat

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

                results = sodecl.sodecl(openclplatform, opencldevice, openclkernel,
                                        initx, params, solver,
                                        orbits, nequat, nnoi,
                                        dt, tspan, ksteps, localgroupsize)

                end_time = time.time()

                #print("Simulation execution time: ", end_time - start_time, " seconds.")
                if numpy.isnan(numpy.sum(numpy.sum(results))):
                    raise RuntimeError("NaN present!")

                runtimes[nequat_count][orbits_count][reps_count] = end_time - start_time
                reps_count = reps_count + 1
            
            runtimes[nequat_count][orbits_count][reps_count] = numpy.mean(runtimes[nequat_count][orbits_count][0:repetitions-1])
            
            orbits_count = orbits_count + 1
        nequat_count = nequat_count + 1

    print(runtimes)

    #####################################################
    ## Write the results to files
    numpy.savetxt('nequat_3.txt', runtimes[0][:][:], fmt='%.4f')
    numpy.savetxt('nequat_10.txt', runtimes[1][:][:], fmt='%.4f')
    numpy.savetxt('nequat_20.txt', runtimes[2][:][:], fmt='%.4f')