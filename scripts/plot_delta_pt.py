import numpy as np
import matplotlib.pyplot as plt
import sys
import os


label = "1 GeV"
dat = np.loadtxt(sys.argv[1])
eta = np.arctanh(dat[:,2] / np.sqrt(dat[:,0]**2 + dat[:,2]**2))
plt.figure("pT fit result")
plt.semilogy(eta, dat[:,1], 'o', label=label)
plt.figure("deltaPOverP")
OneOverP = 1. / np.sqrt(dat[:, 1]**2 + dat[:,-2]**2)
plt.semilogy(eta, dat[:,-1] / OneOverP, '+', label=label)
plt.figure("pT fit result")
plt.xlabel(r"$\eta$")
plt.ylabel(r"$p_T$ fit")
plt.legend(title=r"true $p_T$")
plt.title("FCChh TkLayout Option 3 v02 - Barrel Only - Prelim Full Reco ")
plt.savefig("PtTrue.pdf")
plt.savefig("PtTrue.png")
plt.figure("deltaPOverP")
plt.xlabel(r"$\eta$")
plt.ylabel(r"$\frac {\delta p} { p}$")
plt.legend(title=r"true $p_T$")
plt.title("FCChh TkLayout Option 3 v02 - Barrel Only - Prelim Full Reco ")
plt.savefig("deltaP.pdf")
plt.savefig("deltaP.png")

plt.show()
