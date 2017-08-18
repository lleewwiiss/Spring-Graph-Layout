#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <sstream>
#include <vector>
#include <cmath>


float beta = 0.0001;
float alpha = 1.0;
int frames = 0;

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
        edge f{};
        f.start = v;
        f.end = u;
        f.distance = sqrt(pow((v.x - u.x), 2) + pow((v.y - u.y), 2));
        f.difference = ((v.x - u.x) + (v.y - u.y))/f.distance;
        graph_container[u].insert(f);
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
        float coulomb = 0.0;
        float hook = 0.0;
        for (auto &it2 : it.second) {
            coulomb += (beta / pow(it2.distance, 2)) * it2.difference;
            hook += -1 * alpha * (it2.distance - 1.0) * it2.difference;
        }
        node t = it.first;
        std::set<edge> e = it.second;
        t.force = coulomb + hook;
        new_map[t] = e;
    }
    g.set_graph(new_map);
}

bool equilibrium(const graph &g) {
    std::map<node, std::set<edge>> map = g.get_graph();
    float force = 0.0;
    for (auto it : map) {
        force += fabs(it.first.force);
    }
    return (force > 0.01);
}

void move(graph &g) {
    std::map<node, std::set<edge>> map = g.get_graph();
    std::map<node, std::set<edge>> new_map;

    for (const auto &it : map) {
        node n{};
        n.id = it.first.id;
        n.x = it.first.x + 0.01*it.first.force;
        n.y = it.first.y + 0.01*it.first.force;
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

int balance_graph(graph &g){
    while (equilibrium(g)) {
        calc_force(g);
        move(g);
        update_edges(g);
    }

    return 0;
}

int main(int argc, char **argv) {

    std::string line, file;
    std::string::size_type sz;

    int nodes = 0, i = 0, j = 0;

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
                std::stringstream parse(line);
                parse >> j;
                frames = j;
            } else if (i == 1) {
                std::stringstream parse(line);
                parse >> j;
                nodes = j;
            } else if (i <= nodes + 1) {
                node n{};
                std::stringstream parse(line);
                parse >> j;
                n.x = j;
                parse >> j;
                n.y = j;
                n.id = i - 2;
                g.add_node(n);
            } else {
                node temp{};

                std::stringstream parse(line);

                temp.id = i - nodes - 2;

                while (parse >> j) {
                    node temp2{};
                    temp2.id = j;
                    g.add_edge(temp, temp2);
                }
            }

            i++;

        }
        myfile.close();
    } else {
        std::cout << "Unable to open file";
    }

    std::cout << "Graph Loaded";

    return balance_graph(g);
}
