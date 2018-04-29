from hexy.robot.hexapod import Hexapod
from time import sleep

hexy =  Hexapod()

print("lie flat, curl up, then get up")
hexy.boot_up()

for i in range(4):
	print "rotate left"
	hexy.rotate(offset = 15)

	print "walk forward"
	hexy.walk(swing=25, repetitions = 10)

print "lie down, lie flat, curl up, and die"
hexy.shut_down()
