#!/usr/bin/env python3
# Exact post-buckling (elastica) of Euler columns, from elliptic integrals.
# reference: Timoshenko & Gere, Theory of Elastic Stability, 2nd ed., sec. 2.7
#
# Cantilever of length l under an axial tip load P, alpha = tip rotation:
#   k = sin(alpha / 2), K = K(k), E = E(k) (complete elliptic integrals, modulus k)
#   P / Pcr = (2 K / pi)^2, Pcr = pi^2 EI / (4 l^2)
#   lateral tip deflection: dh / l = 2 k / K
#   tip shortening:         dv / l = 2 - 2 E / K
#
# Pinned-pinned (length L) is two cantilevers of length L / 2 and clamped-clamped
# is four of length L / 4, so with the same P / Pcr (Pcr = pi^2 EI / L^2 and
# 4 pi^2 EI / L^2) both have mid-span deflection k L / K and shortening L (2 - 2 E / K).
#
# usage: python3 reference.py  (writes reference.dat in this folder and in the
# Euler Pinned / Euler Clamped folders; columns: P / Pcr, deflection / L,
# shortening / L, rotation alpha in rad)

import os
import numpy as np
from scipy.special import ellipk, ellipe

here = os.path.dirname(os.path.abspath(__file__))

def elastica(alpha):
	k = np.sin(alpha / 2)
	K = ellipk(k * k)
	E = ellipe(k * k)
	return (2 * K / np.pi) ** 2, 2 * k / K, 2 - 2 * E / K

def write(path, scale_deflection):
	with open(path, "w") as file:
		# trivial branch
		for p in np.linspace(0, 1, 6)[:-1]:
			file.write("%.8e\t%.8e\t%.8e\t%.8e\n" % (p, 0, 0, 0))
		# post-buckled branch
		for alpha in np.linspace(0, np.radians(150), 31):
			p, dh, dv = elastica(alpha)
			file.write("%.8e\t%.8e\t%.8e\t%.8e\n" % (p, scale_deflection * dh, dv, alpha))

write(os.path.join(here, "reference.dat"), 1.0)
write(os.path.join(here, "..", "Euler Pinned", "reference.dat"), 0.5)
write(os.path.join(here, "..", "Euler Clamped", "reference.dat"), 0.5)
