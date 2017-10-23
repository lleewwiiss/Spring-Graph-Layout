import random

frames = int(input('Frames:'))
nodes = int(input('Nodes:'))
edges = int(input('Edges:'))

with open('test_n' + str(nodes) + '_f' + str(frames) + '_e' + str(edges), 'w') as f:
    f.write(str(frames) + '\n')
    f.write(str(nodes) + '\n')
    for i in range(nodes):
        a = random.randrange(0, nodes)
        while a == i:
            a = random.randrange(0, nodes)
        b = random.randrange(0, nodes)
        while b == i or a == b:
            b = random.randrange(0, nodes)
        f.write(str(a) + ' ' + str(b) + '\n')
    prev = -1
    for j in range(nodes):
        for k in range(edges):
            a = random.randrange(0, nodes)
            while a == j or a == prev:
                a = random.randrange(0, nodes)
            prev = a
            f.write(str(a) + ' ')
        f.write('\n')
