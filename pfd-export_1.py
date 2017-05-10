import numpy as np
import sys, prepfold

if len(sys.argv) < 3:
    print("usage:pfd-export <input pfd file> <output csv file>")
    sys.exit(1)

pfd_file=sys.argv[1]
csv_file=sys.argv[2]
print("PFD file: "+pfd_file)
print("CSV file: "+csv_file)

pfd=prepfold.pfd(pfd_file)
print(pfd)
pfd.dedisperse(pfd.bestdm)
time_phase=pfd.time_vs_phase()
global_max = np.maximum.reduce(np.maximum.reduce(time_phase))
min_parts = np.minimum.reduce(time_phase, 1)
tp = (time_phase - min_parts[:, np.newaxis])/np.fabs(global_max)
sdm="{0:.3f}".format(pfd.bestdm)
np.savetxt(csv_file, tp, fmt='%0.6f', delimiter=" ")
print("time phase data exported.")
