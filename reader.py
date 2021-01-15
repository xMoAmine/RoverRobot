import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BCM)

GPIO.setup(26, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)


while 1:
	print(GPIO.input(26))