#include "parsegraph.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <map>

using namespace std;


ostream& operator<< (ostream &os, Edge const& e){
    os << "u = " << e.u << " v = " << e.v << " required = " << e.required  << " weight = " << e.w << endl;
    return os;
}
ostream& operator<< (ostream &os, DoubleEdge const& e){
    if (e.u != -1){
        os << "u = " << e.u << " v = " << e.v << " required = " << e.required  << " weight = " << e.w;
        os << " nouveau poids = " << e.shortest << " ancien noms sommets = (" << e.i << "," << e.j << ") " << endl;
        os << "liste edges required dans le chemin : " << endl;
        for (auto elem : e.required_edges){
            os << "(" << elem.u << "," << elem.v << ")" << endl;
        }
    }
    return os;
}

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


bool Path::is_closed(){
    if (path.size() <= 1){
        return true;
    }
    return path[0].u == path[path.size()-1].v;
}



Graph read_graph(string path){
    cout << "ouverture fichier" << endl;
    // ouverture fichier
    ifstream f;
    f.open(path);
    cout << "fichier ouvert" << endl;
    // declaration
    int nb_vertices, nb_required, nb_edges, u, v, w;
    vector<Edge> list_edges;
    // parsing
    int i = 0;
    string line;

    while (getline(f, line)){
        if (i == 2){
            nb_vertices = stoi(split(line, ' ')[2]);
        } else if (i == 3){
            nb_required = stoi(split(line, ' ')[2]);
        } else if (i == 4){
            nb_edges = nb_required + stoi(split(line, ' ')[2]);
        } else if ((i >= 6 && i < 6 + nb_required) || (i > 6 + nb_required && i < 6 + nb_edges + 1)){
            vector<string> tokens = split(line, ' ');
            tokens[1].pop_back();
            u = stoi(tokens[1]);
            tokens[2].pop_back();
            v = stoi(tokens[2]);
            w = stoi(tokens[4]);
            list_edges.push_back(Edge(u-1, v-1, w, i < 6 + nb_required));
        }
        i++;
    }

    f.close();
    cout << "lecture graph termine" << endl;
    cout << "nb vertices " << nb_vertices << endl;
    cout << "nb required " << nb_required << endl;
    cout << "nb edges " << nb_edges << endl;

    return Graph(list_edges, nb_vertices, nb_required);
}

