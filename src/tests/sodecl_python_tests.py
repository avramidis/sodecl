import unittest
import sys
import numpy
import random
sys.path.append('..')
import pythonscripts.sodecl

class TestPythonWrapper(unittest.TestCase):
    
    def test_simple(self):
        orbits = 4
        nequat = 3

        openclplatform = 0
        opencldevice = 0
        openclkernel = 'kuramoto.cl'
        solver = 0    
        dt = 5e-2
        tspan = 200
        ksteps = 40
        localgroupsize = 0

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

        results = pythonscripts.sodecl.sodecl(openclplatform, opencldevice, openclkernel,
                                                initx, params, solver,
                                                orbits, nequat, nnoi,
                                                dt, tspan, ksteps, localgroupsize)

        if numpy.isnan(len(results)):
            raise RuntimeError("NaN present!")

        self.assertGreater(results.size, 0)
        
if __name__ == '__main__':
    unittest.main()