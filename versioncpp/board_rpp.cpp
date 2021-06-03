#include <random>
#include "parsegraph.hpp"
#include <set>
#include <iostream>


using namespace std;


const int MaxPlayoutLength = 10000;
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
            if (graph.adj_mat[current_node][i].u != -1){
                moves[c] = Move(current_node, i);
                c++;
            }
        }
        return c;
    }

    // play the move, update gamestate
    void play (Move m) {
        rollout[length] = m;
        cost += graph.adj_mat[m.u][m.v].w;
        if (g.adj_list[m.u][m.v].required){
            required_edges.insert(graph.adj_mat[m.u][m.v]);
            required_edges.insert(graph.adj_mat[m.v][m.u]);
        }
        current_node = m.v;
        length++;
    }

    bool terminal () {
        //cout << "terminal?" << endl;
        if (required_edges.size() == 2*graph.nb_required){
            cout << "parcours termine" << endl;
        } else if (length >= MaxPlayoutLength){
            cout << "pas fini" << endl;
        }
        return required_edges.size() == 2*graph.nb_required || length >= MaxPlayoutLength;
    }
  
    double score () {
        if (!closed){
            return -cost; 
        } else{
            return -cost - graph.shortest_path[rollout[0].u][rollout[length].v];
        }
    }

    void print (FILE *fp) {
        fprintf (fp, "length = %d\n", length);
    }
};

#include "gnrpa.c"

int main(int argc, char *argv []) {
  //testTimeNRPA (4, "leftMove");
  //exit (0);
    cout << "Debut du programme main" << endl;

    while (true) {
        int level = 0;
        nrpa (level, true);
        fprintf (stderr, "score final : ");
        bestBoard.print (stderr);
        fprintf (stderr, "\n");
    }
}
