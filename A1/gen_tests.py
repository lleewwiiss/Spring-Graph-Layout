import random

frames = int(input('Frames:'))
nodes = int(input('Nodes:'))

with open('test_n' + str(nodes) + '_f' + str(frames), 'w') as f:
    f.write(str(frames) + '\n')
    f.write(str(nodes) + '\n')
    for i in range(nodes):
        f.write(str(random.randrange(0, nodes)) + ' ' + str(random.randrange(0, nodes)) + '\n')
    for j in range(nodes):
        a = random.randrange(0, nodes)
        while a == j:
            a = random.randrange(0, nodes)
        b = random.randrange(0, nodes)
        while b == j and a == b:
            b = random.randrange(0, nodes)
        f.write(str(a) + ' ' + str(b) + '\n')
