#!/usr/bin/env python3
# Exact large-deflection response of the diamond (rhombus) frame: four members of length L at 45
# degrees, rigid side corners, hinged loaded corners, pulled apart (tension) or pushed together
# (compression) by P at the hinges; inextensible elastica, no shear.
# reference: Mattiasson, doi.org/10.1002/nme.1620170113 (the published tables are in reference-*.dat)
#
# By symmetry one member is enough: from the hinge A (on the vertical axis, zero moment, vertical
# force P / 2, no horizontal force) to the side corner B (on the horizontal axis), whose rotation is
# zero. Going from A to B with tangent angle phi (unit L and EI, lambda = P L^2 / (2 EI) per the test's
# load 2 EI / L^2 per unit load factor, so the member force is lambda):
#   phi' = -s lambda x, x' = cos(phi), y' = sin(phi), phi(B) = -pi / 4, s = -1 tension, +1 compression
# and the unknown slope phi(A) is found by shooting, following the load from zero.
#
#   column 1: displacement of the hinge along the load (outward in tension, inward in compression) / L
#   column 2: displacement of the side corner (inward in tension, outward in compression) / L
#   column 3: pi / 4 + rotation of the member end at the hinge (absolute angle, as in the original tables)
#
# usage: python3 reference.py  (writes reference-tension.dat and reference-compression.dat;
# columns: lambda, then the three quantities above; agrees with the original published tables to
# within one unit of their fifth decimal)

import os
import numpy as np
from scipy.integrate import solve_ivp
from scipy.optimize import newton

here = os.path.dirname(os.path.abspath(__file__))
h = np.sqrt(0.5)

def member(q, s, phi0):
	rhs = lambda t, z: [-s * q * z[1], np.cos(z[0]), np.sin(z[0])]
	return solve_ivp(rhs, [0, 1], [phi0, 0, 0], rtol=1e-12, atol=1e-14).y[:, -1]

def frame(q, s):
	# continuation in the load keeps the root on the loading branch
	phi = -np.pi / 4
	for qi in np.linspace(0, q, max(2, int(np.ceil(q / 0.05)) + 1))[1:]:
		phi = newton(lambda p: member(qi, s, p)[0] + np.pi / 4, phi, tol=1e-13, maxiter=100)
	_, x, y = member(q, s, phi)
	return s * (y + h), s * (x - h), np.pi / 4 - (phi + np.pi / 4)

loads = np.concatenate([np.arange(0.2, 2.01, 0.2), np.arange(2.5, 10.01, 0.5)])
for s, name in ((-1, "tension"), (+1, "compression")):
	with open(os.path.join(here, "reference-%s.dat" % name), "w") as file:
		for p in loads:
			file.write("%.8e\t%.8e\t%.8e\t%.8e\n" % ((p,) + frame(p, s)))
