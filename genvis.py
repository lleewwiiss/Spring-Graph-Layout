import sys
import os
import matplotlib.pyplot as plt
from matplotlib.animation import ArtistAnimation


def gen_visualisation():
    """
    Generates an .mp4 movies representing the restructuring
    of the initial graph based on output files from spring.cpp
    """
    fig = plt.figure()
    try:
        center = sys.argv[1] # ensure test_file specified
    except IndexError:
        print('Usage: genvis.py test_file')
        sys.exit(1)
    nodes = 0
    frames = 0
    data = []
    images = []
    # load the initial graph as the first frame
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

    scat = plt.scatter(*zip(*data), s=10)
    # add the frame to list
    images.append([scat])

    # loop over all other output files and generate frames
    for i in range(frames):
        data = []
        with open(str(i) + '.out', 'r') as f:
            for l in f:
                x, y = map(float, l.strip('\n').split(' '))
                data.append([x, y])
        scat = plt.scatter(*zip(*data), s=10)
        images.append([scat])

    # create animation
    line_anim = ArtistAnimation(fig, images, interval=50, blit=True)
    line_anim.save('graph_layout.mp4')
    # plt.show()

    # cleanup output files
    files = [f for f in os.listdir(".") if f.endswith(".out")]
    for f in files:
        os.remove(f)


if __name__ == '__main__':
    gen_visualisation()