import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits.mplot3d import Axes3D

x,y,z,x1,y1,z1,x2,y2,z2 = np.genfromtxt("3body_stable_PEFRL_Orbit.txt", skip_header=0, unpack=True)

# ~ m_max = m.max()
# ~ v=1/m_max
# ~ m*=v

fig1=plt.figure()
fig1.subplots_adjust(bottom=0.2,left=0.2)
# ~ ax1 = fig1.add_subplot(111, projection='3d')
ax1 = Axes3D(fig1)
ax1.plot(x,y,z,linewidth=1)
ax1.plot(x1,y1,z1,linewidth=1)
ax1.plot(x2,y2,z2,linewidth=1)
ax1.set_xlabel(r'x')
ax1.set_ylabel(r'y')
ax1.set_zlabel(r'z')
plt.show()

# ~ fig2=plt.figure()
# ~ fig2.subplots_adjust(bottom=0.2,left=0.2)
# ~ ax2 = Axes3D(fig2)
# ~ ax2.scatter(vx, vy, vz, c='r', s=1)
# ~ ax2.set_xlabel(r'$v_x$')
# ~ ax2.set_ylabel(r'$v_y$')
# ~ ax2.set_zlabel(r'$v_z$')
#plt.show()
