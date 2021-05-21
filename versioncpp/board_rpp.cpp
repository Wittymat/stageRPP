#include <vector>
#include "gnrpa.cpp"

using namespace std;

const int MaxLegalMoves = 2;
const int MaxPlayoutLength = 101;
const int MaxMoveNumber = 202;

typedef int Move;

class Board {
    public:

    Move rollout [MaxPlayoutLength];
    int length;
    int nbMovesLeft;
    int maxi;

    Board (int m = 100) {
        length = 0;
        maxi = m;
        nbMovesLeft = 0;
    }

    int code (Move m) {
        return m + length * 2;
    }
  
    int code (Move m, float b = 0.0) {
        return m + length * 2;
    }
  
    int legalMoves (Move moves [MaxLegalMoves]) {
        current_node = rollout[nbMovesLeft-1]

    }

  void play (Move m) {
    if (m == 0){
        nbMovesLeft++;
    }
    rollout [length] = m;
    length++;
  }

    bool terminal () {
    return length == maxi;
    }
  
    double score () {
    return nbMovesLeft;
    }

    void print (FILE *fp) {
    fprintf (fp, "nbMovesLeft = %d, length = %d\n", nbMovesLeft, length);
    }
};


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
