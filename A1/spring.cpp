#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <sstream>
#include <vector>
#include <cmath>


double beta = 0.000001;
int frames = 0;
int nodes = 0;

struct node {
    double x, y, force;
    int id;
};

struct edge {
    node start, end;
    double distance, difference;
};

inline bool operator<(const node &l, const node &r) {
    return (l.id < r.id);
}

inline bool operator==(const node &l, const node &r) {
    return (l.id == r.id);
}

inline bool operator<(const edge &l, const edge &r) {
    return (l.distance < r.distance);
}

inline bool operator==(const edge &l, const edge &r) {
    return (l.start.id == r.start.id && l.end.id == r.end.id);
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
        e.distance = sqrt(pow((u.x - v.x), 2) + pow((u.y - v.y), 2));
        e.difference = ((u.x - v.x) + (u.y - v.y))/e.distance;
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
    for (auto it : map) {
        double coulomb = 0.0;
        double hook = 0.0;
        for (auto &it2 : it.second) {
            coulomb += (beta / pow(it2.distance, 2)) * it2.difference;
            hook += -1.0 * (it2.distance - 0.05) * it2.difference;
        }
        node t = it.first;
        std::set<edge> e = it.second;
        t.force = coulomb + hook;
        new_map[t] = e;
    }
    g.set_graph(new_map);
}

double equilibrium(const graph &g) {
    std::map<node, std::set<edge>> map = g.get_graph();
    double force = 0.0;
    for (auto it : map) {
        force += fabs(it.first.force);
    }
    return force;
}

void move(graph &g) {
    std::map<node, std::set<edge>> map = g.get_graph();
    std::map<node, std::set<edge>> new_map;

    for (const auto &it : map) {
        node n{};
        n.id = it.first.id;
        n.x = it.first.x + 0.01 * it.first.force;
        n.y = it.first.y + 0.01 * it.first.force;
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
    //double force = equilibrium(g);
    while (i < frames) {
        move(g);
        update_edges(g);
        calc_force(g);
        output_graph(g, i++);
        //force = equilibrium(g);
    }


    return 0;
}

int main(int argc, char **argv) {

    std::string line, file;
    std::string::size_type sz;

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

    return balance_graph(g);
}
