import os

# headless mode
os.environ['SDL_VIDEODRIVER'] = 'dummy'

import sys, pickle, glob
import pygame, pygame.gfxdraw

#run_dir  = '../runs/run' + run + '/'
#run_dir  = sys.argv[1]
img_dir  = 'images/'

if len(sys.argv) == 1:
    data_dir = 'data/'
else:
    data_dir  = sys.argv[1]

print data_dir
print img_dir

files = glob.glob( data_dir + '*.dat' )
files.sort()

n_snapshots = len(files)

print n_snapshots, "snapshots to convert."

# delete the previous images
#os.system('rm images/*.png')

black = (0, 0, 0)
#white = (255, 255, 255, 150)
#white = (255, 255, 255, 25)
#white = (255, 255, 255, 75)
white = (255, 255, 255)
#width, height = 720, 720
width, height = 1280, 720

def draw_frame(screen, particles):
    screen.blit(background, (0, 0))
    # draw particles
    for x,y in particles:
        x, y = height*x/2 + width/2, height*y/2 + height/2
        pygame.gfxdraw.pixel(screen, 
                int(x), int(y), white)
    # not sure if this does anything in headless mode
    #pygame.display.flip()

def load_snapshot(fname):
    f = open(fname)
    particles = []
    for s in f:
        s = s.split()
        s = [float(c) for c in s]
        particles.append(s)
    f.close()
    return particles

pygame.init()
size = width, height

# visual options
#pygame.display.set_caption(' ')
#screen = pygame.display.set_mode(size, pygame.NOFRAME) 
#screen = pygame.Surface( size )

# headless options
pygame.display.set_mode( (1, 1) )
screen = pygame.Surface(size, pygame.SRCALPHA, 32)
pygame.draw.rect(screen, (0,0,0), (0, 0, width, height), 0)

background = pygame.Surface(screen.get_size())
background.fill(black)

clock = pygame.time.Clock()
#for snapshot in snapshots:
for i in range(0, n_snapshots):
    for event in pygame.event.get():
        if event.type == pygame.QUIT: sys.exit()
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE: sys.exit()

    fname = files[i]
    print fname
    savename =  'images' + fname[4:-3] + 'png'

    if not os.path.exists(savename):
        # load the particles
        particles = load_snapshot(fname)

        draw_frame(screen, particles)
        # save the image
        pygame.image.save(screen, savename)

sys.exit()
