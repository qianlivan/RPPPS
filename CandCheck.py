#Candidates checking, pzc, 20150619

import os,sys, getopt

#1, checking if knownPSR.dat exists
filename=r'./knownPSR.dat'
if os.path.exists(filename):
  #for debug: print 'The knownPSR.dat exists.'
  database = 1
else:
  #print 'The known pulsar list file knownPSR.dat does not exist, try to create it.'
  database = 0

#1.1, if does not exist, checking if command "psrcat" exists
if(database == 0):
  #for debug: print 'Checking if psrcat exists.....'
  psrcat_flag = os.popen('which psrcat').readlines()
#1.1.1, if "psrcat" exists, run "psrcat -x -c "name Jname raj decj p0 dm s1400 type binary survey" > knownPSR.dat"
  if(len(psrcat_flag) != 0):
    #print 'Creating knownPSR.dat.....'
    tmp = os.popen('psrcat -x -c \"name Jname RaJ DecJ p0 dm s1400 type binary survey\" > knownPSR.dat').readlines()
    #print 'The file knownPSR.dat created successfully.'
#1.1.2, if "psrcat" does not exist, exit
  if(len(psrcat_flag) == 0):
    print 'No psrcat is found, please install psrcat first, thank you!'
    exit()
#1.2, if exists, reading the data file
#for debug: print 'Reading knownPSR.dat.....'
fp = open(filename, 'r')
#psrdata = fp.readlines()
#print len(psrdata)
#fp.close()
#data: name, Jname, RA, DEC, p0, DM, s1400, binary
#index:    0,     2, 4,   7, 10, 13,    16,     20
#data = [[0 for col in range(2500)] for row in range(8)]
name = []
jname = []
ra = []
dec = []
p = []
dm = []
s1400 = []
binary = []
#for i in range(0,len(psrdata)-1):
#  data.append('')
#  for j in range(0, 7):
#    data[i].append('')
flag = 0
n_known_psr = 0.0
for lines in fp:
  #tmp1 = lines.replace("\n"," ")
  #tmp2 = tmp1.replace("    ",",")
  #tmp3 = tmp2.split(',')
  tmp = lines.split()
  #data.append[tmp3]
  #print tmp[0],tmp[2],tmp[4],tmp[7],tmp[10],tmp[13],tmp[16],tmp[20]
  #data = [tmp[0],tmp[2],tmp[4],tmp[7],tmp[10],tmp[13],tmp[16],tmp[20]]
  #flag = flag + 1
  #data[flag].append(tmp[0],tmp[2],tmp[4],tmp[7],tmp[10],tmp[13],tmp[16],tmp[20])
  name.append(tmp[0])
  jname.append(tmp[2])
  #tmp = tmp[4]
  #tmp_h = float(tmp[0:2])*15.0
  #tmp_m = float(tmp[4:5])*15.0/60.0
  #tmp_s = float(tmp[7:12])*15.0/3600.0
  #raa = tmp_h + tmp_m + tmp_s
  ra.append(tmp[4])
  dec.append(tmp[7])
  p.append(tmp[10])
  dm.append(tmp[13])
  s1400.append(tmp[16])
  binary.append(tmp[20])
  #data[flag][0] = tmp[0]
  #flag = flag + 1
  n_known_psr = n_known_psr + 1
#for debug: print "%d known pulsars have been read." %(n_known_psr)
fp.close()

#convert RA from h/m/s to degree
for i in range(0,len(ra)):
  tmp = ra[i]
  tmp_h = 0.0
  tmp_m = 0.0
  tmp_s = 0.0
  if(tmp == "*"):
    tmp_h = 0.0
    tmp_m = 0.0
    tmp_s = 0.0
  else:
    #print tmp
    tmp_h = float(tmp[0:2])*15.0
    if(len(tmp[3:5]) == 0):
      tmp_m = 0.0
    else:
      tmp_m = float(tmp[3:5])*15.0/60.0
    if(len(tmp[6:12]) == 0):
      tmp_s = 0.0
    else:
      tmp_s = float(tmp[6:12])*15.0/3600.0
  raa = tmp_h + tmp_m + tmp_s
  ra[i] = raa
#print ra

#convert Dec from d/m/s to degree
for i in range(0,len(dec)):
  tmp = dec[i]
  #print tmp
  tmp_h = 0.0
  tmp_m = 0.0
  tmp_s = 0.0
  if(tmp == "*"):
    tmp_h = 0.0
    tmp_m = 0.0
    tmp_s = 0.0
  else:
    tmp_h = float(tmp[0:3])
    if(len(tmp[4:6]) == 0):
      tmp_m = 0.0
    else:
      tmp_m = float(tmp[4:6])/60.0
    if(len(tmp[8:13]) == 0):
      tmp_s = 0.0
    else:
      tmp_s = float(tmp[8:13])/3600.0
    if(tmp_h < 0):
      tmp_m = tmp_m * (-1)
      tmp_s = tmp_s * (-1)
  decc = tmp_h + tmp_m + tmp_s
  dec[i] = decc
#print dec

for i in range(0,len(p)):
  if(p[i] == '*'):
    p[i] = '-1'
  p[i] = float(p[i])

for i in range(0,len(dm)):
  if(dm[i] == '*'):
    dm[i] = '-1'
#  dm[i] = float(dm[i])

for i in range(0,len(dm)):
  if(s1400[i] == '*'):
    s1400[i] = '-1'
  s1400[i] = float(s1400[i])



#for debug: print "reading inputs..."
#obtain input parameters
'''opts, args = getopt.getopt(sys.argv[1:], "",["ra","dec","p","dm"])
print opts, args
for op, value in opts:
  if op == "--ra":
    in_ra = value
  elif op == "--dec":
    in_dec = value
  elif op == "--p":
    in_p == value
  elif op == "--dm":
    in_dm = value
print in_p'''

tmp_ra = sys.argv[1]
degree_ra = tmp_ra.find(':')
if(degree_ra == -1):
  in_ra = float(tmp_ra)
else:
  tmp = tmp_ra
  #print tmp
  tmp_h = float(tmp[0:2])*15.0
  #print tmp_h
  #print tmp[3:5]
  if(len(tmp[3:5]) == 0):
    tmp_m = 0.0
  else:
    tmp_m = float(tmp[3:5])*15.0/60.0
  #print tmp_m
  if(len(tmp[6:12]) == 0):
    tmp_s = 0.0
  else:
    tmp_s = float(tmp[6:12])*15.0/3600.0
  #print tmp_s
  raa = tmp_h + tmp_m + tmp_s
  in_ra = raa
if(in_ra < 0 or in_ra > 360):
  print "You input RA is wrong, please check it!"
  exit()

tmp_dec = sys.argv[2]
degree_dec = tmp_dec.find(':')
if(degree_dec == -1):
  in_dec = float(tmp_dec)
else:
  tmp = tmp_dec
  #print tmp
  tmp_h = float(tmp[0:3])
  #print tmp_h
  if(len(tmp[4:6]) == 0):
    tmp_m = 0.0
  else:
    tmp_m = float(tmp[4:6])/60.0
  #print tmp_m
  if(len(tmp[8:13]) == 0):
    tmp_s = 0.0
  else:
    tmp_s = float(tmp[8:13])/3600.0
  #print tmp_s
  if(tmp_h < 0):
    tmp_m = tmp_m * (-1)
    tmp_s = tmp_s * (-1)
  decc = tmp_h + tmp_m + tmp_s
  in_dec = decc
if(in_dec > 90 or in_dec < -90):
  print "You input DEC is wrong, please check it!"
  exit()

in_p = float(sys.argv[3])
in_p = in_p / 1000.0
if(p <= 0):
  print "The rotation period should be larger than 0!"
  exit()
in_dm = float(sys.argv[4])
if(in_dm < 0):
  print "No DM smaller than 0!"
  exit()
#for debug: print "Input parameters:    RA %6.2f, DEC %6.2f, period %8.4f, DM %6.2f." %(in_ra, in_dec, in_p*1000.0, in_dm)




#2, checking RA and DEC with the given errors
#checking input format
#input format:1, 11:22:33 33:22:11; 2, 34.567 76.543
#2.1, if with ':'
length = len(ra)
errors_position = 5.0 # +/- 1 degree
errors_p = 0.00005# 1/10000 period
#may be    0.0002 for some cases: J1840-0840, PM0160_00121 17th harmonics
if(in_p < 0.01):
  errors_p = errors_p * 10.0
errors_dm = 250
harmonics = 60
repeats = 25
m = 0
n = 0
match_message = "This is: %s, RA %6.2f, DEC %6.2f, period %8.4f, DM %6.2f, S1400 %4.2f, binary type %s, harmonics %2d, repeated %2d, errors %.2e."
#running = "prepfold -mask %s -nsub 96 -dm %6.2f -accelfile %s -accelcand %d -nosearch %s"
for i in range(0,length-1):
  if(abs(ra[i] - in_ra) <= errors_position):
    if(abs(dec[i] - in_dec) <= errors_position):
      #print name[i]
      tmp1 = 1.00000
      for m in range(1,harmonics):
        for n in range(1,repeats):
          if(((m % n) != 0 and float(m)/float(n) != tmp1) or (m == 1 or n == 1) or (float(p[i] <= 0.0))):
            tmp = abs((float(in_p)*m/n - float(p[i]))/float(p[i]))
            #print name[i],m,n,tmp
            if(tmp <= errors_p):
              #print name[i]
              if(abs(in_dm - float(dm[i])) <= errors_dm or float(dm[i]) <= 0.0):
                tmp1 = float(m)/float(n);
                #print str(name[i]), float(ra[i]), float(dec[i]), float(p[i])*1000.0, float(dm[i]), float(s1400[i]), str(binary[i]), int(m), int(n)
                #if(m <= 5 or n <= 5):
                print match_message % (str(name[i]), float(ra[i]), float(dec[i]), float(p[i])*1000.0, float(dm[i]), float(s1400[i]), str(binary[i]), int(m), int(n), tmp)
                  #print running % ("PM0001_00111_rfifind.mask", float(dm[i]), "PM0001_00111_ACCEL_50.cand", 1, "PM0001_00111.sf")
#i = 0
#print name[i],ra[i],dec[i],p[i],dm[i],s1400[i],binary[i]



