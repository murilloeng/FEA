#!/usr/bin/env python3
# Exact large-deflection response of a circular ring (radius R, bending stiffness EI,
# inextensible, no shear) under two opposite point loads P, pushed in (compression) or pulled out (tension).
# reference: inextensible elastica of the ring, see e.g. Mattiasson, doi.org/10.1002/nme.1620170113,
# and Timoshenko & Gere (linear limit: diameter changes 2 (pi / 4 - 2 / pi) and 2 (2 / pi - 1 / 2) times P R^3 / EI)
#
# By symmetry only the quarter between the top point A (on the vertical axis) and the side point
# B (on the horizontal axis) is needed. Both cuts keep their initial slope, the horizontal force
# at A vanishes and the vertical force at A is P / 2. Going from A to B with arc length s, tangent
# angle phi (phi(A) = 0) and position (x, y) relative to A, the elastica is
#   phi' = -(1 / R + M / EI), M = M_A + s (P / 2) x, x' = cos(phi), y' = sin(phi)
# with s = +1 in compression and -1 in tension, and the unknown moment M_A is found by shooting so
# that phi(B) = -pi / 2. Displacements are written as magnitudes:
#   compression: loaded point inward v / R = 1 + y_B / R, side point outward u / R = x_B / R - 1
#   tension:     loaded point outward v / R = -(1 + y_B / R), side point inward u / R = 1 - x_B / R
#
# compression is limited to lambda = 6 (v / R = 0.74, before the loaded points would meet at the centre)
# usage: python3 reference.py  (writes reference-compression.dat and reference-tension.dat;
# columns: lambda = P R^2 / EI, v / R, u / R)

import os
import numpy as np
from scipy.integrate import solve_ivp
from scipy.optimize import brentq

here = os.path.dirname(os.path.abspath(__file__))

def ring(q, sign):
	# unit radius and stiffness, load parameter q = P R^2 / EI, sign = +1 (compression) or -1 (tension)
	def shoot(m):
		rhs = lambda s, y: [-(1 + m + sign * q / 2 * y[1]), np.cos(y[0]), np.sin(y[0])]
		return solve_ivp(rhs, [0, np.pi / 2], [0, 0, 0], rtol=1e-12, atol=1e-14).y[:, -1]
	if q == 0:
		return 0.0, 0.0
	m = brentq(lambda m: shoot(m)[0] + np.pi / 2, -q, q, xtol=1e-14)
	_, x, y = shoot(m)
	return sign * (1 + y), sign * (x - 1)

for sign, name, q_max in ((+1, "compression", 6), (-1, "tension", 20)):
	v, u = ring(1e-4, sign)
	print("%s, linear limit: v / lambda = %.6f (exact %.6f), u / lambda = %.6f (exact %.6f)" % (name, v / 1e-4, np.pi / 8 - 1 / np.pi, u / 1e-4, 1 / np.pi - 1 / 4))
	with open(os.path.join(here, "reference-%s.dat" % name), "w") as file:
		for q in np.linspace(0, q_max, 31):
			v, u = ring(q, sign)
			file.write("%.8e\t%.8e\t%.8e\n" % (q, v, u))
