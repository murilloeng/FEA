#exact large-amplitude pendulum released from horizontal: sin(phi / 2) = k * sn(K - w * t, k)
#output: t u1 u2 (tip displacements) over two periods
import numpy as np
from scipy.special import ellipj, ellipk
L, g = 1.0, 9.81
k = np.sin(np.pi / 4)
w = np.sqrt(g / L)
K = ellipk(k * k)
t = np.linspace(0, 2 * 4 * K / w, 2001)
phi = 2 * np.arcsin(k * ellipj(K - w * t, k * k)[0])
np.savetxt('Test/data/Rigid 2D/Single Pendulum/reference.dat', np.c_[t, L * np.sin(phi) - L, -L * np.cos(phi)], fmt='%+.6e')
