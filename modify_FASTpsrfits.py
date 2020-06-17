import types
import numpy as np 
import pyfits
import os
import datetime
import time
import sys
from array import array
import matplotlib as mpl
import matplotlib.pyplot as plt
from pylab import *

#mpl.rcParams['image.interpolation']='none'
if (len(sys.argv)<3):
  print 'too few inputs!'
  print 'example:'
  print 'python modify_FASTpsrfits.py FAST.fits keyfile'
  #print 'python modify_FASTpsrfits.py FAST.fits KEY1 value1'
  #print 'python cut_FASTpsrfits.py FAST.fits'
  sys.exit()

starttime=datetime.datetime.now()

filename=sys.argv[1]
#key=sys.argv[2]
keyfile=sys.argv[2]



#u19700101=62135683200.0

hdulist = pyfits.open(filename)

hdu0 = hdulist[0]
data0 = hdu0.data
header0 = hdu0.header
#print data0

hdu1 = hdulist[1]
data1 = hdu1.data
header1 = hdu1.header
#print header1


for line in open(keyfile):
    #key=line.replace('\n','').split(",")[0].strip()
    #tempvalue=line.replace('\n','').split(",")[1].strip()
    line1=" ".join(line.split())
    key=line1.replace('\n','').split(" ")[0].strip()
    tempvalue=line1.replace('\n','').split(" ")[1].strip()

    print type(hdu0.header[key])

    if isinstance(hdu0.header[key],int):
       hdu0.header[key]=int(tempvalue)
    if isinstance(hdu0.header[key],float):
       hdu0.header[key]=float(tempvalue)
    if isinstance(hdu0.header[key],str):
       hdu0.header[key]=tempvalue

    print key,tempvalue,hdu0.header[key]




hdulist2 = pyfits.HDUList([hdu0,hdu1])
os.system('rm -f FASTpsrfits_out.fits')
hdulist2.writeto('FASTpsrfits_out.fits')



print '--------------------------------------------'
print '             Finished!                      '


endtime=datetime.datetime.now()
print 'START:',starttime
print 'END:',endtime
duration=endtime-starttime
print 'DURATION:',duration.seconds,' sec'




