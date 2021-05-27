#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>


#define MAXINT 2147483647

using namespace std;


class Edge{
    public:
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
            compute_shortest_path();
        }
        vector<Edge> edges;
        int nb_vertices, nb_required;
        map<int, vector<Edge> > adj_list;
        vector<vector<int> > shortest_path;
        vector<vector<int> > adj_mat;

        // methods

        void make_adj_list(){

            for (int i = 0; i < edges.size(); i++){
                adj_list[edges[i].u].push_back(edges[i]);
                adj_list[edges[i].v].push_back(edges[i].reverse());
            }
        }

        void make_adj_mat(){
            // init
            for (int i = 0; i < nb_vertices; i++){
                adj_mat.push_back(vector<int>());
                for (int j = 0; j < nb_vertices; j++){
                    adj_mat[i].push_back(0);
                }
            }
            // fill
            for (int i = 0; i < edges.size(); i++){
                adj_mat[edges[i].u][edges[i].v] = edges[i].w;
            }

        }
        

        void compute_shortest_path(){
            // floyd warshall algorithm

            // init
            vector<vector<vector<int> > > ws;
            vector<vector<int> > w0;
            ws.push_back(w0);

            for (int i = 0; i < nb_vertices; i++){
                w0.push_back(vector<int>());
                for (int j = 0; j < nb_vertices; j++){
                    if (i == j){
                        w0[i].push_back(0);
                    } else if (adj_mat[i][j]) {
                        w0[i].push_back(adj_mat[i][j]);
                    } else{
                        w0[i].push_back(MAXINT);
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
