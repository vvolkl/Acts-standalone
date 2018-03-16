import matplotlib as mpl
import moritat.mock_track as m
import sys
from matplotlib.ticker import MultipleLocator 
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
import matplotlib.pyplot as plt

import numpy
from mpl_toolkits.mplot3d import proj3d
def orthogonal_proj(zfront, zback):
    a = (zfront+zback)/(zfront-zback)
    b = -2*(zfront*zback)/(zfront-zback)
    return numpy.array([[1,0,0,0],
                        [0,1,0,0],
                        [0,0,a,b],
                        [0,0,0,zback]])
proj3d.persp_transformation = orthogonal_proj

mpl.rcParams['legend.fontsize'] = 10

fig = plt.figure()
ax = fig.gca(projection='3d')
ax.grid(False)
ax.xaxis.pane.set_edgecolor('white')
ax.yaxis.pane.set_edgecolor('white')
ax.zaxis.pane.set_edgecolor('white')
ax.xaxis.pane.fill = False
ax.yaxis.pane.fill = False
ax.zaxis.pane.fill = False

layers = m.Layers(radii = np.arange(10,100, 20))
layers.draw_3D(ax=ax, phi1=0.75, phi2=1)
plt.xlim(0,100)
plt.ylim(0,100)
ax.set_zlim(0,100)
ax.xaxis._axinfo['tick']['inward_factor'] = 0
ax.xaxis._axinfo['tick']['outward_factor'] = 0.4
ax.yaxis._axinfo['tick']['inward_factor'] = 0
ax.yaxis._axinfo['tick']['outward_factor'] = 0.4
ax.zaxis._axinfo['tick']['inward_factor'] = 0
ax.zaxis._axinfo['tick']['outward_factor'] = 0.4
ax.zaxis._axinfo['tick']['outward_factor'] = 0.4
#ax.xaxis.set_major_locator(MultipleLocator(5))
#ax.yaxis.set_major_locator(MultipleLocator(5))
#ax.zaxis.set_major_locator(MultipleLocator(0.01))
[t.set_va('center') for t in ax.get_yticklabels()]
[t.set_ha('left') for t in ax.get_yticklabels()]
[t.set_va('center') for t in ax.get_xticklabels()]
[t.set_ha('right') for t in ax.get_xticklabels()]
[t.set_va('center') for t in ax.get_zticklabels()]
[t.set_ha('left') for t in ax.get_zticklabels()]

for f in sys.argv[1:]:
  pt = float(f.split("_")[1].split("GeV")[0])
  dat = np.loadtxt(f, delimiter=',')
  dat = dat[:, :3]
  plt.plot(*dat.T, label=pt)
plt.scatter([0], [0], zs=[0], s=15, marker='o', color="k")
plt.legend(title="pT [Gev]")
plt.xlabel("x [mm]")
plt.ylabel("y [mm]")
#ax.set_zlabel("z [mm]")
plt.savefig("track_cartoon.png")
