import sys
import unittest
sys.path.append('../build/lib.linux-x86_64-2.7/hyperdual')
from numpy_hyperdual import hyperdual
import numpy as np
from pylab import *

x = np.linspace(0.0, 2*np.pi, 13)

f = np.sin(x)
dfdx = np.cos(x)
dfdx2 = -np.sin(x)

xh = np.zeros_like(x, dtype=hyperdual)
for i in range(len(x)):
    xh[i] = hyperdual(x[i], 1.0, 1.0, 0.0)

fh = np.sin(xh)

fhd_0 = np.zeros_like(x)
fhd_1 = np.zeros_like(x)
fhd_2 = np.zeros_like(x)
fhd_12 = np.zeros_like(x)

for i in range(len(x)):
    fhd_0[i] = fh[i].f0
    fhd_1[i] = fh[i].f1
    fhd_2[i] = fh[i].f2
    fhd_12[i] = fh[i].f12


figure()
subplot(311)
plot(x, f, 's', label='Function')
plot(x, fhd_0, 'x', label='HD Function')
ylabel('Function Values')
legend()
subplot(312)
plot(x, dfdx, 's', label='Exact')
plot(x, fhd_1, 'x',  label='HD 1')
plot(x, fhd_2, '.', label='HD 2')
ylabel('First Der.')
legend()
subplot(313)
plot(x, dfdx2, 's', label='Exact')
plot(x, fhd_12, 'x', label='HD 12')
legend()
ylabel('Second Der.')
show()

print 'Max error in first der', abs(dfdx-fhd_1).max()
print 'Max error in second der', abs(dfdx2-fhd_12).max()

