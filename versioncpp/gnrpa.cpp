#include <float.h>

float ALPHA = 1.0;
int N = 100;
//int N [10] = {20, 20, 20, 20, 20, 20, 20, 20, 50, 50};

bool reinit = false;
float regul = 0.0;
float r = 0.0;
float dropout = 0.0;
bool readapt = false;
float k = 0.0;
int P = 1;
float temperature = 1.0;

const int MaxLevel = 6;

float scoreBestRollout [10];
int lengthBestRollout [10];
Move bestRollout [10] [MaxPlayoutLength];
int indexBestMoveRollout [10] [MaxPlayoutLength];
int nbMovesBestRollout [10] [MaxPlayoutLength];
int codeBestRollout [10] [MaxPlayoutLength] [MaxLegalMoves];
float betaBestRollout [10] [MaxPlayoutLength] [MaxLegalMoves];

Board bestBoard;

clock_t startClockNRPA, stopClockNRPA;
float bestScoreNRPA = -DBL_MAX;
float nextTimeNRPA = 0.01;
int indexTimeNRPA;
int indexSearch;
float valueAfterTimeNRPA [10000] [100];
float sumValueAfterTimeNRPA [100];
int nbSearchTimeNRPA [100];

int nbTimesNRPA = 14;
bool stopOnTime = false;
float firstTimeNRPA = 0.01;
int nbSearchesNRPA = 200;

int SizeBeam [MaxLevel] = {1, 1, 1, 1, 1, 1};
int startLearning = 0;
int printLevel = 2;
float constante = 0.4;
float kAMAF = 1.0;
float minNorm = -0.1, maxNorm = 2.0;

#include <vector>

//static union{float d; struct{int j, i;} n;} e;
//#define A (1048576/M_LN2)
//#define C 60801
//#define expon(y) (e.n.i=A*(y)+(1072693248-C),e.d)

float expon (float x) {
  return exp (x);
}

/*
float expon (float x) {
   union { float f; int i; } y;
   y.i = (int)(x * 0xB5645F + 0x3F7893F5);
   return (y.f);
}
*/
  
using namespace std;

float probaMove [10] [MaxPlayoutLength] [MaxLegalMoves];
static float polp [MaxMoveNumber];
static float polLevel [10] [MaxMoveNumber];

float playoutNRPA (Board & board) {
  int nbMoves = 0;
  Move moves [MaxLegalMoves];
 
  while (true) {
    if (board.terminal ()) {
      float score = board.score ();
      scoreBestRollout [0] = score;
      lengthBestRollout [0] = board.length;
      for (int k = 0; k < board.length; k++)
        replace_move(bestRollout[0][k], board.rollout[k]);
	      //bestRollout [0] [k] = board.rollout [k];

      //for (int k = 0; k < board.length; k++)
      //policyAMAF.add (board.code (board.rollout [k]), score);
      if (score > bestScoreNRPA) {
	bestScoreNRPA = score;
	bestBoard = board;
	board.print (stderr);
      }
      stopClockNRPA = clock ();
      float time = ((float)(stopClockNRPA - startClockNRPA)) / CLOCKS_PER_SEC;
      if (time > nextTimeNRPA) {
	while (time > 2 * nextTimeNRPA) {
	  indexTimeNRPA++;
	  nextTimeNRPA *= 2;
	}
	valueAfterTimeNRPA [indexSearch] [indexTimeNRPA] = bestScoreNRPA;
	sumValueAfterTimeNRPA [indexTimeNRPA] += bestScoreNRPA;
	nbSearchTimeNRPA [indexTimeNRPA]++;
	indexTimeNRPA++;
	nextTimeNRPA *= 2;
      }
      return score;
    }
    nbMoves = board.legalMoves (moves);
    nbMovesBestRollout [0] [board.length] = nbMoves;
    for (int i = 0; i < nbMoves; i++) {
      float b = 0.0;
      int c = board.code (moves [i], b);
      codeBestRollout [0] [board.length] [i] = c;
      betaBestRollout [0] [board.length] [i] = b;
      probaMove [0] [board.length] [i] = expon (polLevel [1] [c] / temperature + b);
    }
	
    float sum = probaMove [0] [board.length] [0];
    for (int i = 1; i < nbMoves; i++) 
      sum += probaMove [0] [board.length] [i];
    float r = (rand () / (RAND_MAX + 1.0)) * sum;
    int j = 0;
    float s = probaMove [0] [board.length] [0];
    while ((s < r) && (j < nbMoves - 1)) { 
      j++;
      s += probaMove [0] [board.length] [j];
    }
    indexBestMoveRollout [0] [board.length] = j;
    board.play (moves [j]);
  }
  return 0.0;  
}


void adaptLevel (int length, int level) {
  for (int i = 0; i < length; i++) {
    for (int j = 0; j < nbMovesBestRollout [level] [i]; j++) {
      polp [codeBestRollout [level] [i] [j]] = polLevel [level] [codeBestRollout [level] [i] [j]];
    }
  }
  for (int i = 0; i < length; i++) {
    int b = indexBestMoveRollout [level] [i];
    float z = 0.0;
    for (int j = 0; j < nbMovesBestRollout [level] [i]; j++) {
      float b = betaBestRollout [level] [i] [j];
      int c = codeBestRollout [level] [i] [j];
      probaMove [level] [i] [j] = expon (polLevel [level] [c] / temperature + b);
      z += probaMove [level] [i] [j];
    }
    float a = ALPHA;
    polp [codeBestRollout [level] [i] [b]] += (a / temperature);
    for (int j = 0; j < nbMovesBestRollout [level] [i]; j++)
      polp [codeBestRollout [level] [i] [j]] -= (a / temperature) * probaMove [level] [i] [j] / z;
  }
  for (int i = 0; i < length; i++) {
    for (int j = 0; j < nbMovesBestRollout [level] [i]; j++) {
      polLevel [level] [codeBestRollout [level] [i] [j]] = polp [codeBestRollout [level] [i] [j]];
    }
  }
  //pol.regularize ();
}
/**/

float alpha = 1.0;

float getAlpha (int last, int current) {
  // return 1.0; // nrpa
  // if (last == current) // edelkamp
  // return 1.0;
  // else
  // return 0.0;
  return alpha * expon (-k * (current - last));
}

int stepCopyPolicy = 0;

float nrpa(int level, bool root = false) {
  scoreBestRollout [level] = -DBL_MAX;
  if (level == 0) {
    //polLevel [level].add (pol);
    //return playoutNRPA (polLevel [level]);
    Board b; // = binit;
    return playoutNRPA (b);
  }
  else {
    if (!root)
      for (int i = 0; i < MaxMoveNumber; i++)
	polLevel [level] [i] = polLevel [level + 1] [i];
    int last = 0;
    int n = N;
    int Level = 1;
    if (level <= Level)
      n = P * N;
    for (int i = 0; i < n; i++) {
      float score = nrpa (level - 1);
      if (score >= scoreBestRollout [level]) {
	last = i;
	scoreBestRollout [level] = score;
	lengthBestRollout [level] = lengthBestRollout [level - 1];
	for (int k = 0; k < lengthBestRollout [level]; k++)
    replace_move(bestRollout[level][k], bestRollout[level-1][k]);
	  //bestRollout [level] [k] = bestRollout [level - 1] [k];
	for (int k = 0; k < lengthBestRollout [level]; k++) {
	  indexBestMoveRollout [level] [k] = indexBestMoveRollout [level - 1] [k];
	  nbMovesBestRollout [level] [k] = nbMovesBestRollout [level - 1] [k];
	  for (int l = 0; l < nbMovesBestRollout [level - 1] [k]; l++) {
	    codeBestRollout [level] [k] [l] = codeBestRollout [level - 1] [k] [l];
	    betaBestRollout [level] [k] [l] = betaBestRollout [level - 1] [k] [l];
	  }
	} 
	if (level > printLevel) {
	  for (int t = 0; t < level - 1; t++)
	    fprintf (stderr, "\t");
	  fprintf(stderr,"Level : %d, N:%d, score : %f\n", level, i, score);
	}
      }
      if ((level > Level) || ((level <= Level) && (i % P == P - 1))) {
	adaptLevel (lengthBestRollout [level], level);
      }
      if (stopOnTime && (indexTimeNRPA > nbTimesNRPA))
	return scoreBestRollout [level];
    }
    return scoreBestRollout [level];
  }
}

void writeAllValues (int index, const char * prefix) {
  char s [1000];
  //sprintf (s, "NRPA.time.nbThreads=%d.nbSearches=%d.k=%2.3f.plot", nbThreads, nbSearchesNRPA, k);
  sprintf (s, "%s.AllValues.nbTimes.%d.nbSearches.%d.plot", prefix, nbTimesNRPA, nbSearchesNRPA);
  FILE * fp = fopen (s, "w");
  if (fp != NULL) {
    fprintf (fp, "%d\n", index + 1);
    fprintf (fp, "%d\n", nbTimesNRPA);
    for (int l = 0; l < index + 1; l++) {
      float t = firstTimeNRPA;
      for (int j = 0; j <= nbTimesNRPA; j++) {
	fprintf (fp, "%f %f\n", t, valueAfterTimeNRPA [l] [j]);
	t *= 2;
      }
    }
  }
  fclose (fp); 
}

void writeValues (int nbThreads, const char * prefix) {
  char s [1000];
  //sprintf (s, "NRPA.time.nbThreads=%d.nbSearches=%d.k=%2.3f.plot", nbThreads, nbSearchesNRPA, k);
  sprintf (s, "%s.nbThreads=%d.nbSearches=%d.plot", prefix, nbThreads, nbSearchesNRPA);
  FILE * fp = fopen (s, "w");
  if (fp != NULL) {
    fprintf (fp, "# %d searches\n", indexSearch + 1);
    float t = firstTimeNRPA;
    for (int j = 0; j <= nbTimesNRPA; j++) {
      float sum = 0.0;
      int nbValues = 0;
      for (int l = 0; l < indexSearch + 1; l = l + nbThreads)
	if (l + nbThreads - 1 < indexSearch + 1) {
	  float maxi = -DBL_MAX;
	  for (int m = 0; m < nbThreads; m++)
	    if (valueAfterTimeNRPA [l + m] [j] > maxi)
	      maxi = valueAfterTimeNRPA [l + m] [j];
	  sum += maxi;
	  nbValues++;
	}
      fprintf (fp, "%f %f\n", t, sum / nbValues);
      t *= 2;
    }
  }
  fclose (fp); 
}

void testTimeNRPA (int level, const char * prefix) {
  char s [1000];
  for (int i = 0; i < 100; i++) {
    for (int j = 0; j < 10000; j++)
      valueAfterTimeNRPA [j] [i] = 0.0;
    sumValueAfterTimeNRPA [i] = 0.0;
    nbSearchTimeNRPA [i] = 0;
  }
  stopOnTime = true;
  for (indexSearch = 0; indexSearch < nbSearchesNRPA; indexSearch++) {
    bestScoreNRPA = -DBL_MAX;
    nextTimeNRPA = firstTimeNRPA;
    indexTimeNRPA = 0;
    startClockNRPA = clock ();
    while (true) {
      //initPolicy (level);
      nrpa (level, true);
      if (indexTimeNRPA > nbTimesNRPA)
	break;
    }
    float t = firstTimeNRPA;
    fprintf (stderr, "level %d, iteration %d\n", level, indexSearch + 1);
    for (int j = 0; j <= nbTimesNRPA; j++) {
      if (nbSearchTimeNRPA [j] >= (7 * (indexSearch + 1)) / 10)
	fprintf (stderr, "%f %f\n", t, sumValueAfterTimeNRPA [j] / nbSearchTimeNRPA [j]);
      t *= 2;
    }
    writeAllValues (indexSearch, prefix);
    //writeValues (1, prefix);
    //writeValues (2);
    //writeValues (4);
    //writeValues (8);
    //writeValues (16);
  }
  /*
  sprintf (s, "NRPA.time.Level=%d.nbSearches=%d.k=%2.3f.alpha=%2.2f.N=%d.plot", level, nbSearchesNRPA, k, alpha, N);
  FILE * fp = fopen (s, "w");
  if (fp != NULL) {
    fprintf (fp, "# %d searches\n", nbSearchesNRPA);
    float t = firstTimeNRPA;
    for (int j = 0; j <= nbTimesNRPA; j++) {
      if (nbSearchTimeNRPA [j] >= (7 * nbSearchesNRPA) / 10)
	fprintf (fp, "%f %f\n", t, sumValueAfterTimeNRPA [j] / nbSearchTimeNRPA [j]);
      t *= 2;
    }
  }
  fclose (fp);
  */
}

