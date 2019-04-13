import unittest
import numpy
import random
import pickle
import sodecl

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

        test_results = pickle.load(open("data/oculomotor_euler.pkl", "rb"))
        self.assertTrue(numpy.isclose(results, results).all())

    def test_oculomotor_rk(self):
        openclplatform = 0
        opencldevice = 0
        openclkernel = 'oculomotor.cl'
        solver = 2
        dt = 1e-6
        tspan = 0.1
        ksteps = 400
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

        test_results = pickle.load(open("data/oculomotor_rk.pkl", "rb"))
        self.assertTrue(numpy.isclose(results, results).all())

if __name__ == '__main__':
    unittest.main()
