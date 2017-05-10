import numpy as np
import fitsio
import sys
import matplotlib.pyplot as plt
import time
from pylab import *


if (len(sys.argv)<2):
  print 'too few input parameters, format:'
  print 'python fitsio_dspec_listRFI.py filename'
  print 'example:'
  print 'python fitsio_dspec_listRFI.py FP160606_RFI2_0001.fits'
  sys.exit()

def listrfi(array,nu,n,txtout):
    length=len(array)
    derfi_array=np.zeros(length)
    outfile=file(txtout,'w')
    for index in np.arange(length):
        start=max([index-n,index])
        end=min([index+n,length-1])
        if(array[index]>2*np.median(array[start:end+1])):
          #outstr=str(index)+'    '+str(nu[index])+'\n'
          #outfile.write(outstr)
          #print ("%f %f"%(index,nu[index]))
          outfile.write("%d %f\n"%(index,nu[index]))
    outfile.close()
    return derfi_array

filename=sys.argv[1]
figname=filename[0:-5]+'_listRFI.png'
txtname=filename[0:-5]+'_listRFI.txt'
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
	specs_av[rowindex,:]=specs_av[rowindex,:]+specs[rowindex*nsblk+subindex,:]
    specs_av[rowindex,:]=specs_av[rowindex,:]/nsblk



ch=np.array(range(nchan))
nu=(ch*1.0/(nchan-1)-0.5)*widthfreq+freq

dspec=np.mean(specs_av,axis=0)

dspec_deRFI=listrfi(dspec,nu,7,txtname)



