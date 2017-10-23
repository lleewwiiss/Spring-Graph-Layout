import random


def gen_frames():
    """
    Based on user input will generate a valid test file for use in the
    spring graph layout algorithm
    """
    frames = int(input('Frames:'))
    nodes = int(input('Nodes:'))
    edges = int(input('Edges:'))

    with open('test_n' + str(nodes) + '_f' + str(frames) + '_e' + str(edges), 'w') as f:
        f.write(str(frames) + '\n')
        f.write(str(nodes) + '\n')
        # generate the initial coordinates of the graph
        for i in range(nodes):
            a = random.randrange(0, nodes)
            b = random.randrange(0, nodes)
            f.write(str(a) + ' ' + str(b) + '\n')
        # generate the respective edges for each node in the graph
        for j in range(nodes):
            edges_list = []
            # number of edges based on user input
            for k in range(edges):
                a = random.randrange(0, nodes)
                while a == j or a not in edges_list:  # ensure edge doesn't point to itself or repeat edges
                    a = random.randrange(0, nodes)
                    edges_list.append(a)
                f.write(str(a) + ' ')
            f.write('\n')


if __name__ == '__main__':
    gen_frames()
