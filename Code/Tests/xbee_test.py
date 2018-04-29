import serial 
import RPi.GPIO as GPIO
import os,time

#GPIO.setmode(GPIO.BOARD)

ser = serial.Serial('/dev/ttyUSB0',baudrate=9600,timeout=0.5)

while True:
	text = ser.readline()
	print(text)
	print("\n")
	
