import matplotlib.pyplot as plt
import numpy as np

from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import numpy as np

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

import sys
dat = np.loadtxt(sys.argv[1])
ax.scatter(*dat[:, :3].T)
plt.show()
