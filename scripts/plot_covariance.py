import numpy as np
import matplotlib.pyplot as plt
import sys
import os

logfiles = sys.argv[1:]
for logfile in logfiles:
  print logfile
  os.system("grep covariance " + logfile +  " > tmp.txt" )
  os.system("sed -i -- 's/covariance//g' tmp.txt")


  dat = np.loadtxt("tmp.txt")
  np.set_printoptions(linewidth=120)
  diag = np.eye(5).reshape(25)
  diag_indices = np.nonzero(diag)[0]

  print dat[0].reshape(5,5)
  print dat[0, diag_indices]
  plt.figure("pT fit result")
  plt.semilogy(np.abs(dat[:, diag_indices]), "o")

plt.show()
