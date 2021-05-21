#pragma once
#include "parsegraph.hpp"
#include <vector>

using namespace std;

class GameResult{
    public:
    int score;
    vector<Edge> tour;
    GameResult(int s, vector<Edge> t){
        score = s;
        tour = t;
    }
    // methods

    void print_result(bool print_tour){

        cout << "Cost of tour = " << score << endl;
        cout << "Length of tour = " << tour.size() << endl;
        if (print_tour){
            cout << "\nTour found :" << endl;
            for (int i = 0; i < tour.size(); i++){
                cout << tour[i].u << " -> ";
            }
        }
    }
};

GameResult random_tour(Graph graph);