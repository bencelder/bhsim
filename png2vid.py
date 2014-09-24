import os, sys

'''
ffmpeg -f image2 -r 1/5 -i image%05d.png -vcodec mpeg4 -y movie.mp4

This command takes images with 5 digit padding (image00001.png) and compresses them into an mpeg4 quicktime, at a rate of holding each frame for 5 seconds. You could do a different fps if your images are greater: -r 24 (24 frames per second)
'''

#run_dir = sys.argv[1]
image_dir = 'images/'

fps = 24
# this one has some problems with compression -- pixels drop out =(
#cmd = "ffmpeg -f image2 -r 12 -i img/fr_%05d.png -vcodec mpeg4 -y movie.mp4"
cmd = "ffmpeg -f image2 -r 24 -i " + image_dir + "%05d.png -q:v 0 -vcodec mpeg4 -y movie.mp4"
os.system(cmd)
