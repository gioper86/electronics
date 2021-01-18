#!/usr/bin/env python3

import RPi.GPIO as GPIO
import time
import smbus

button_pin = 21

back_light = True

def setup():
	GPIO.setmode(GPIO.BCM)
	GPIO.setup(button_pin, GPIO.IN)

def handle_click(ev=None):
    global back_light

    BUS = smbus.SMBus(1)
    back_light = not back_light
    if back_light:
        BUS.write_byte(0x27,0x08)
    else:
        BUS.write_byte(0x27,0x00)
    BUS.close()

def main():
	GPIO.add_event_detect(button_pin, GPIO.FALLING, callback=handle_click)
	while True:
		time.sleep(1)

def destroy():
	GPIO.cleanup()

if __name__ == '__main__':
	setup()
	try:
		main()
	except KeyboardInterrupt:
		destroy()
