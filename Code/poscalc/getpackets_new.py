"""
  File Name   : getpackets_new.py
  Description : Acquires and displays the current location of receiver with 
                averaging
  Team ID     : 
  Authors     : Yogesh Mahajan, Meet Udeshi, Prathu baronia
"""

from positioning import location, rssi2dist
from xbee import XBee
import serial
import sys
from math import sqrt
from time import sleep

ser = serial.Serial(sys.argv[1], 9600)

# Use an XBee 802.15.4 device
xbee = XBee(ser)

a = [[],[],[],[]]
locs = []

def printfl(label, arr):
    print(label + ': ' + (', '.join(['{:2f}'.format(x) for x in arr])))

while True:
    try:
        # x = input('Enter x: ')
        # y = input('Enter y: ')

	# actdist = [sqrt(x**2 + y**2),
        #            sqrt(x**2 + (150-y)**2),
        #            sqrt((150-x)**2 + (150-y)**2),
        #            sqrt((150-x)**2 + y**2)]
        # printfl(actdist)
        a = [[],[],[],[]]
	ser.reset_input_buffer()

        while min([len(x) for x in a]) < 3:
            frame = xbee.wait_read_frame()
            src = ord(frame['source_addr'][1])
            rssi = ord(frame['rssi'])
            a[src-2].append(rssi)

	vals = [float(sum(x))/len(x) for x in a]
        printfl('Rssi', vals)
	# printfl('Dist', [rssi2dist(x, i) for i, x in enumerate(vals)])
	#print("Rssi Values \n")
	#print(type(vals[0]))
	loc = location(L=150,A1_rssi=vals[0],A2_rssi=vals[1],A3_rssi=vals[2],A4_rssi=vals[3]);
	# printfl('Location', loc)
	locs.append(loc)

	if len(locs) > 3:
	    l = len(locs)
	    x = sum([a[0] for a in locs])/l
	    y = sum([a[1] for a in locs])/l
	    print('Average: ')
	    print(x,y)
	    locs = []
	
    except KeyboardInterrupt:
        break

ser.close()
