import numpy as np
import fitsio
import sys
import matplotlib.pyplot as plt
import time
from pylab import *
import os


if (len(sys.argv)<4):
  print 'too few input parameters!'
  print 'example:'
  print 'python fitsio_cut_freq_time.py startchan endchan startn endn FAST.fits'
  sys.exit()


startfreq=int(sys.argv[1])
endfreq=int(sys.argv[2])
startn=int(sys.argv[3])
endn=int(sys.argv[4])
filename=sys.argv[5]
outname=filename[0:-5]+'_cut_deRFI.fits'
fits=fitsio.FITS(filename)


chnum=endfreq-startfreq+1
linenum=endn-startn+1
nrow=linenum



h0 = fits[0].read_header()
h1 = fits[1].read_header()
freq=h0['OBSFREQ']
nchan_origin=h0['OBSNCHAN']
widthfreq=h0['OBSBW']

tsample=h1['TBIN']
nsblk=h1['NSBLK']
npol=h1['NPOL']

nchan=chnum


specs=np.zeros((nrow*nsblk,nchan))
specs_av=np.zeros((nrow,nchan))


command='rm -f '+outname
os.system(command)



data2=fits[1][:]
data3 = np.zeros(nrow,dtype=[('TSUBINT','float64'),('OFFS_SUB','float64'),('LST_SUB','float64'),('RA_SUB','float64'),('DEC_SUB','float64'),('GLON_SUB','float64'),('GLAT_SUB','float64'),('FD_ANG','float32'),('POS_ANG','float32'),('PAR_ANG','float32'),('TEL_AZ','float32'),('TEL_ZEN','float32'),('DAT_FREQ','float32',(nchan)),('DAT_WTS','float32',(nchan)),('DAT_OFFS','float32',(2*nchan)),('DAT_SCL','float32',(2*nchan)),('DATA','uint8',(nsblk,npol,nchan,1))])
#data3['TSUBINT']=data2['TSUBINT']
#data3 = np.zeros(nrow,dtype=[('TSUBINT','float64'),('OFFS_SUB','float64'),('LST_SUB','float64'),('RA_SUB','float64'),('DEC_SUB','float64'),('GLON_SUB','float64'),('GLAT_SUB','float64')])

fitsout=fitsio.FITS(outname,'rw')




ch=np.array(range(nchan))
nu=(ch*1.0/(nchan-1)-0.5)*widthfreq+freq




for index in range(nrow):
    rowindex=index+startn
    data = fits[1].read(rows=[rowindex], columns=['DATA'])
    print index+startn
    for subindex in range(nsblk):
	tempspec1=data[0][0][subindex,0,:,0]
	tempspec2=data[0][0][subindex,1,:,0]
        #data2['DATA'][rowindex][subindex,0,:,0]=tempspec1
        data3['TSUBINT'][index]=fits[1].read(rows=[rowindex], columns=['TSUBINT'])[0][0]
        data3['OFFS_SUB'][index]=fits[1].read(rows=[rowindex], columns=['OFFS_SUB'])[0][0]
        data3['LST_SUB'][index]=fits[1].read(rows=[rowindex], columns=['LST_SUB'])[0][0]
        data3['RA_SUB'][index]=fits[1].read(rows=[rowindex], columns=['RA_SUB'])[0][0]
        data3['DEC_SUB'][index]=fits[1].read(rows=[rowindex], columns=['DEC_SUB'])[0][0]
        data3['GLON_SUB'][index]=fits[1].read(rows=[rowindex], columns=['GLON_SUB'])[0][0]
        data3['GLAT_SUB'][index]=fits[1].read(rows=[rowindex], columns=['GLAT_SUB'])[0][0]
        data3['FD_ANG'][index]=fits[1].read(rows=[rowindex], columns=['FD_ANG'])[0][0]
        data3['POS_ANG'][index]=fits[1].read(rows=[rowindex], columns=['POS_ANG'])[0][0]
        data3['PAR_ANG'][index]=fits[1].read(rows=[rowindex], columns=['PAR_ANG'])[0][0]
        data3['TEL_AZ'][index]=fits[1].read(rows=[rowindex], columns=['TEL_AZ'])[0][0]
        data3['TEL_ZEN'][index]=fits[1].read(rows=[rowindex], columns=['TEL_ZEN'])[0][0]
        data3['DAT_FREQ'][index]=fits[1].read(rows=[rowindex], columns=['DAT_FREQ'])[0][0][startfreq:endfreq+1]
        data3['DAT_WTS'][index]=fits[1].read(rows=[rowindex], columns=['DAT_WTS'])[0][0][startfreq:endfreq+1]
        
        data3['DAT_OFFS'][index][0:nchan]=fits[1].read(rows=[rowindex], columns=['DAT_OFFS'])[0][0][startfreq:endfreq+1]
        data3['DAT_OFFS'][index][nchan:2*nchan]=fits[1].read(rows=[rowindex], columns=['DAT_OFFS'])[0][0][startfreq+nchan_origin:endfreq+1+nchan_origin]
        data3['DAT_SCL'][index][0:nchan]=fits[1].read(rows=[rowindex], columns=['DAT_SCL'])[0][0][startfreq:endfreq+1]
        data3['DAT_SCL'][index][nchan:2*nchan]=fits[1].read(rows=[rowindex], columns=['DAT_SCL'])[0][0][startfreq+nchan_origin:endfreq+1+nchan_origin]
        data3['DATA'][index][subindex,0,:,0]=tempspec1[startfreq:endfreq+1]
        data3['DATA'][index][subindex,1,:,0]=tempspec2[startfreq:endfreq+1]
#header={'OBSFREQ':freq}
#fitsout.write(data2,header=h0)
#h0['TFORM1']='1D'
h0['OBSFREQ']=((startfreq+endfreq)*1.0/2+1.0)/((nchan-1.0)*1.0/2+1.0)*freq
h0['OBSBW']=chnum*1.0
h0['OBSNCHAN']=chnum
fitsout.write(data3,header=h0)
fitsout[0].write_key('OBSFREQ', freq, comment="")
fitsout[0].write_key('OBSNCHAN', nchan, comment="")
fitsout[0].write_key('OBSBW', widthfreq, comment="")
fitsout[1].write_key('TBIN', tsample, comment="")
fitsout[1].write_key('NSBLK', nsblk, comment="")
fitsout.close()
