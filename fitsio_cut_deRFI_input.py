import numpy as np
import fitsio
import sys
import matplotlib.pyplot as plt
import time
from pylab import *
import os


if (len(sys.argv)<2):
  print 'too few input parameters, format:'
  print 'python fitsio_cut_deRFI_intpu.py fitsfilename txtfilename'
  print 'example:'
  print 'python fitsio_cut_deRFI_intpu.py FP160606_RFI2_0001.fits FP160606_RFI2_0001_listRFI.txt'
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


command='rm -f '+outname
os.system(command)

#data2=fits[1][0]
data2=fits[1][:]
fitsout=fitsio.FITS(outname,'rw')
#fitsout.write(data2,header=h0)


ch=np.array(range(nchan))
nu=(ch*1.0/(nchan-1)-0.5)*widthfreq+freq



indexarraystr = np.loadtxt(txtfilename,unpack=True,usecols=[0])
indexarray = np.zeros(len(indexarraystr))
indexarray = map(int,indexarraystr)
print indexarray

for rowindex in range(nrow):
    data = fits[1].read(rows=[rowindex], columns=['DATA'])
    print rowindex
    for subindex in range(nsblk):
	specs[rowindex*nsblk+subindex,:]=data[0][0][subindex,0,:,0]
	specs_av[rowindex,:]=specs_av[rowindex,:]+specs[rowindex*nsblk+subindex,:]
    specs_av[rowindex,:]=specs_av[rowindex,:]/nsblk

dspec=np.mean(specs_av,axis=0)
dspec_deRFI=derfi(dspec,7)

for rowindex in range(nrow):
    data = fits[1].read(rows=[rowindex], columns=['DATA'])
    print rowindex
    for subindex in range(nsblk):
	tempspec=data[0][0][subindex,0,:,0]
        tempspec[indexarray]=dspec_deRFI[indexarray]
        data2['DATA'][rowindex][subindex,0,:,0]=tempspec

#header={'OBSFREQ':freq}
fitsout.write(data2,header=h0)
fitsout[0].write_key('OBSFREQ', freq, comment="")
fitsout[0].write_key('OBSNCHAN', nchan, comment="")
fitsout[0].write_key('OBSBW', widthfreq, comment="")
fitsout[1].write_key('TBIN', tsample, comment="")
fitsout[1].write_key('NSBLK', nsblk, comment="")
fitsout.close()
