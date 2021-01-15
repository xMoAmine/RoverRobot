import RPi.GPIO as GPIO
from time import sleep
GPIO.setmode(GPIO.BOARD)


GPIO.setup(37, GPIO.OUT)
pwm=GPIO.PWM(37, 50)


pwm.start(0)

def SetAngle(angle):
	duty = angle / 18 + 2
	GPIO.output(37, True)
	pwm.ChangeDutyCycle(duty)







while(True):
	angle = input("angle:")
	SetAngle(int(angle))


pwm.stop()
GPIO.cleanup()