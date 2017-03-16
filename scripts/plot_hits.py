import numpy as np
import matplotlib.pyplot as plt
import sys

filename = sys.argv[1]

alldat = np.loadtxt(filename)
pts = alldat[:,3]




for pt in np.unique(pts):
  dat = alldat[pts==pt]
  r = np.linalg.norm(dat[:,:2], axis=1)
  z = dat[:,2]
  rmax = np.amax(r)

  figures = []
  figures.append(plt.figure("x-y", figsize=(7,7)))

  # filter barrel hits
  x = dat[z < 400,0]
  y = dat[z < 400,1]
  plt.plot(x, y, 'o', color="red" if pt < 10 else "black")
  plt.xlabel("x [mm]")
  plt.ylabel("y [mm]")
  plt.xlim(0, 1.1*rmax)
  plt.ylim(0, -1.1*rmax)
  plt.axes().set_aspect('equal', 'datalim')

  figures.append(plt.figure("r-z"))
  plt.plot(z, r, 'o', alpha=0.6, color="red" if pt < 10 else "black")
  plt.xlabel("z [mm]")
  plt.ylabel("r [mm]")
  plt.xlim(0, np.amax(z)*1.1)
  plt.ylim(0, rmax*1.1)

  for eta in np.arange(0,5.1,0.2):
    if eta > 0:
      tantheta = np.tan( 2* np.arctan( np.exp(-1*eta)))
      if np.floor(eta) == eta:
        kwargs = {"ls": '--', "lw": 1 }
      else:
        kwargs = {"ls": '-', "lw":0.3}
      kwargs["color"] = "grey"
      kwargs["alpha"] = 0.5
      plt.plot([0,plt.xlim()[1]], [0, tantheta*plt.xlim()[1]], **kwargs)

  for f in figures:
    plt.title("Tracker Hits, pT = " + str(pt))


    print pt;
    print np.unique(dat[:,4], return_counts=True)

plt.show()
