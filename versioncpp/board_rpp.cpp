#include <random>
#include "parsegraph.hpp"
#include <set>
#include <iostream>
#include <stdlib.h>


using namespace std;


const int MaxPlayoutLength = 10000;
const bool closed = true;
Graph g1 = read_graph("/Users/swarm/Documents/stage/URPP/UR532");
RppGraph g = RppGraph(g1.edges, g1.nb_vertices, g1.nb_required);
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
    RppGraph graph;
    set<Edge> required_edges;
    int cost;
    int current_node;
    

    Board () {

        length = 0;
        cost = 0;
        graph = g;
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
        // first move
        if (length == 0){
            current_node = random()%graph.nb_vertices;
            //cout << "first node = " << current_node << endl;
        }
        int c = 0;
        for (int i = 0; i < graph.nb_vertices; i++){
            if (graph.adj_mat_induced[current_node][i].u != -1 && current_node != i){
                moves[c] = Move(current_node, i);
                c++;
            }
        }
        return c;
    }

    // play the move, update gamestate
    void play (Move m) {
        rollout[length] = m;

        if (graph.adj_mat_induced[m.u][m.v].required){
            cost += graph.adj_mat_induced[m.u][m.v].w;

            required_edges.insert(graph.adj_mat_induced[m.u][m.v]);
            graph.adj_mat_induced[m.u][m.v].required = false;
            required_edges.insert(graph.adj_mat_induced[m.v][m.u]);
            graph.adj_mat_induced[m.v][m.u].required = false;
        } else {
            for (auto elem : graph.adj_mat_induced[m.u][m.v].required_edges){
                required_edges.insert(elem);
            }
            cost += graph.adj_mat_induced[m.u][m.v].shortest;
        }
        current_node = m.v;
        length++;
    }

    bool terminal () {
        //cout << "terminal?" << endl;
        if (required_edges.size() == 2*graph.nb_required){
            //cout << "parcours termine" << endl;
        } else if (length >= MaxPlayoutLength){
            //cout << "pas fini" << endl;
        }
        return required_edges.size() == 2*graph.nb_required || length >= MaxPlayoutLength;
    }
  
    double score () {
        if (!closed){
            return -cost; 
        } else{
            return -cost - graph.adj_mat_induced[rollout[length-1].v][rollout[0].u].shortest;
        }
    }

    void print (FILE *fp) {
        fprintf (fp, "length = %d, cost = %d, required edges %d/%d\n", length, cost, (int) required_edges.size()/2, graph.nb_required);
    }
};

#include "gnrpa.c"

int main(int argc, char *argv []) {
    srandom(time(NULL));
  //testTimeNRPA (4, "leftMove");
  //exit (0);
    cout << "Debut du programme main" << endl;

    while (true) {
        int level = 3;
        nrpa (level, true);
        fprintf (stderr, "score final : ");
        bestBoard.print (stderr);
        fprintf (stderr, "\n");
    }
    return 0;
}
