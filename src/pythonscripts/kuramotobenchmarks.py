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
    orbit_set = [512, 5120, 25600, 40960, 81920]
    nequat_set = [5, 10, 15]
    repetitions = 2

    # orbit_set = [8]
    # nequat_set = [15]
    # repetitions = 2

    openclplatform = 1
    opencldevice = 0
    openclkernel = 'kuramoto.cl'
    solver = 0    
    dt = 0.05
    tspan = 50
    ksteps = 40
    localgroupsize = 256

    runtimes = numpy.zeros((len(nequat_set), len(orbit_set), repetitions+1))

    nequat_count = 0
    for nequat in nequat_set:
        orbits_count = 0
        for orbits in orbit_set:
            reps_count = 0
            for reps in range(repetitions):
                print("nequat: ", nequat_set[nequat_count])
                print("norbits: ", orbit_set[orbits_count])
                print("rep: ", reps)
                
                time.sleep(1)
                start_time = time.time()
                
                nparams = nequat+1
                nnoi = nequat

                # Initialise initial state of the system
                initx = numpy.ndarray((orbits, nequat))
                for o in range(orbits):
                    for p in range(nequat):
                        initx[o][p] = random.uniform(-3.1416, 3.1416)

                # Initialise parameters values of the system
                params = numpy.ndarray((orbits, nparams))
                for o in range(orbits):
                    params[o][0] = 0.2
                    for p in range(1,nequat+1):
                        params[o][p] = random.uniform(0.2, 0.4)

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

                # import matplotlib.pyplot as plt
                # plt.plot(results[0, :])
                # plt.xlabel('Time')
                # plt.ylabel('Value')
                # plt.show()

                #print("Simulation execution time: ", end_time - start_time, " seconds.")
                if numpy.isnan(numpy.sum(numpy.sum(results))):
                    raise RuntimeError("NaN present!")

                runtimes[nequat_count][orbits_count][reps_count] = end_time - start_time
                reps_count = reps_count + 1
                print("")

            runtimes[nequat_count][orbits_count][reps_count] = numpy.mean(runtimes[nequat_count][orbits_count][0:repetitions-1])
            
            orbits_count = orbits_count + 1
        nequat_count = nequat_count + 1

    print(runtimes)

    #####################################################
    ## Write the results to files
    numpy.savetxt('nequat_5.txt', runtimes[0][:][:], fmt='%.4f')
    numpy.savetxt('nequat_10.txt', runtimes[1][:][:], fmt='%.4f')
    numpy.savetxt('nequat_25.txt', runtimes[2][:][:], fmt='%.4f')