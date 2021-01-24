#!/usr/bin/env python3

import RPi.GPIO as GPIO
import time

# Set up pins
MotorPin1   = 17
MotorPin2   = 27
MotorEnable = 22
pmw = None

def setup():
	global pwm
	GPIO.setmode(GPIO.BCM)
	GPIO.setup(MotorPin1, GPIO.OUT)
	GPIO.setup(MotorPin2, GPIO.OUT)
	GPIO.setup(MotorEnable, GPIO.OUT, initial=GPIO.LOW)

	pwm = GPIO.PWM(MotorEnable, 100)
	pwm.start(0) 

def motor(direction, speed=50):
	global pwm
	# Clockwise
	pwm.ChangeDutyCycle(speed)
	if direction == 1:
		# Set direction
		GPIO.output(MotorPin1, GPIO.HIGH)
		GPIO.output(MotorPin2, GPIO.LOW)
		# Enable the motor
		#GPIO.output(MotorEnable, GPIO.HIGH)
		print ("Clockwise ", speed)
	# Counterclockwise
	if direction == -1:
		# Set direction
		GPIO.output(MotorPin1, GPIO.LOW)
		GPIO.output(MotorPin2, GPIO.HIGH)
		# Enable the motor
		#GPIO.output(MotorEnable, GPIO.HIGH)
		print ("Counterclockwise ", speed)
	# Stop
	if direction == 0:
		# Disable the motor
		pwm.ChangeDutyCycle(0)
		#GPIO.output(MotorEnable, GPIO.LOW)
		print ("Stop")

def main():

	# Define a dictionary to make the script more readable
	# CW as clockwise, CCW as counterclockwise, STOP as stop
	directions = {'CW': 1, 'CCW': -1, 'STOP': 0}
	speeds = [20, 40, 50, 80, 100]
	while True:
		# Clockwise
		for speed in speeds:
			motor(directions['CW'], speed)
			time.sleep(5)
		# Stop
		motor(directions['STOP'])
		time.sleep(5)
		# Anticlockwise
		for speed in speeds:
			motor(directions['CCW'], speed)
			time.sleep(5)
		# Stop
		motor(directions['STOP'])
		time.sleep(5)

def destroy():
	GPIO.output(MotorEnable, GPIO.LOW)
	GPIO.cleanup()    

# If run this script directly, do:
if __name__ == '__main__':
	setup()
	try:
		main()
	except KeyboardInterrupt:
		destroy()
