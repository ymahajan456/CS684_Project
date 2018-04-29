"""
  File Name   : profileGen.py
  Description : Prints the RSSI values in to a file log for profiling the arena
  Team ID     : 
  Authors     : Yogesh Mahajan, Meet Udeshi, Prathu baronia
"""

from xbee import XBee
import serial
import sys
from math import sqrt



ser = serial.Serial(sys.argv[1], 9600)

# Use an XBee 802.15.4 device
xbee = XBee(ser)

a = [[],[],[],[]]

def stringfl(arr):
    return (', '.join(['{:2f}'.format(x) for x in arr]))

while True:
    try:
        x = input('Enter x: ')
        y = input('Enter y: ')
        X = x
        actdist = [sqrt(x**2 + y**2),
                    sqrt(x**2 + (150-y)**2),
                    sqrt((150-x)**2 + (150-y)**2),
                    sqrt((150-x)**2 + y**2)]
        # printfl(actdist)
        a = [[],[],[],[]]
        ser.reset_input_buffer()

        while min([len(x) for x in a]) < 5:
            frame = xbee.wait_read_frame()
            src = ord(frame['source_addr'][1])
            rssi = ord(frame['rssi'])
            a[src-2].append(rssi)

        vals = [float(sum(x))/len(x) for x in a]
        file = open('profile3.csv', 'a')
        file.write(str(X) + ', ' + str(y) + ', ')
        file.write(stringfl(vals))
        file.write('\n')
        file.close()
    # printfl([rssi2dist(x, i) for i, x in enumerate(vals)])
    except KeyboardInterrupt:
        break

ser.close()
