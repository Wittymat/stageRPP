#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>


#define MAXINT 2147483647

using namespace std;


class Edge{
    public:
        Edge(){}
        Edge(int a, int b, int c, bool d){
            u = a;
            v = b;
            w = c;
            required = d;
        }
        int u, v, w;
        bool required;

        // methods
        Edge reverse(){
            return Edge(v, u, w, required);
        }
        bool operator<(const Edge & a) const {
            return a.u > u || (a.u == u && a.v > v);
        }

        
};



class DoubleEdge : public Edge {
    public :
    int shortest;
    bool double_edge;
    int i;
    int j;
    set<Edge> required_edges;
    DoubleEdge(int a, int b, int c, bool d, int s, int e, int u, int v) : Edge(a, b, c, d){
        shortest = s;
        double_edge = e;
        i = u;
        j = v;
    }
};

ostream& operator<< (ostream &os, DoubleEdge const& e);

class Path {
    public:
    vector<Edge> path;
    Path();
    bool is_closed();
};



class Graph {
    public:
        // default constructor
        Graph(){
            nb_vertices = 0;
            nb_required = 0;
        }
        // constructor
        Graph(vector<Edge> e, int n, int r){
            edges = e;
            nb_vertices = n;
            nb_required = r;
            make_adj_list();
            make_adj_mat();
            //compute_shortest_path();
        }

        vector<Edge> edges;
        int nb_vertices, nb_required;
        map<int, vector<Edge> > adj_list;
        vector<vector<int> > shortest_path;
        vector<vector <Edge> > adj_mat;

        // methods

        void make_adj_list(){

            for (int i = 0; i < edges.size(); i++){
                adj_list[edges[i].u].push_back(edges[i]);
                adj_list[edges[i].v].push_back(edges[i].reverse());
            }
            cout << "adj list faite" << endl;
        }

        void make_adj_mat(){
            // init
            for (int i = 0; i < nb_vertices; i++){
                adj_mat.push_back(vector<Edge>());
                for (int j = 0; j < nb_vertices; j++){
                    adj_mat[i].push_back(Edge(-1, -1, 0, false));
                }
            }
            // fill
            for (int i = 0; i < edges.size(); i++){
                adj_mat[edges[i].u][edges[i].v] = edges[i];
                adj_mat[edges[i].v][edges[i].u] = edges[i].reverse();

            }
            cout << "adj mat faite" << endl;
        }

        

        void compute_shortest_path(){
            // floyd warshall algorithm

            // init
            vector<vector<vector<int> > > ws;
            ws.push_back(vector < vector < int> >());

            for (int i = 0; i < nb_vertices; i++){
                ws[0].push_back(vector<int>());
                for (int j = 0; j < nb_vertices; j++){
                    if (i == j){
                        ws[0][i].push_back(0);
                    } else if (adj_mat[i][j].u != -1) { // non existing edge
                        ws[0][i].push_back(adj_mat[i][j].w);
                    } else{
                        ws[0][i].push_back(MAXINT);
                    }
                }
            }


            // recurrence
            for (int k = 1; k<nb_vertices; k++){
                ws.push_back(vector<vector<int> >());
                for (int i = 0; i < nb_vertices; i++){
                    ws[k].push_back(vector<int>());
                    for (int j = 0; j < nb_vertices; j++){
                        int n = min(ws[k-1][i][j], ws[k-1][i][k] + ws[k-1][k][j]);
                        ws[k][i].push_back(n);
                    }
                }
            }
            shortest_path = ws[ws.size()-1];
            cout << "shortest pass fait" << endl;
        }

        void print_graph(bool edges){
            cout << "Number vertices : " << nb_vertices << endl;
            cout << "Number edges required : " << nb_required << endl;
            if (edges){
                cout << "\nList of edges :" << endl;
                for (auto const & [key, val] : adj_list){
                    cout << "Edges of " << key << " : " << endl;
                    for (int i = 0; i < val.size(); i++){
                        cout << val[i].v << " ";
                    }
                    cout << endl;
                }
            }
        }

        bool has_required_edge(int e[2]){
            for (int i = 0; i < adj_list[e[0]].size(); i++){
                if (adj_list[e[0]][i].v == e[1] && adj_list[e[0]][i].required){
                    return true;
                }
            }
            return false;
        }
};

Graph read_graph(string path);


class RppGraph : public Graph {
    public:

    vector<vector<int> > paths;
    vector<vector<DoubleEdge> > adj_mat_induced;
    RppGraph(){}
    RppGraph(vector<Edge> e, int n, int r) : Graph(e, n, r){
        cout << "debut construction graph induit " << endl;
        vector<int> required_nodes(nb_vertices, -1);
        int c = 0;
        for (int i = 0; i < edges.size(); i++){
            Edge current_edge = edges[i];
            if (current_edge.required){
                if (required_nodes[current_edge.u] == -1){
                    required_nodes[current_edge.u] = c;
                    c++;
                }
                if (required_nodes[current_edge.v] == -1){
                    required_nodes[current_edge.v] = c;
                    c++;
                }
            }
        }
        cout << "nombre noeuds induits = " << c << endl;
        vector<int> nodes_required;
        for (int i = 0; i<nb_vertices; i++){
            if (required_nodes[i] != -1){
                nodes_required.push_back(i);
            }
        }

        // shortest path matrix
        vector <vector<int> > shortest;
        vector <vector<int> > previous;
        // init matrix
        for (int i = 0; i < nodes_required.size(); i++){
            shortest.push_back(vector<int>());
            previous.push_back(vector<int>());
            for (int j = 0; j < nb_vertices; j++){
                if (nodes_required[i] == j){
                    shortest[i].push_back(0);
                } else{
                    shortest[i].push_back(MAXINT);
                }
                previous[i].push_back(-1);
            }
        }
        // multiple dijkstra
        set<int> finish;
        for (int i = 0; i < nodes_required.size(); i++){
            int current_node = nodes_required[i];
            finish.clear();
            while (finish.size() != nb_vertices){
                // select min
                int dmin = MAXINT;
                int indmin = -1;

                for (int k = 0; k < nb_vertices; k++){

                    if (shortest[i][k] < dmin && finish.count(k) == 0) {
                        dmin = shortest[i][k];
                        indmin = k;
                    }
                }
                finish.insert(indmin);
                // neighbors
                for (int b = 0; b < nb_vertices; b++){
                    if (adj_mat[indmin][b].u != -1 && finish.count(b) == 0){
                        if (shortest[i][b] > shortest[i][indmin] + adj_mat[indmin][b].w){
                            shortest[i][b] = shortest[i][indmin] + adj_mat[indmin][b].w;
                            previous[i][b] = indmin;
                        }
                    }
                }
            }
        }

        // construire la matrice d'adjacence du graphe induit par les arretes requises
        for (int i = 0; i < c; i++){
            adj_mat_induced.push_back(vector<DoubleEdge>());
            for (int j = 0; j < c; j++){
                int u = nodes_required[i];
                int v = nodes_required[j];
                bool e = (adj_mat[u][v].u == -1);
                adj_mat_induced[i].push_back(DoubleEdge(i, j, adj_mat[u][v].w, adj_mat[u][v].required, shortest[i][v], e, u, v));
                // un noeud ne doit pas aller vers lui même
                if (i == j){
                    adj_mat_induced[i][j].u = -1;
                    adj_mat_induced[i][j].v = -1;
                }
            }
        }

        // si le chemin d'une arrete u-v dans le graphe induit passe par un required dans le graphe normal, ajouter cette info
        for (int i = 0; i < c; i++){
            for (int j = 0; j < c ; j++){
                int src = nodes_required[i];
                int dst = nodes_required[j];
                while (dst != src){
                    int u = previous[i][dst];
                    // s'il y a un edge requis, l'ajouter
                    if (adj_mat[u][dst].u != -1 && adj_mat[u][dst].required){
                        adj_mat_induced[i][j].required_edges.insert(adj_mat[u][dst]);
                        adj_mat_induced[i][j].required_edges.insert(adj_mat[dst][u]);
                    }
                    dst = u;
                }
            }
        }

        nb_vertices = c;
        paths = previous;
        cout << "graphe induit construit " << endl;
        //print_graph();
    }

    void print_graph(){
        for (int i = 0; i < nb_vertices; i++){
            for (int j = 0; j < nb_vertices; j++){
                cout << adj_mat_induced[i][j] << endl;
            }
        }
    }
};