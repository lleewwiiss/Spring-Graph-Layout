#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <sstream>
#include <vector>
#include <cmath>
#include <ctime>


double beta = -3.0;
int frames = 0;
int nodes = 0;

struct node {
    double x, y, force_x, force_y;
    int id;
};

bool operator==(const node &l, const node &r) {
    return (l.id == r.id);
}

bool operator<(const node &l, const node &r) {
    return (l.id < r.id);
}

struct edge {
    node start, end;
    double distance, dx, dy;
};


bool operator<(const edge &l, const edge &r) {
    return (l.end.id != r.end.id);
}




class graph {
private:
    std::map<node, std::set<edge>> graph_container;
public:
    void add_node(const node &v) {
        std::set<edge> new_set;
        graph_container[v] = new_set;
    }

    void add_edge(const node &v, const node &u) {
        edge e{};
        e.start = v;
        e.end = u;
        e.dx = (u.x - v.x);
        e.dy = (u.y - v.y);
        e.distance = e.dx * e.dx + e.dy * e.dy;
        graph_container[v].insert(e);
    }

    std::map<node, std::set<edge>> get_graph()const {
        return graph_container;
    };

    void set_graph(std::map<node, std::set<edge>> &g) {
        graph_container = g;
    };

};


void calc_force(graph &g) {
    std::map<node, std::set<edge>> map = g.get_graph();
    std::map<node, std::set<edge>> new_map;
    //loop through each node in graph
    for (auto it : map) {
        double coulomb_x = 0.0, coulomb_y = 0.0;
        double hook_x = 0.0, hook_y = 0.0;
        //get hook forces for each edge
        for (auto &it2 : it.second) {
            hook_x += 1.0 * (sqrt(it2.distance) - 2.0) * (it2.dx / sqrt(it2.distance));
            hook_y += 1.0 * (sqrt(it2.distance) - 2.0) * (it2.dy / sqrt(it2.distance));
        }
        //get coulomb forces for all other nodes
        for (auto &it3 : map) {
            double dx, dy, dist;
            //don't consider set
            if (it3.first.id != it.first.id) {
                dx = it3.first.x - it.first.x;
                dy = it3.first.y - it.first.y;
                dist = dx * dx + dy * dy;
                coulomb_x += (beta / dist) * (dx / sqrt(dist));
                coulomb_y += (beta / dist) * (dy / sqrt(dist));
            }
        }
        node t = it.first;
        std::set<edge> e = it.second;
        t.force_x = coulomb_x + hook_x;
        t.force_y = coulomb_y + hook_y;
        new_map[t] = e;
    }
    g.set_graph(new_map);
}


void move(graph &g) {
    std::map<node, std::set<edge>> map = g.get_graph();
    std::map<node, std::set<edge>> new_map;
    for (const auto &it : map) {
        node n{};
        n.id = it.first.id;
        n.x = it.first.x + 0.05 * it.first.force_x;
        n.y = it.first.y + 0.05 * it.first.force_y;
        new_map[n] = it.second;
    }
    g.set_graph(new_map);
}

void update_edges(graph &g) {
    std::map<node, std::set<edge>> map = g.get_graph();
    graph g_temp;
    for (auto it : map) {
        g_temp.add_node(it.first);
        for (auto &it2 : it.second) {
            g_temp.add_edge(it.first, it2.end);
        }
    }
    g = g_temp;
}

void output_graph(const graph &g, int i) {
    std::string file = std::to_string(i) + ".txt";
    std::ofstream myfile;
    myfile.open(file);
    for (const auto &it : g.get_graph()) {
        myfile << it.first.x << " " << it.first.y << "\n";
    }
    myfile.close();
}

int balance_graph(graph &g){
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

    start = std::clock();

    int i = 0, j = 0;

    if ((argc <= 1) || (argv[argc - 1] == nullptr) || (argv[argc - 1][0] == '-') || (argc > 2)) {
        std::cerr << "Usage: sprint input_file" << std::endl;
        return 1;
    }


    file = argv[argc - 1];

    std::ifstream myfile(file);
    graph g;

    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            if (i == 0) {
                frames = stoi(line);
            } else if (i == 1) {
                nodes = stoi(line);
            } else if (i <= nodes + 1) {
                node n{};
                n.x = std::stof(line, &sz);
                n.y = std::stof(line.substr(sz));
                n.id = i - 2;
                g.add_node(n);
            } else {
                node temp{};

                std::stringstream parse(line);

                temp.id = i - nodes - 2;

                auto it = g.get_graph().find(temp);
                temp = it->first;
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

    calc_force(g);

    
    int temp = balance_graph(g);
    duration = (std::clock() - start) / (double) CLOCKS_PER_SEC;   
    std::cout << "printf: " << duration << '\n';

    return temp;
}
