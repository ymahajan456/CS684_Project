import pygame
import serial
import sys

pygame.init()
screensize = (600, 600)
screen=pygame.display.set_mode(screensize)

WHITE=(255,255,255)
RED=(255,128,128)
YELLOW=(255,255,128)
BLUE=(0,0,255)

joystick = serial.Serial(sys.argv[1], baudrate=115200)
x = 2048
y = 2048

def draw():
    screen.fill(WHITE)
    xs, ys = 556 - x/8, 556 - y/8
    pygame.draw.circle(screen, BLUE, (xs,ys), 3)
    pygame.display.flip()

while True:
    # line = joystick.read(9)
    line = joystick.readline().strip()
    print(line)
    vals = line.split(',')

    try:
        x = int(vals[0])
        y = int(vals[1])
        draw()
    except (IndexError, ValueError):
        pass
