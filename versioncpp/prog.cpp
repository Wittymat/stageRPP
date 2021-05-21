#include "parsegraph.hpp"
#include "rppgame.hpp"
#include <functional>
#include <cstdlib>

using namespace std;

int main(){
    srand(time(NULL));
    // make Graph
    Graph g = read_graph("/Users/swarm/Documents/stage/URPP/UR132");
    g.print_graph(false);

    // random tour
    GameResult gr = random_tour(g);
    gr.print_result(false);

    return 0;
}

