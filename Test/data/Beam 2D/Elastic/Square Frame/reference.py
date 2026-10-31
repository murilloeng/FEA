#!/usr/bin/env python3
# Exact large-deflection response of the square frame: side 2 L, rigid corners, pulled apart (tension)
# or pushed together (compression) by P at the mid-points of two opposite sides; inextensible
# elastica, no shear.
# reference: Mattiasson, doi.org/10.1002/nme.1620170113 (the published tables are in reference-*.dat)
#
# By symmetry a quarter is enough: from the loaded mid-point A (on the vertical axis, zero slope,
# vertical force P / 2, no horizontal force, unknown moment M_A) along the half side to the corner,
# where the tangent turns by -pi / 2 (rigid joint), and along the half side to the other mid-point B
# (on the horizontal axis, zero slope). Unit L and EI, member force lambda (the test applies
# 2 EI / L^2 per unit load factor at each mid-point), tangent angle phi:
#   phi' = -(M_A + s lambda x), x' = cos(phi), y' = sin(phi), phi(A) = 0, phi(B) = -pi / 2,
# s = -1 tension, +1 compression, with M_A found by shooting, following the load from zero.
#
#   column 1: displacement of the loaded mid-point along the load / L
#   column 2: displacement of the other mid-point (inward in tension, outward in compression) / L
#   column 3: corner rotation (sign as in the original tables)
#
# usage: python3 reference.py  (writes reference-tension.dat and reference-compression.dat;
# columns: lambda, then the three quantities above; agrees with the original published tables to
# within one unit of their fifth decimal)

import os
import numpy as np
from scipy.integrate import solve_ivp
from scipy.optimize import newton

here = os.path.dirname(os.path.abspath(__file__))

def quarter(q, s, m):
	rhs = lambda t, z: [-(m + s * q * z[1]), np.cos(z[0]), np.sin(z[0])]
	a = solve_ivp(rhs, [0, 1], [0, 0, 0], rtol=1e-12, atol=1e-14).y[:, -1]
	b = solve_ivp(rhs, [0, 1], [a[0] - np.pi / 2, a[1], a[2]], rtol=1e-12, atol=1e-14).y[:, -1]
	return b, a[0]

def frame(q, s):
	# continuation in the load keeps the root on the loading branch
	m = 0.0
	for qi in np.linspace(0, q, max(2, int(np.ceil(q / 0.05)) + 1))[1:]:
		m = newton(lambda z: quarter(qi, s, z)[0][0] + np.pi / 2, m, tol=1e-13, maxiter=100)
	(_, x, y), corner = quarter(q, s, m)
	return s * (y + 1), s * (x - 1), s * corner

loads = np.concatenate([np.arange(0.2, 2.01, 0.2), np.arange(2.5, 4.01, 0.5)])
for s, name in ((-1, "tension"), (+1, "compression")):
	with open(os.path.join(here, "reference-%s.dat" % name), "w") as file:
		for p in loads:
			file.write("%.8e\t%.8e\t%.8e\t%.8e\n" % ((p,) + frame(p, s)))
