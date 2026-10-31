#!/usr/bin/env python3
# Exact large-deflection response of a cantilever (length L, bending stiffness EI, inextensible,
# no shear) under a tip force P that keeps its direction (perpendicular to the initial axis).
# reference: Mattiasson, doi.org/10.1002/nme.1620170113 (the published table is in reference.dat)
#
# The elastica EI theta'' = -P cos(theta), theta(0) = 0, theta'(L) = 0, x' = cos(theta), y' = sin(theta)
# is solved by shooting on theta'(0) with tight tolerances. With lambda = P L^2 / EI:
#   tip deflection v / L = -y(L) / L, tip shortening u / L = -(1 - x(L) / L), tip rotation theta = -theta(L)
# (signs as in reference.dat: the force points down).
#
# usage: python3 reference.py  (writes reference.dat; columns: lambda, v / L, u / L, theta;
# it agrees with the original published table to within one unit of its fifth decimal)

import os
import numpy as np
from scipy.integrate import solve_ivp
from scipy.optimize import brentq

here = os.path.dirname(os.path.abspath(__file__))

def cantilever(q):
	# unit length, load parameter q = P L^2 / EI; returns tip deflection, shortening and rotation
	def rhs(s, y):
		return [y[1], -q * np.cos(y[0]), np.cos(y[0]), np.sin(y[0])]
	def shoot(c):
		return solve_ivp(rhs, [0, 1], [0, c, 0, 0], rtol=1e-12, atol=1e-14).y[:, -1]
	if q == 0:
		return 0.0, 0.0, 0.0
	c = brentq(lambda c: shoot(c)[1], 0, q, xtol=1e-14)
	t, _, x, y = shoot(c)
	return y, 1 - x, t

loads = np.concatenate([np.arange(0.2, 2.01, 0.2), np.arange(2.5, 10.01, 0.5)])
with open(os.path.join(here, "reference.dat"), "w") as file:
	for p in loads:
		v, u, t = cantilever(p)
		file.write("%.8e\t%.8e\t%.8e\t%.8e\n" % (p, -v, -u, -t))
