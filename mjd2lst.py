import ephem
import numpy as np
import time
import sys

if (len(sys.argv)<2):
  print 'too few input parameters, format:'
  print 'python jd2date.py jd'
  print 'example:'
  print 'python jd2date.py 20.0'
  sys.exit()


jd=float(sys.argv[1])+2400000.5
date=ephem.julian_date('1899/12/31 12:00:00')
djd=jd-date

#print ephem.Date(djd)

Dawodang=ephem.Observer()
Dawodang.lat=str(25.6529518158)  # Official parameter
Dawodang.lon=str(106.856666872)  # Official parameter
Dawodang.horizon='-18' # Astronomical twilight uses the value -18 degrees
Dawodang.date=ephem.Date(djd)

print(float(Dawodang.sidereal_time())*180.0/3.1416)
