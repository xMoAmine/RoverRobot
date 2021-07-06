# import the necessary packages
from __future__ import print_function
import numpy as np
import argparse
import cv2
import imutils

# construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-i", "--image", help = "path to the image")
args = vars(ap.parse_args())



# load the image
image = cv2.imread(args["image"])
hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)





# define the list of boundaries
boundaries = [
	([160, 175, 175], [179, 255, 255])
]

# loop over the boundaries
for (lower, upper) in boundaries:
	# create NumPy arrays from the boundaries
	lower = np.array(lower, dtype = "uint8")
	upper = np.array(upper, dtype = "uint8")

	# find the colors within the specified boundaries and apply
	# the mask
	mask = cv2.inRange(hsv, lower, upper)
	output = cv2.bitwise_and(image, image, mask = mask)

	output2 = cv2.cvtColor(output, cv2.COLOR_HSV2BGR)
	gray = cv2.cvtColor(output2, cv2.COLOR_BGR2GRAY)
	ret,thresh = cv2.threshold(gray,127,255,0)
	if imutils.is_cv3():
	        (_, cnts, _) = cv2.findContours(thresh.copy(), cv2.RETR_EXTERNAL,
        	        cv2.CHAIN_APPROX_SIMPLE)


	# show the images
	cv2.drawContours(image, cnts, -1, (0,255,0), 2)
	cv2.imshow("images", np.hstack([image, output]))
	cv2.waitKey(0)
