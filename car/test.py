#!/usr/bin/env python3

import RPi.GPIO as GPIO
import time

# Set up pins
MotorPin1   = 13
MotorPin2   = 19
MotorEnable = 26
# Set up pins
MotorFrontPin1   = 16
MotorFrontPin2   = 20
MotorFrontEnable = 21

pwm = None


def setup():
	global pwm
	global pwmFront

	GPIO.setmode(GPIO.BCM)
	GPIO.setup(MotorPin1, GPIO.OUT)
	GPIO.setup(MotorPin2, GPIO.OUT)
	GPIO.setup(MotorEnable, GPIO.OUT, initial=GPIO.LOW)

	GPIO.setup(MotorFrontPin1, GPIO.OUT)
	GPIO.setup(MotorFrontPin2, GPIO.OUT)
	GPIO.setup(MotorFrontEnable, GPIO.OUT, initial=GPIO.LOW)

	pwm = GPIO.PWM(MotorEnable, 100)
	pwm.start(0) 

	# pwmFront = GPIO.PWM(MotorFrontEnable, 100)
	# pwmFront.start(0) 	

def set_speed():
	global pwm
	
	speeds = [10,20,40,60,80,100]
	for speed in speeds:
		print("Setting speed to" + str(speed))
		pwm.ChangeDutyCycle(speed)
		time.sleep(3)

def motor(direction):
	global pwm

	# Clockwise
	if direction == 1:
		print ("Rear motor Counterclockwise - Front motor right ")		
		GPIO.output(MotorFrontPin1, GPIO.HIGH)
		GPIO.output(MotorFrontPin2, GPIO.LOW)
		GPIO.output(MotorFrontEnable, GPIO.HIGH)

		GPIO.output(MotorPin1, GPIO.HIGH)
		GPIO.output(MotorPin2, GPIO.LOW)
		#GPIO.output(MotorEnable, GPIO.HIGH)
		set_speed()
	# Counterclockwise
	if direction == -1:
		print ("Rear motor clockwise - Front motor left ")
		GPIO.output(MotorFrontPin1, GPIO.LOW)
		GPIO.output(MotorFrontPin2, GPIO.HIGH)
		GPIO.output(MotorFrontEnable, GPIO.HIGH)

		GPIO.output(MotorPin1, GPIO.LOW)
		GPIO.output(MotorPin2, GPIO.HIGH)
		set_speed()
		#GPIO.output(MotorEnable, GPIO.HIGH)
	# Stop
	if direction == 0:
		print ("Stop")		
		GPIO.output(MotorFrontEnable, GPIO.LOW)
		#GPIO.output(MotorEnable, GPIO.LOW)
		pwm.ChangeDutyCycle(0)
		time.sleep(5)

def main():

	# Define a dictionary to make the script more readable
	# CW as clockwise, CCW as counterclockwise, STOP as stop
	directions = {'CW': 1, 'CCW': -1, 'STOP': 0}
	
	while True:
		# Clockwise
		motor(directions['CW'])

		# Stop
		motor(directions['STOP'])

		# Anticlockwise
		motor(directions['CCW'])

		# Stop
		motor(directions['STOP'])

def destroy():
	GPIO.output(MotorEnable, GPIO.LOW)
	GPIO.output(MotorFrontEnable, GPIO.LOW)
	GPIO.cleanup()

# If run this script directly, do:
if __name__ == '__main__':
	setup()
	try:
		main()
	except KeyboardInterrupt:
		destroy()
