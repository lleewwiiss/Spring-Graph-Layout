import random
import sys

frames = int(sys.argv[1])
nodes = int(sys.argv[2])

with open('test_n' + str(nodes) + '_f' + str(frames), 'w') as f:
    f.write(str(frames) + '\n')
    f.write(str(nodes) + '\n')
    for i in range(nodes):
        f.write(str(random.uniform(0, 1)) + ' ' + str(random.uniform(0, 1)) + '\n')
    for j in range(nodes):
        a = random.randrange(0, nodes)
        while a == j:
            a = random.randrange(0, nodes)
        b = random.randrange(0, nodes)
        while b == j and a == b:
            b = random.randrange(0, nodes)
        f.write(str(a) + ' ' + str(b) + '\n')
