#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <sstream>
#include <vector>
#include <cmath>
#include <ctime>

// constant used for Coloumbs force calculations
double beta = -10.0;
int frames = 0;
int nodes = 0;

// struct to store nodes in the graph
struct node {
    double x, y, force_x, force_y;
    int id;
};

bool operator<(const node &l, const node &r) {
    return (l.id < r.id);
}

// struct to store edge information
struct edge {
    node end;
    double distance, dx, dy;
};


bool operator<(const edge &l, const edge &r) {
    return (l.end.id != r.end.id);
}


class graph {
private:
    std::map<node, std::set<edge>> graph_container;
public:

    // creates a new node
    void add_node(const node &v) {
        std::set<edge> new_set;
        graph_container[v] = new_set;
    }

    // adds edge to graph_container vector
    void add_edge(const node &v, const node &u) {
        edge e{};
        e.end = u;
        // calculate distance based on vectors
        e.dx = (u.x - v.x);
        e.dy = (u.y - v.y);
        e.distance = e.dx * e.dx + e.dy * e.dy;
        graph_container[v].insert(e);
    }

    // returns graph
    std::map<node, std::set<edge>> get_graph() const {
        return graph_container;
    }

    // update graph
    void set_graph(std::map<node, std::set<edge>> &g) {
        graph_container = g;
    }
};

/*
 * O(n^2) time complexity, loops through every node in the graph
 * and calculates the respective force based on each other node and
 * edge in the graph and updates the values stored in the respective
 * nodes
 */
void calc_force(graph &g) {
    std::map<node, std::set<edge>> map = g.get_graph();
    std::map<node, std::set<edge>> new_map;
    // TODO: try adding MPI to further improve performance
    //loop through each node in graph
    #pragma omp parallel for
    for (auto it : map) {
        double coulomb_x = 0.0, coulomb_y = 0.0;
        double hook_x = 0.0, hook_y = 0.0;

        //get hook forces for each edge
        #pragma omp parallel for reduction(+:hook_x, hook_y)
        for (const auto &it2 : it.second) {
            hook_x += 1.0 * (sqrt(it2.distance) - 2.0) * (it2.dx / sqrt(it2.distance));
            hook_y += 1.0 * (sqrt(it2.distance) - 2.0) * (it2.dy / sqrt(it2.distance));
        }

        //get coulomb forces for all other nodes
        #pragma omp parallel for reduction(+:coulomb_x, coulomb_y)
        for (auto &it3 : map) {
            double dx = 0.0, dy = 0.0, dist = 0.0;
            //don't consider self
            if (it3.first.id != it.first.id) {
                dx = it3.first.x - it.first.x;
                dy = it3.first.y - it.first.y;
                dist = dx * dx + dy * dy;

                if (dist != 0) {
                    coulomb_x += (beta / dist) * (dx / sqrt(dist));
                    coulomb_y += (beta / dist) * (dy / sqrt(dist));
                }
            }
        }
        // update node
        node t = it.first;
        t.force_x = coulomb_x + hook_x;
        t.force_y = coulomb_y + hook_y;
        new_map[t] = it.second;
    }

    g.set_graph(new_map);
}

/*
 * Move each node in the graph a small amount based
 * on a constant factor which can be modified to change
 * the momentum of the graph
 */
void move(graph &g) {
    std::map<node, std::set<edge>> map = g.get_graph();
    std::map<node, std::set<edge>> new_map;

    #pragma omp parallel for
    for (auto &it : map) {
        node n{};
        n.id = it.first.id;
        n.x = it.first.x + 0.05 * it.first.force_x;
        n.y = it.first.y + 0.05 * it.first.force_y;
        new_map[n] = it.second;
    }

    g.set_graph(new_map);
}

/*
 * For each edge in the graph update the distance information
 * after the nodes have been moved for quick access later
 */
void update_edges(graph &g) {
    std::map<node, std::set<edge>> map = g.get_graph();
    graph g_temp;

    #pragma omp parallel for
    for (auto &it : map) {
        g_temp.add_node(it.first);
        #pragma omp parallel for
        for (auto &it2 : it.second) {
            // will recalculate the distance functions and update
            g_temp.add_edge(it.first, it2.end);
        }
    }
    g = g_temp;
}

/*
 * Write the current graph to file labelled based on the
 * number of movement iterations
 */
void output_graph(const graph &g, int i) {
    std::ostringstream filename;
    filename << i << ".out";
    std::string file = filename.str();
    std::ofstream myfile;
    myfile.open(file);

    // print the current location of each node in the graph
    // TODO: Added edge information
    for (auto &it : g.get_graph()) {
        myfile << it.first.x << " " << it.first.y << "\n";
    }
    myfile.close();
}

/*
 * For the number of frames specified move the nodes,
 * and print the new locations
 */
int balance_graph(graph &g) {
    int i = 0;

    while (i < frames) {
        move(g);
        update_edges(g);
        calc_force(g);
        output_graph(g, i++);
    }

    return 0;
}

int main(int argc, char **argv) {
    std::string line, file;
    std::string::size_type sz;
    std::clock_t start;
    double duration;

    // track running time
    start = std::clock();

    int i = 0, j = 0;

    if ((argc <= 1) || (argv[argc - 1] == nullptr) || (argv[argc - 1][0] == '-') || (argc > 2)) {
        std::cerr << "Usage: sprint input_file" << std::endl;
        return 1;
    }


    file = argv[argc - 1];

    std::ifstream myfile(file);
    graph g;

    // read in test file
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            if (i == 0) {
                frames = stoi(line);
            } else if (i == 1) {
                nodes = stoi(line);
            } else if (i <= nodes + 1) {
                // get locations of all the nodes
                node n{};
                n.x = std::stof(line, &sz);
                n.y = std::stof(line.substr(sz));

                n.id = i - 2;
                g.add_node(n);
            } else {
                // get all edge information
                node temp{};

                std::stringstream parse(line);

                temp.id = i - nodes - 2;

                auto it = g.get_graph().find(temp);

                temp = it->first;
                // get all edges in line
                while (parse >> j) {
                    node temp2{};
                    temp2.id = j;
                    auto it2 = g.get_graph().find(temp2);
                    temp2 = it2->first;
                    g.add_edge(temp, temp2);
                }
            }
            i++;
        }
        myfile.close();
    } else {
        std::cout << "Unable to open file";
    }

    //calculate initial forces
    calc_force(g);
    int temp = balance_graph(g);

    duration = (std::clock() - start) / (double) CLOCKS_PER_SEC;
    // print running time
    std::cout << "printf: " << duration << '\n';

    return temp;
}
