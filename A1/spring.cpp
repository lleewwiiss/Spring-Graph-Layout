#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <sstream>
#include <vector>
#include <cmath>

float beta = 0.0001;
float alpha = 1.0;


struct node {
    float x, y;
    int id;
    float force;
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
public:
    std::map<node, std::set<edge>> graph_container;

    void add_node(node &v) {
        std::set<edge> new_set;
        graph_container[v] = new_set;
    }

    void add_edge(const node &v, const node &u) {
        edge e{};
        e.start = v;
        e.end = u;
        e.difference = (u.x - v.x) + (u.y - v.y);
        e.distance = sqrt(pow((u.x - v.x), 2) + pow((u.y - v.y), 2));
        graph_container[v].insert(e);
        edge f{};
        f.start = v;
        f.end = u;
        f.difference = (v.x - u.x) + (v.y - u.y);
        f.distance = sqrt(pow((v.x - u.x), 2) + pow((v.y - u.y), 2));
        graph_container[u].insert(f);
    }

};


void calc_force(graph &g) {
    std::map<node, std::set<edge>> map = g.graph_container;

    for (auto it : map) {
        float coulomb = 0.0;
        float hook = 0.0;
        for (auto &it2 : it.second) {
            coulomb += (beta / pow(it2.distance, 2)) * (it2.difference / it2.distance);
            hook += -1 * alpha * (it2.distance - 1.0) * (it2.difference / it2.distance);
        }
        node t = it.first;
        std::set<edge> e = it.second;
        t.force = coulomb + hook;
        map.erase(it.first);
        map[t] = e;
    }


}


int main(int argc, char **argv) {

    std::string line, file;
    std::string::size_type sz;

    int frames = 0, nodes = 0, i = 0, j = 0;

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
    return 0;
}