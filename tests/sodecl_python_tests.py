import unittest
import sys
import numpy
import random
import time
import sodecl
import matplotlib

class TestPythonWrapper(unittest.TestCase):

    def test_oculomotor_euler(self):
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

        # Initialise initial state of the system
        initx = numpy.ndarray((orbits, nequat))
        for o in range(orbits):
            initx[o][0] = 0.0
            initx[o][1] = 0.0
            initx[o][2] = 0.0
            initx[o][3] = 0.0
            initx[o][4] = 0.0
            initx[o][5] = 2.0

        # Initialise parameters values of the system
        params = numpy.ndarray((orbits, nparams))
        for o in range(orbits):
            params[o][0] = 120
            params[o][1] = 1.5
            params[o][2] = 0.0045
            params[o][3] = 0.05
            params[o][4] = 600
            params[o][5] = 9

        results = sodecl.sodecl(openclplatform, opencldevice, openclkernel,
                                initx, params, solver,
                                orbits, nequat, nnoi,
                                dt, tspan, ksteps, localgroupsize)

        import matplotlib.pyplot as plt
        plt.plot(results[0, :])
        plt.xlabel('Time')
        plt.ylabel('Value')
        plt.show()

        # if numpy.isnan(len(results)):
        #     raise RuntimeError("NaN present!")
        #
        # # print("Simulation execution time: ", end_time - start_time, " seconds.")
        # if numpy.isnan(numpy.sum(numpy.sum(results))):
        #     raise RuntimeError("NaN present!")

        self.assertGreater(results.size, 0)

    def kuramoto_stochastic_euler(self):

        orbit_set = [512]
        nequat_set = [15]
        repetitions = 1

        openclplatform = 0
        opencldevice = 0
        openclkernel = 'kuramoto.cl'
        solver = 0
        dt = 0.05
        tspan = 50
        ksteps = 40
        localgroupsize = 0

        runtimes = numpy.zeros((len(nequat_set), len(orbit_set), repetitions + 1))

        nequat_count = 0
        for nequat in nequat_set:
            orbits_count = 0
            for orbits in orbit_set:
                reps_count = 0
                for reps in range(repetitions):
                    print("nequat: ", nequat_set[nequat_count])
                    print("norbits: ", orbit_set[orbits_count])
                    print("rep: ", reps)

                    time.sleep(3)
                    start_time = time.time()

                    nparams = nequat + 1
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
                        for p in range(1, nequat + 1):
                            params[o][p] = random.uniform(0.2, 0.4)

                    # Initialise noise values of the system
                    noise = numpy.ndarray((orbits, nequat))
                    for o in range(orbits):
                        for p in range(nequat):
                            noise[o][p] = random.uniform(0.01, 0.03)

                    params = numpy.concatenate((params, noise), axis=1)

                    results = pythonscripts.sodecl.sodecl(openclplatform, opencldevice, openclkernel,
                                                          initx, params, solver,
                                                          orbits, nequat, nnoi,
                                                          dt, tspan, ksteps, localgroupsize)

                    end_time = time.time()

                    # print("Simulation execution time: ", end_time - start_time, " seconds.")
                    if numpy.isnan(numpy.sum(numpy.sum(results))):
                        raise RuntimeError("NaN present!")

                    runtimes[nequat_count][orbits_count][reps_count] = end_time - start_time
                    reps_count = reps_count + 1
                    print("")

                runtimes[nequat_count][orbits_count][reps_count] = numpy.mean(
                    runtimes[nequat_count][orbits_count][0:repetitions - 1])

                orbits_count = orbits_count + 1
            nequat_count = nequat_count + 1

            if numpy.isnan(len(results)):
                raise RuntimeError("NaN present!")

            self.assertGreater(results.size, 0)

    def oculomotor_stochastic_euler(self):

        openclplatform = 0
        opencldevice = 0
        openclkernel = 'oculomotor.cl'
        solver = 0
        dt = 1e-8
        tspan = 1
        ksteps = 40000
        localgroupsize = 0

        orbits = 8
        nequat = 6
        nparams = 6
        nnoi = 2

        # Initialise initial state of the system
        initx = numpy.ndarray((orbits, nequat))
        for o in range(orbits):
            initx[o][0] = 0.0
            initx[o][1] = 0.0
            initx[o][2] = 0.0
            initx[o][3] = 0.0
            initx[o][4] = 0.0
            initx[o][5] = 2.0

        # Initialise parameters values of the system
        params = numpy.ndarray((orbits, nparams))
        for o in range(orbits):
            params[o][0] = 120
            params[o][1] = 1.5
            params[o][2] = 0.0045
            params[o][3] = 0.05
            params[o][4] = 600
            params[o][5] = 9

        # Initialise noise values of the system
        noise = numpy.ndarray((orbits, 2))
        for o in range(orbits):
            noise[o][0] = 0.04
            noise[o][1] = 20.0

        params = numpy.concatenate((params, noise), axis=1)

        results = pythonscripts.sodecl.sodecl(openclplatform, opencldevice, openclkernel,
                                              initx, params, solver,
                                              orbits, nequat, nnoi,
                                              dt, tspan, ksteps, localgroupsize)

        import matplotlib.pyplot as plt
        plt.plot(results[0, :])
        plt.xlabel('Time')
        plt.ylabel('Value')
        plt.show()

        if numpy.isnan(len(results)):
            raise RuntimeError("NaN present!")

        # print("Simulation execution time: ", end_time - start_time, " seconds.")
        if numpy.isnan(numpy.sum(numpy.sum(results))):
            raise RuntimeError("NaN present!")

        self.assertGreater(results.size, 0)


if __name__ == '__main__':
    unittest.main()
