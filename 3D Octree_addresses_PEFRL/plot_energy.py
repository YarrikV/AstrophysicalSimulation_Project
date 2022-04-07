import matplotlib.pyplot as plt
import numpy as np
# ~ from mpl_toolkits.mplot3d import Axes3D

t,E = np.genfromtxt("3body_stable_PEFRL_EnergyError.txt", skip_header=0, unpack=True)
# ~ t1,E1 = np.genfromtxt("3-body_eps=0.030000_h=0.001000_EnergyError.txt", skip_header=0, unpack=True)
# ~ t2,E2 = np.genfromtxt("3-body_eps=0.030000_h=0.000100_EnergyError.txt", skip_header=0, unpack=True)

y=np.zeros((6))
for i in range(6):
	y[i]= 10**(-8-i)

fig1=plt.figure()
fig1.subplots_adjust(bottom=0.2,left=0.2)
ax1 = fig1.add_subplot(111)
# ~ ax1 = Axes3D(fig1)
ax1.set_yscale('log')
ax1.set_xlabel(r'$t$ (programma eenheden)')
ax1.set_ylabel(r'$\delta E/E$')
ax1.set_ylim(10**(-13),10**(-9))
ax1.grid()
ax1.set_yticks(y)
ax1.plot(t,E,label= r'$h=0.01$')
# ~ ax1.plot(t1,E1,label= r'$h=0.001$')
# ~ ax1.plot(t2,E2,label= r'$h=0.0001$')
ax1.legend(loc='upper center', bbox_to_anchor=(0.8,0.83))
plt.title("Gebonden 3-body systeem (PEFRL) ($\epsilon=0$)")
plt.show()

# ~ fig2=plt.figure()
# ~ fig2.subplots_adjust(bottom=0.2,left=0.2)
# ~ ax2 = Axes3D(fig2)
# ~ ax2.scatter(vx, vy, vz, c='r', s=1)
# ~ ax2.set_xlabel(r'$v_x$')
# ~ ax2.set_ylabel(r'$v_y$')
# ~ ax2.set_zlabel(r'$v_z$')
#plt.show()
