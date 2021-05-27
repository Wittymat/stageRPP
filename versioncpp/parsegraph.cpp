#include "parsegraph.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <map>

using namespace std;

vector<string> split(string const& s, char d){
    vector<string> tokens;
    string token = "";
    for (int i = 0; i < s.size(); i++){
        if (s[i] == d){
            if (token != "") {
                tokens.push_back(token);
            }
            token = "";
        } else {
            token.push_back(s[i]);
        }
    }
    if (token != ""){
        tokens.push_back(token);
    }
    return tokens;
}

Graph read_graph(string path){
    // ouverture fichier
    ifstream f;
    f.open(path);
    // declaration
    int nb_vertices, nb_required, nb_edges, u, v, w;
    vector<Edge> list_edges;
    // parsing
    int i = 0;
    string line;

    while (getline(f, line)){
        // nombre sommet
        if (i == 2){

            nb_vertices = stoi(split(line, ' ')[2]);

        } else if (i == 3){
            nb_required = stoi(split(line, ' ')[2]);
        } else if (i == 4){
            nb_edges = nb_required + stoi(split(line, ' ')[2]);
        } else if ((i >= 6 && i < 6 + nb_required) || (i > 6 + nb_required && i < 6 + nb_edges)){
            vector<string> tokens = split(line, ' ');
            tokens[1].pop_back();
            u = stoi(tokens[1]);
            tokens[2].pop_back();
            v = stoi(tokens[2]);
            w = stoi(tokens[4]);
            list_edges.push_back(Edge(u, v, w, i < 6 + nb_required));
        }
        i++;
    }

    f.close();
    return Graph(list_edges, nb_vertices, nb_required);
}