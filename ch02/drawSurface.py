# 倒山峰形
import numpy as np
from matplotlib import pyplot as plt
fig = plt.figure()
ax = plt.axes(projection="3d")
x = y = np.arange(start=-4, stop=4, step=0.1)
X, Y = np.meshgrid(x, y)
Z = X**2+Y**2
ax.plot_surface(X,Y,Z,alpha=0.9, cstride=1, rstride = 1, cmap='rainbow')
plt.show()
exit()


#峰值函数上半轴
import numpy as np
from matplotlib import pyplot as plt
fig = plt.figure()
ax = plt.axes(projection="3d")
x = y = np.arange(start=-4, stop=4, step=0.1)
X, Y = np.meshgrid(x, y)
Z = 3 * (1-X)**2 * np.exp(-X**2-(Y+1)**2)- 10 * (X/5- X**3 - Y**5)*np.exp(-X**2-Y**2)-1/3*np.exp(-(X+1)**2-Y**2)
Z[Z<0]=0
ax.plot_surface(X,Y,Z,alpha=0.9, cstride=1, rstride = 1, cmap='rainbow')
plt.show()
exit()

# 峰值函数全貌
import numpy as np
from matplotlib import pyplot as plt
fig = plt.figure()
ax = plt.axes(projection="3d")
x = y = np.arange(start=-4, stop=4, step=0.1)
X, Y = np.meshgrid(x, y)
Z = 3 * (1-X)**2 * np.exp(-X**2-(Y+1)**2)- 10 * (X/5- X**3 - Y**5)*np.exp(-X**2-Y**2)-1/3*np.exp(-(X+1)**2-Y**2)
ax.plot_surface(X,Y,Z,alpha=0.9, cstride=1, rstride = 1, cmap='rainbow')
plt.show()
exit()
