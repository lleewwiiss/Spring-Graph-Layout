import matplotlib.pyplot as plt
from matplotlib.animation import ArtistAnimation
import sys
import sys

import matplotlib.pyplot as plt
from matplotlib.animation import ArtistAnimation

fig = plt.figure()
center = sys.argv[1]
nodes = 0
frames = 0
data = []
images = []
with open(center, 'r') as f:
    for i, l in enumerate(f):
        if i == 0:
            frames = int(l.strip('\n'))
        elif i == 1:
            nodes = int(l.strip('\n'))
        elif 1 < i <= nodes + 1:
            x, y = map(float, l.strip('\n').split(' '))
            data.append([x, y])
        elif i > nodes + 1:
            break

scat = plt.scatter(*zip(*data), s=100)
images.append([scat])

for i in range(frames):
    data = []
    with open(str(i), 'r') as f:
        for l in f:
            x, y = map(float, l.strip('\n').split(' '))
            data.append([x, y])
    scat = plt.scatter(*zip(*data), s=100)
    images.append([scat])

line_anim = ArtistAnimation(fig, images, interval=50, blit=True)
line_anim.save('my_animation.mp4')
plt.show()
