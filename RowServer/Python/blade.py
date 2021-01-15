
import RPi.GPIO as GPIO
from time import sleep

GPIO.setmode(GPIO.BOARD)
GPIO.setup(36, GPIO.OUT)
pwm=GPIO.PWM(36 ,50)
pwm.start(0)


def SetAngle(angle):
	duty = angle / 18 + 2
	GPIO.output(36, True)
	pwm.ChangeDutyCycle(duty)
	sleep(1)
	GPIO.output(36, False)
	pwm.ChangeDutyCycle(0)



try:

	while(True):
		f = open("blade.txt", "r")

		bladeState = f.read()

		print(bladeState)








		if bladeState is "u":
				SetAngle(200)


		elif bladeState is "d":
				SetAngle(70)

		sleep(5)


except:
	print("file Read failed")

finally:
	pwm.stop()
	GPIO.cleanup()


