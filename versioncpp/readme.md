Avant de compiler, choisir le bon chemin pour le fichier contenant le graph

- compile

g++ -c board_rpp.cpp parsegraph.cpp -std=c++17
g++ -o target board_rpp.o parsegraph.o

- run 

./target