import numpy as np
import fitsio
import sys
import matplotlib.pyplot as plt
import time
from pylab import *


if (len(sys.argv)<2):
  print 'too few input parameters, format:'
  print 'python fitsio_dspec.py filename'
  print 'example:'
  print 'python fitsio_dspec.py FP160606_RFI2_0001.fits'
  sys.exit()

filename=sys.argv[1]
figname=filename[0:-5]+'.png'
fits=fitsio.FITS(filename)

h0 = fits[0].read_header()
h1 = fits[1].read_header()
freq=h0['OBSFREQ']
nchan=h0['OBSNCHAN']
widthfreq=h0['OBSBW']

tsample=h1['TBIN']
nsblk=h1['NSBLK']

ra_sub= fits[1].read(columns=['RA_SUB'])
nrow=len(ra_sub)


specs=np.zeros((nrow*nsblk,nchan))
specs_av=np.zeros((nrow,nchan))

for rowindex in range(nrow):
    data = fits[1].read(rows=[rowindex], columns=['DATA'])
    #print rowindex,np.size(data[0][0][:,0,0,0]),np.size(data[0][0][0,:,0,0]),np.size(data[0][0][0,0,:,0]),np.size(data[0][0][0,0,0,:])
    print rowindex
    for subindex in range(nsblk):
	specs[rowindex*nsblk+subindex,:]=data[0][0][subindex,0,:,0]
        #print data[0][0][subindex,0,:,0]
	specs_av[rowindex,:]=specs_av[rowindex,:]+specs[rowindex*nsblk+subindex,:]
    specs_av[rowindex,:]=specs_av[rowindex,:]/nsblk



#x=np.array(range(1000))*nsblk*tsample # seconds
ch=np.array(range(nchan))
nu=(ch*1.0/(nchan-1)-0.5)*widthfreq+freq

#np.save("FAST_dspec.npy",continuum_av)

fig=plt.figure(figsize=(10,10))


font = {'family' : 'serif',  
        'color'  : 'black',  
        'weight' : 'normal',  
        'size'   : 16,  
        }  

ax=fig.add_subplot(111)
dspec=np.mean(specs_av,axis=0)
#ax.plot(ch,dspec)
ax.plot(nu,dspec)

ylabel('Intensity',fontdict=font)
#xlabel('Channel',fontdict=font)
xlabel('Frequency (MHz)',fontdict=font)

savefig(figname)
plt.show()


