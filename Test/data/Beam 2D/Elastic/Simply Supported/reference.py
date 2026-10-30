#!/usr/bin/env python3
# Exact large-deflection response of a simply supported beam (pin + roller,
# span L) with a central point load P.
# reference: Mattiasson, doi.org/10.1002/nme.1620170113
#
# By symmetry each half is a cantilever of length l = L / 2, clamped at
# mid-span, with a tip load P / 2 that stays vertical (the roller reaction has no
# horizontal component). The cantilever elastica
#   EI theta'' = -F cos(theta), theta(0) = 0, theta'(l) = 0
# is solved here by shooting on theta'(0) with tight tolerances (exact to plotting
# accuracy; it reproduces Test/data/Beam 2D/Elastic/Cantilever Force/reference.dat).
# With lambda = P L^2 / EI the cantilever load parameter is F l^2 / EI = lambda / 8.
#
#   mid-span deflection  v / L = (cantilever tip deflection) / 2
#   roller end slide     u / L = (cantilever tip shortening)  (both halves shorten by half of it)
#   end rotation         theta = cantilever tip rotation
#
# usage: python3 reference.py  (writes reference.dat; columns: lambda, v / L, u / L, theta)

import os
import numpy as np
from scipy.integrate import solve_ivp
from scipy.optimize import brentq

here = os.path.dirname(os.path.abspath(__file__))

def cantilever(q):
	# unit length, load parameter q = F l^2 / EI; returns tip deflection, shortening and rotation
	def rhs(s, y):
		return [y[1], -q * np.cos(y[0]), np.cos(y[0]), np.sin(y[0])]
	def shoot(c):
		return solve_ivp(rhs, [0, 1], [0, c, 0, 0], rtol=1e-12, atol=1e-14).y[:, -1]
	if q == 0:
		return 0.0, 0.0, 0.0
	c = brentq(lambda c: shoot(c)[1], 0, q, xtol=1e-14)
	t, _, x, y = shoot(c)
	return y, 1 - x, t

def check():
	data = np.loadtxt(os.path.join(here, "..", "Cantilever Force", "reference.dat"))
	error = 0
	for q, v, u, t in data:
		y, s, r = cantilever(q)
		error = max(error, abs(y + v), abs(s + u), abs(r + t))
	print("max deviation from Cantilever Force/reference.dat: %.2e" % error)

check()
with open(os.path.join(here, "reference.dat"), "w") as file:
	for p in np.linspace(0, 40, 41):
		y, s, r = cantilever(p / 8)
		file.write("%.8e\t%.8e\t%.8e\t%.8e\n" % (p, y / 2, s, r))
