#include "rppgame.hpp"
#include <random>
#include <set>

using namespace std;

GameResult random_tour(Graph graph){
    set <Edge> required;
    int cost = 0;
    int current_node = 1 + rand()%graph.nb_vertices;
    vector<Edge> tour;
    int itermax = 1000000;
    while ((required.size() < 2*graph.nb_required || tour[0].u != tour[tour.size()-1].v) && itermax > 0){
        int nb_adj = graph.adj_list[current_node].size();
        Edge next_edge = graph.adj_list[current_node][random()%nb_adj];
        cost += next_edge.w;
        if (next_edge.required){
            required.insert(Edge(current_node, next_edge.v, 0, false));
            required.insert(Edge(next_edge.v, current_node, 0, false));
        }
        tour.push_back(Edge(current_node, next_edge.v,0, false));
        current_node = next_edge.v;
        //itermax--; // avoid infinite loop
    }
    
    return GameResult(cost, tour);
}