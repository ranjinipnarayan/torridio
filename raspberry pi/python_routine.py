from subprocess import call

try:
	call(['./raspberry_qt'])
except:
	print('Failed to capture image.')

try:
	call(['sudo', 'python', 's3_upload.py'])
except:
	print('Failed to upload to cloud.')
