import sys
import unittest
sys.path.append('../build/lib.linux-x86_64-2.7/hyperdual')
from numpy_hyperdual import hyperdual
import numpy as np
def func1(x):
    return x*x , 2.0*x, 2.0

def func2(x):
    return x*x*x , 3.0*x*x, 6.0*x

def complex_func(x):
    if x > 12:
        return x*x + np.cos(x*10) + np.exp(-x/10) + x/10.0
    else:
        return x*x + np.cos(x*10) + np.exp(-x/10)

import unittest


class TestStringMethods(unittest.TestCase):
    def test_1(self):
        x = 10.0
        xh = hyperdual(x, 1.0, 1.0, 0.0)
        for func in [func1, func2]:
            f = func(x)
            fh = func(xh)[0]
            self.assertEqual(f[0], fh.f0)
            self.assertEqual(f[1], fh.f1)
            self.assertEqual(f[1], fh.f2)
            self.assertEqual(f[2], fh.f12)

    def test_2(self):
        x = 10.0
        xh = hyperdual(x, 1.0, 1.0, 0.0)
        f = complex_func(x)
        h = 1e-6
        fd_1 = (complex_func(x + h) - complex_func(x - h))/2/h
        fd_2 = (complex_func(x + h) - complex_func(x)*2.0 + complex_func(x - h))/h/h
        fh = complex_func(xh)
        self.assertEqual(f, fh.f0)
        self.assertAlmostEqual(fd_1, fh.f1,3)
        self.assertAlmostEqual(fd_1, fh.f2,3)
        self.assertAlmostEqual(fd_2, fh.f12,-1)
        print 'finite diff: ', fd_1, fd_2
        print 'hyper dual: ', fh.f1, fh.f12

if __name__ == '__main__':
    unittest.main()
