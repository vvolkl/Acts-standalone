import numpy as np
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import sys
import os


#label = "1 GeV"
filename = sys.argv[1]
alldat = np.loadtxt(filename)
pts = alldat[:,5]
uncertainties = alldat[:, 8]
markers=['s', '+', 'd', '.', 'o', '*']


for i, pt in enumerate(np.unique(pts)):
  print pt
  dat = alldat[pts==pt]
  uncertainties = dat[:, 8]
  eta = np.arctanh(dat[:,2] / np.sqrt(dat[:,0]**2 + dat[:,2]**2))
  plt.figure("pT fit result")
  plt.loglog(uncertainties, dat[:,1], 'o', label=pt)
  plt.legend(title=r"true $p_T$")
  plt.figure("deltaPOverP")
  OneOverP = 1. / np.sqrt(dat[:, 0]**2 + dat[:,2]**2)
  plt.xlabel(r"$\eta$")
  plt.ylabel(r"$\frac {\delta p} { p}$")
  plt.loglog(uncertainties, dat[:, 4] / OneOverP, '+', label=pt)
  plt.figure("pT fit result")
  plt.xlabel(r"$\eta$")
  plt.ylabel(r"$p_T$ fit")
  plt.legend(title=r"true $p_T$")
  plt.figure("cov Pt")
  plt.loglog(uncertainties, dat[:, 4], markers[i], label=pt, alpha=0.4)
  plt.xlabel(r"initial track parameter uncertainty")
  plt.ylabel(r"$\delta p_T$")
  plt.legend(title=r"measurement uncertainty")

  plt.figure("deltaPt")
  plt.semilogy(eta, dat[:, 4], '+', label=pt)
  
plt.figure("pT fit result")
plt.legend(title=r"true $p_T$")
plt.savefig("PtTrue.png")
plt.figure("deltaPOverP")
plt.legend(title=r"true $p_T$")
plt.savefig(filename.replace(".dat", "_deltaP.png"))
plt.figure("cov Pt")
plt.savefig(filename.replace(".dat", "_covPt.png"))
plt.show()
