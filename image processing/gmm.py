import matplotlib.pyplot as plt
from sklearn.mixture import GaussianMixture
from matplotlib.pyplot import imshow
import cv2
import numpy as np

np.set_printoptions(threshold=np.nan)
NUM_PIXELS = 4800
NUM_COMPONENTS = 3
GAUSSIAN_BLUR_SIZE = 5


##### Reads in images #####


# s = 'data/rgb14.jpg'
# s = 'data/rgbBrownCh2.jpg'
s = 'data/rgbBlackCh1.jpg'
# s = 'data/rgbNS2.jpg'
cimg = cv2.imread(s) # color image
cimg = cv2.GaussianBlur(cimg,(GAUSSIAN_BLUR_SIZE,GAUSSIAN_BLUR_SIZE),0)
img = cv2.cvtColor(cimg, cv2.COLOR_BGR2GRAY)


##############################################################################
############################### THRESHOLDING #################################
##############################################################################
img_hsv = cv2.cvtColor(cimg, cv2.COLOR_BGR2HSV)

# lower mask
lower_colors1 = np.array([0,50,50])
upper_colors1 = np.array([20,255,255])

# upper mask
lower_colors2 = np.array([150,50,50])
upper_colors2 = np.array([180,255,255])

# join masks
mask = cv2.inRange(img_hsv, lower_colors1, upper_colors1) + cv2.inRange(img_hsv, lower_colors2, upper_colors2)

output_hsv = img_hsv.copy()
output_hsv[np.where(mask==0)] = 0

output_threshold = cv2.cvtColor(output_hsv, cv2.COLOR_HSV2BGR)

output_threshold_gray = (cv2.cvtColor(output_threshold, cv2.COLOR_BGR2GRAY)).astype(np.uint16)


##############################################################################
###################################  GMM  ####################################
##############################################################################

gmm = GaussianMixture(n_components=NUM_COMPONENTS, covariance_type="tied")
x,y,z = cimg.shape
data = cimg.reshape(x*y, 3)
gmm = gmm.fit(data)
cluster = gmm.predict(data)


data = cluster.reshape(x,y) # data is now grayscale image

# plt.imshow(data, cmap='gray')
# plt.show('hold')
# cv2.waitKey(0)
# cv2.destroyAllWindows()

data = data.astype(np.uint8)

##############################################################################
#################### FIND INDEX CORRESPONDING TO HEAT VALUE ##################
##############################################################################
gmm_index_with_highest_heat_value = -1;
max_count = -1;

for i in xrange(NUM_COMPONENTS):
	data_matrix = np.copy(data)

	if i == 0:
		data_matrix[data_matrix == 0] = NUM_COMPONENTS + 1
		data_matrix[data_matrix < (NUM_COMPONENTS + 1)] = 0

	else:	
		data_matrix[data_matrix != i] = 0

	# element wise multiply
	count_matrix = np.multiply(data_matrix, (output_threshold_gray))
	# print count_matrix

	# find number of pixels in threshold relative to how many there are

	if np.count_nonzero(data_matrix) != 0:
		count = float(np.count_nonzero(count_matrix))/np.count_nonzero(data_matrix)
	else:
		print 'yo'
		count = 0
	countorig = np.count_nonzero(count_matrix)
	
	if count > max_count:
		max_count = count
		gmm_index_with_highest_heat_value = i

# filter out everything that's not the gmm index we're looking for
data[data == gmm_index_with_highest_heat_value] = NUM_COMPONENTS + 1
data[data < (NUM_COMPONENTS + 1)] = 255 # make everything white that is irrelevant
# plt.imshow(data, cmap='gray')
# plt.show('hold')
# cv2.waitKey(0)


##############################################################################
##### Blob detection #####
##############################################################################

pad = 5
data = np.pad(data, ((pad,pad),(pad,pad)), 'constant', constant_values=(255)) # pad array to get corner blobs
params = cv2.SimpleBlobDetector_Params()

params.filterByArea = True;
params.minArea = 15;
params.maxArea = 1000;
params.filterByColor = False;
params.filterByCircularity = False
params.filterByConvexity = False
params.filterByInertia = False


detector = cv2.SimpleBlobDetector_create(params)

# Detect blobs.
keypoints = detector.detect(data)

# img = np.pad(img, ((pad,pad),(pad,pad)), 'constant', constant_values=(255)) # pad array to get corner blobs
cimg = np.pad(cimg, ((pad,pad),(pad,pad),(3,3)), 'constant', constant_values=(255)) # pad array to get corner blobs

data_with_keypoints = cv2.drawKeypoints(cimg, keypoints, np.array([]), (0,0,255), cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
data_with_keypoints =cv2.putText(img=np.copy(data_with_keypoints), text="Detected: " + str(len(keypoints)), org=(0,68),fontFace=1, fontScale=0.5, color=(265,191,0), thickness=0)
print data_with_keypoints.shape

# Show keypoints
print 'keypoints'
print len(keypoints)
cv2.imshow("Keypoints", data_with_keypoints)

plt.show('hold')
cv2.waitKey(0)


##############################################################################
##### Hough circle detection: HOW TO ONLY DETECT ONE CHANNEL POTENTIALLY #####
##############################################################################

# try:
# 	circles = cv2.HoughCircles(data,cv2.HOUGH_GRADIENT,1,30,param1=10,param2=3,minRadius=4,maxRadius=10)
# 	# print circles

# 	circles = np.uint16(np.around(circles))
# 	for i in circles[0,:]:
# 	    # draw the outer circle
# 	    cv2.circle(cimg,(i[0],i[1]),i[2],(0,255,0),2)
# 	    # draw the center of the circle
# 	    # cv2.circle(cimg,(i[0],i[1]),2,(0,0,255),3)


# 	print 'Number of people found =', len(circles[0])

# 	cv2.imshow('detected circles',cimg)
# 	plt.show('hold')
# 	plt.show()
# 	# time.sleep(0.7)
# 	cv2.waitKey(0)
# 	cv2.destroyAllWindows()
# except:
# 	print 'No people found'


 


