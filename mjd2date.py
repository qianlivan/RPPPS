import ephem
import numpy as np
import time
import sys

if (len(sys.argv)<4):
  print 'too few input parameters, format:'
  print 'python jd2date.py jd number_of_subints lenght_of_subint'
  print 'example:'
  print 'python jd2date.py 57905.60810226851852 120 0.8192'
  sys.exit()

jd=float(sys.argv[1])+2400000.5
subints=int(sys.argv[2])
subint_length=float(sys.argv[3])
jd=jd+subints*subint_length/3600./24.
date=ephem.julian_date('1899/12/31 12:00:00')
#date=ephem.julian_date('18991231120000')
djd=jd-date

#print ephem.Date(djd)
print "%04d%02d%02d%02d%02d%02d" % ephem.Date(djd).tuple()

