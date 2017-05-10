import sys
import numpy as np
import matplotlib.pyplot as plt

if len(sys.argv) < 2:
    print("usage:show-csv <file name>")
    sys.exit(1)
    
tp=np.genfromtxt(sys.argv[1])
tp=1.0-tp
tp2=np.concatenate((tp,tp),axis=1)
plt.imshow(tp2, cmap='gray')
plt.show()
