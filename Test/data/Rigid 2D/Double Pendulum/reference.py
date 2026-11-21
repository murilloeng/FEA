#double pendulum with point masses (exact lagrange equations, angles from the downward vertical), integrated with scipy dop853 at tight tolerances
#output: t u1 v1 u2 v2 (displacements of the two masses) over five seconds
import numpy as np
from scipy.integrate import solve_ivp
g, m1, m2, l1, l2 = 9.81, 1.0, 1.0, 1.0, 1.0
q0 = np.array([+np.pi / 4, -np.pi / 4])
def system(t, y):
	q1, q2, w1, w2 = y
	d = q1 - q2
	M = np.array([[(m1 + m2) * l1 * l1, m2 * l1 * l2 * np.cos(d)], [m2 * l1 * l2 * np.cos(d), m2 * l2 * l2]])
	f = np.array([-m2 * l1 * l2 * np.sin(d) * w2 * w2 - (m1 + m2) * g * l1 * np.sin(q1), m2 * l1 * l2 * np.sin(d) * w1 * w1 - m2 * g * l2 * np.sin(q2)])
	return np.r_[w1, w2, np.linalg.solve(M, f)]
t = np.linspace(0, 5, 2001)
q1, q2 = solve_ivp(system, (0, 5), np.r_[q0, 0, 0], method='DOP853', t_eval=t, rtol=1e-12, atol=1e-12).y[:2]
x1, y1 = l1 * np.sin(q1), -l1 * np.cos(q1)
x2, y2 = x1 + l2 * np.sin(q2), y1 - l2 * np.cos(q2)
np.savetxt('Test/data/Rigid 2D/Double Pendulum/reference.dat', np.c_[t, x1 - x1[0], y1 - y1[0], x2 - x2[0], y2 - y2[0]], fmt='%+.6e')
