#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <map>

struct node {
    int x, y;
};

class graph {
private:
    std::map<node, std::set<node>> graph_container;
public:
    void add_node(const node &v) {
        std::set<node> new_set;
        graph_container[v] = new_set;
    }

    void add_edge(const node &v, const node &u) {
        graph_container[v].insert(u);
        graph_container[u].insert(v);
    }
};

int main(int argc, char **argv) {

    std::string line, file;
    int frames = 0, nodes = 0, i = 0;

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
                frames = atoi(line);
            } else if (i == 1) {
                nodes = atoi(line);
            } else if (i < nodes + 1) {
                node n;

            }

            i++;

        }
        myfile.close();
    } else std::cout << "Unable to open file";


    return 0;
}