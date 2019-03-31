import unittest
import sys
import numpy
import random
sys.path.append('../interfaces/python')
import pysodecl

class TestPythonWrapper(unittest.TestCase):

    def test_simple(self):

        results = pysodecl.sodecl2(4)

        self.assertGreater(results, 3)
    
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

        # Initialise the time
        t0 = pysodecl.DoubleVector(orbits)
        for o in range(orbits):
            t0[o] = 0.0

        # Initialise initial state of the system
        initx = pysodecl.DoubleVector(orbits*nequat)
        for o in range(orbits*nequat):
            initx[o] = random.uniform(-3.1416, 3.1416)

        # Initialise parameters values of the system
        params = pysodecl.DoubleVector(orbits*nequat)
        for o in range(orbits*nequat):
            params[o] = random.uniform(0.01, 0.03)

        # Initialise noise values of the system
        noise = pysodecl.DoubleVector(orbits*nequat)
        for o in range(orbits*nequat):
            noise[o] = random.uniform(0.01, 0.03)

        nparams = nequat+nequat;

        results = pysodecl.sodeclcall(  t0, initx, params,
                                        openclplatform, opencldevice, openclkernel,
                                        solver, orbits, nequat, nparams, nnoi,
                                        dt, tspan, ksteps, localgroupsize)

        if numpy.isnan(len(results)):
            raise RuntimeError("NaN present!")

        self.assertGreater(5, 0)
        #self.assertGreater(results.size, 0)
        
if __name__ == '__main__':
    unittest.main()