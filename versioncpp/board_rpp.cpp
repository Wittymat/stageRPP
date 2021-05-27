#include <vector>
#include <random>
#include "parsegraph.hpp"
#include <set>


using namespace std;

const int MaxPlayoutLength = 500000;
const int MaxLegalMoves = 1000;
const int MaxMoveNumber = 100000;
const bool closed = true;
Graph g = read_graph("/Users/swarm/Documents/stage/URPP/UR132");

typedef int Move[2];

void replace_move(Move move, Move m){
    move[0] = m[0];
    move[1] = m[1];
}

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
        current_node = random()%g.nb_vertices;
    }

    // hash function for a move
    int code (Move m) {
        return graph.nb_vertices * m[0] + m[1];
    }
    int code (Move m, int b) {
        return graph.nb_vertices * m[0] + m[1];
    }

    // return the number of legal moves, put all legal moves in moves array
    int legalMoves (Move moves [MaxLegalMoves]) {
        int nb = graph.adj_list[current_node].size();
        for (int i = 0; i < nb; i++){
            moves[i][0] = current_node;
            moves[i][1] = graph.adj_list[current_node][i].v;
        }
        return nb;
    }

    // play the move, update gamestate
    void play (Move m) {
        rollout[length][0] = m[0];
        rollout[length][1] = m[1];
        cost += graph.adj_list[current_node][m[1]].w;
        length++;
        current_node = graph.adj_list[current_node][m[1]].v;

    }

    bool terminal () {
        return required_edges.size() == 2*graph.nb_required && (!closed || rollout[0] == rollout[length]);
    }
  
    double score () {
        if (!closed){
            return -cost; 
        } else{
            return -cost - graph.shortest_path[rollout[0][1]][rollout[length][1]];
        }
    }



    // double score2 () {
    //     map<int[2], int> occurences = count_occurences(rollout, length);
    //     int p_a = 1000000;
    //     int p_b = 1000;
    //     // penalites pour chaque occurence d'un edge + de 2 fois
    //     // bonus pour chaque arrete obligatoire empruntee
    //     int penalites = 0;
    //     int bonus = 0;
    //     for (const auto& [key, value] : occurences){
    //         if (value > 2){
    //             penalites += p_a;
    //         }
    //         if (graph.has_required_edge(key)){
    //             bonus += p_b;
    //         }
    //     }
    //     return bonus - penalites;
    // }

    // map<int[2], int> count_occurences (Move rollout[], int length) {
    //     map<int[2], int> res;
    //     for (int i = 0; i < length; i++){
    //         int t[2] = {rollout[i][0], rollout[i][1]};
    //         if (res.count(t) == 0) {
    //             res.insert(std::make_pair(t, 0));
    //         } else {
    //             res[t]++;
    //         }
    //     }

    //     return res;
    // }

    void print (FILE *fp) {
        
    }
};

#include "gnrpa.cpp"

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
