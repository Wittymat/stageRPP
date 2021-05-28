#include <random>
#include "parsegraph.hpp"
#include <set>


using namespace std;

const int MaxPlayoutLength = 1000;
const bool closed = true;
Graph g = read_graph("/Users/swarm/Documents/stage/URPP/UR132");
const int MaxLegalMoves = 1000;
const int MaxMoveNumber = MaxLegalMoves*MaxLegalMoves;



class Move{
    public:
    int u;
    int v;
    Move(int x, int y){
        u = x;
        v = y;
    }
    // default constructor
    Move(){
        u = -1;
        v = -1;
    }
};


class Board {
    public:

    Move rollout [MaxPlayoutLength];
    int length;
    Graph graph;
    set<Edge> required_edges;
    int cost;
    int current_node;
    

    Board () {
        // first node to explore
        length = 0;
        cost = 0;
        graph = g;
        current_node = 0;
    }

    // hash function for a move
    int code (Move m) {
        return graph.nb_vertices * m.u + m.v;
    }
    int code (Move m, int b) {
        return graph.nb_vertices * m.u + m.v;
    }

    // return the number of legal moves, put all legal moves in moves array
    int legalMoves (Move moves [MaxLegalMoves]) {
        int nb = graph.adj_list[current_node].size();
        for (int i = 0; i < nb; i++){
            moves[i] = Move(current_node, graph.adj_list[current_node][i].v) ;
        }
        return nb;
    }

    // play the move, update gamestate
    void play (Move m) {
        rollout[length] = m;
        cost += graph.adj_list[current_node][m.v].w;
        length++;
        current_node = graph.adj_list[current_node][m.v].v;
    }

    bool terminal () {
        return required_edges.size() == 2*graph.nb_required;
    }
  
    double score () {
        if (!closed){
            return -cost; 
        } else{
            return -cost - graph.shortest_path[rollout[0].u][rollout[length].v];
        }
    }

    void print (FILE *fp) {
        
    }
};

#include "gnrpa.c"

int main(int argc, char *argv []) {
  //testTimeNRPA (4, "leftMove");
  //exit (0);
  while (true) {
    int level = 2;
    nrpa (level, true);
    fprintf (stderr, "score final : ");
    bestBoard.print (stderr);
    fprintf (stderr, "\n");
  }
}
