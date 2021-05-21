#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>

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
        }
        vector<Edge> edges;
        int nb_vertices, nb_required;
        map<int, vector<Edge> > adj_list;

        // methods

        void make_adj_list(){

            for (int i = 0; i < edges.size(); i++){
                adj_list[edges[i].u].push_back(edges[i]);
                adj_list[edges[i].v].push_back(edges[i].reverse());
            }
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
};

Graph read_graph(string path);
