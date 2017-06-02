import numpy as np
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import sys
import os


#label = "1 GeV"
alldat = np.loadtxt(sys.argv[1])
pts = alldat[:,0]



for pt in np.unique(pts):
  dat = alldat[pts==pt]
  eta = np.arctanh(dat[:,2] / np.sqrt(dat[:,0]**2 + dat[:,2]**2))
  plt.figure("pT fit result")
  plt.semilogy(eta, dat[:,1], 'o', label=pt)
  plt.legend(title=r"true $p_T$")
  plt.figure("deltaPOverP")
  OneOverP = 1. / np.sqrt(dat[:, 0]**2 + dat[:,2]**2)
  plt.xlabel(r"$\eta$")
  plt.ylabel(r"$\frac {\delta p} { p}$")
  plt.semilogy(eta, dat[:, 4] / OneOverP, '+', label=pt)
  plt.figure("pT fit result")
  plt.xlabel(r"$\eta$")
  plt.ylabel(r"$p_T$ fit")
  plt.legend(title=r"true $p_T$")
  plt.figure("cov Pt")
  plt.semilogy(eta, dat[:, 4], 'd', label=pt)
  plt.xlabel(r"$\eta$")
  plt.ylabel(r"cov pt")
  plt.legend(title=r"true $p_T$")

#plt.title("FCChh TkLayout Option 3 v02 - Barrel Only - Prelim Full Reco ")
  #plt.savefig("PtTrue.pdf")
  #plt.savefig("PtTrue.png")
#plt.legend(title=r"true $p_T$")
 
#plt.title("FCChh TkLayout Option 3 v02 - Barrel Only - Prelim Full Reco ")
  #plt.savefig("deltaP.pdf")
  #plt.savefig("deltaP.png")
  plt.figure("deltaPt")
  plt.semilogy(eta, dat[:, 4], '+', label=pt)
  
plt.figure("pT fit result")
plt.legend(title=r"true $p_T$")
plt.savefig("PtTrue.png")
plt.figure("deltaPOverP")
plt.legend(title=r"true $p_T$")
plt.savefig("deltaP.png")
plt.figure("cov Pt")
plt.savefig("cov_pt.png")
plt.show()
