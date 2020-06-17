import ephem
import numpy as np
import time
import sys


if (len(sys.argv)<3):
  print 'too few input parameters, format:'
  print 'python radec2glonglat ra dec'
  print '                     (hours) (degrees)'
  print 'example:'
  print 'python radec2glonglat 20.0 20.0'
  sys.exit()


ra=sys.argv[1]
ra0=float(ra)/15.0
#print ra0
ra0=ephem.hours(ra0)
#ra0=ephem.hours(sys.argv[1])
dec0=ephem.degrees(sys.argv[2])

eb=ephem.Equatorial(ra0,dec0,epoch=ephem.J2000)
gb=ephem.Galactic(eb)
#print gb.lon, gb.lat
gl=float(ephem.degrees(gb.lon))*180.0/np.pi
gb=float(ephem.degrees(gb.lat))*180.0/np.pi
#print '%.2f %.2f' % (float(ephem.degrees(gb.lon))*180.0/np.pi,  float(ephem.degrees(gb.lat))*180.0/np.pi)
if(gb >= 0):  print 'G%05.1fp%04.1f' % (gl, gb)
if(gb < 0):
  gb=gb*(-1.0)
  print 'G%05.1fm%04.1f' % (gl, gb)
