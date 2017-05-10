import numpy as np
import pyfits
import sys
import matplotlib.pyplot as plt
import time
from pylab import *
import os


if (len(sys.argv)<2):
  print 'too few input parameters, format:'
  print 'python fitsio_cut_deRFI_intput.py fitsfilename txtfilename'
  print 'example:'
  print 'python fitsio_cut_deRFI_intput.py FP160606_RFI2_0001.fits FP160606_RFI2_0001_listRFI.txt'
  sys.exit()

def derfi(array,n):
    length=len(array)
    derfi_array=np.zeros(length)
    for index in np.arange(length):
        start=max([index-n,index])
        end=min([index+n,length-1])
        derfi_array[index]=np.median(array[start:end+1])
    return derfi_array


filename=sys.argv[1]
txtfilename=sys.argv[2]
figname=filename[0:-5]+'_cut_deRFI.png'
outname=filename[0:-5]+'_cut_deRFI.fits'
hdulist = pyfits.open(filename)



h0 = hdulist[0].header
h1 = hdulist[1].header
freq=h0['OBSFREQ']
nchan=h0['OBSNCHAN']
widthfreq=h0['OBSBW']

tsample=h1['TBIN']
nsblk=h1['NSBLK']

ra_sub= hdulist[1].data['RA_SUB']
nrow=len(ra_sub)


specs=np.zeros((nrow*nsblk,nchan))
specs_av=np.zeros((nrow,nchan))




ch=np.array(range(nchan))
nu=(ch*1.0/(nchan-1)-0.5)*widthfreq+freq

data=hdulist[1].data['DATA']


indexarraystr = np.loadtxt(txtfilename,unpack=True,usecols=[0])
indexarray = np.zeros(len(indexarraystr))
indexarray = map(int,indexarraystr)
print indexarray

for rowindex in range(nrow):
    print rowindex
    for subindex in range(nsblk):
	specs[rowindex*nsblk+subindex,:]=data[rowindex,subindex,0,:,0]
	specs_av[rowindex,:]=specs_av[rowindex,:]+specs[rowindex*nsblk+subindex,:]
    specs_av[rowindex,:]=specs_av[rowindex,:]/nsblk

dspec=np.mean(specs_av,axis=0)
dspec_deRFI=derfi(dspec,7)

for rowindex in range(nrow):
    print rowindex
    for subindex in range(nsblk):
	tempspec=data[rowindex,subindex,0,:,0]
        tempspec[indexarray]=dspec_deRFI[indexarray]
        data[rowindex,subindex,0,:,0]=tempspec

hdulist[1].data['DATA']=data
command='rm -f '+outname
os.system(command)
hdulist.writeto(outname)
