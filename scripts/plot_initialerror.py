import numpy as np
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import sys
import os


#label = "1 GeV"
filename = sys.argv[1]
alldat = np.loadtxt(filename)
pts = alldat[:,0]
uncertainties = alldat[:, 9]
markers=['s', '+', 'd', '.', 'o', '*']


for i, pt in enumerate(np.unique(pts)):
  dat = alldat[pts==pt]
  eta = np.arctanh(dat[:,2] / np.sqrt(dat[:,0]**2 + dat[:,2]**2))
  plt.figure("pT fit result")
  plt.loglog(uncertainties, dat[:,1], 'o', label=pt)
  plt.legend(title=r"true $p_T$")
  plt.figure("deltaPOverP")
  OneOverP = 1. / np.sqrt(dat[:, 0]**2 + dat[:,2]**2)
  plt.xlabel(r"$\eta$")
  plt.ylabel(r"$\frac {\delta p} { p}$")
  plt.loglog(uncertainties, dat[:, 4] / OneOverP, marker='+', label=pt)
  plt.figure("pT fit result")
  plt.xlabel(r"$\eta$")
  plt.ylabel(r"$p_T$ fit")
  plt.legend(title=r"true $p_T$")
  plt.figure("cov Pt")
  plt.loglog(uncertainties, dat[:, 4], label=pt, alpha=0.4, marker=markers[i])
  plt.xlabel(r"$\eta$")
  plt.ylabel(r"$\delta p_T$")
  plt.legend(title=r"true $p_T$")

  plt.figure("deltaPt")
  plt.semilogy(eta, dat[:, 4], marker='+', label=pt)
  
plt.figure("pT fit result")
plt.legend(title=r"true $p_T$")
plt.savefig("PtTrue.png")
plt.figure("deltaPOverP")
plt.legend(title=r"true $p_T$")
plt.savefig(filename.replace(".dat", "_deltaP.png"))
plt.figure("cov Pt")
plt.savefig(filename.replace(".dat", "_covPt.png"))
plt.show()
