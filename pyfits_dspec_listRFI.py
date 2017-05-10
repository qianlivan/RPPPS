import numpy as np
import pyfits
import sys
import matplotlib.pyplot as plt
import time
from pylab import *


if (len(sys.argv)<2):
  print 'too few input parameters, format:'
  print 'python pyfits_dspec_listRFI.py filename'
  print 'example:'
  print 'python pyfits_dspec_listRFI.py FP160606_RFI2_0001.fits'
  sys.exit()

def listrfi(array,nu,n,txtout):
    length=len(array)
    derfi_array=np.zeros(length)
    outfile=file(txtout,'w')
    for index in np.arange(length):
        start=max([index-n,index])
        end=min([index+n,length-1])
        if(array[index]>2*np.median(array[start:end+1])):
          outfile.write("%d %f\n"%(index,nu[index]))
    outfile.close()
    return derfi_array

filename=sys.argv[1]
figname=filename[0:-5]+'_listRFI.png'
txtname=filename[0:-5]+'_listRFI.txt'
hdulist = pyfits.open(filename)

h0 = hdulist[0].header
h1 = hdulist[1].header
freq=h0['OBSFREQ']
nchan=h0['OBSNCHAN']
widthfreq=h0['OBSBW']

tsample=h1['TBIN']
nsblk=h1['NSBLK']

data=hdulist[1].data['DATA']

ra_sub= hdulist[1].data['RA_SUB']
nrow=len(ra_sub)


specs=np.zeros((nrow*nsblk,nchan))
specs_av=np.zeros((nrow,nchan))


for rowindex in range(nrow):
    print rowindex
    for subindex in range(nsblk):
	specs[rowindex*nsblk+subindex,:]=data[rowindex,subindex,0,:,0]
	specs_av[rowindex,:]=specs_av[rowindex,:]+specs[rowindex*nsblk+subindex,:]
    specs_av[rowindex,:]=specs_av[rowindex,:]/nsblk



ch=np.array(range(nchan))
nu=(ch*1.0/(nchan-1)-0.5)*widthfreq+freq

dspec=np.mean(specs_av,axis=0)

dspec_deRFI=listrfi(dspec,nu,7,txtname)



