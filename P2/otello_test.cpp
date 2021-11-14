#include "minimax.h"
#include "alphabeta.h"
#include <string.h>
#include <stdio.h>
#include <ctime>
#include <numeric>
#include <iomanip>

// The rounds to play for statistics
#define ROUNDS 500
// Set to false, to Use the minimax algorithm.
#define PRUNE true
// Set to true, to manually compare each move minimax
// and alphabeta are generating to see, that they are equal
#define COMPARE false

using namespace std;

// This is a global variable, since it might have to be used by heuristic, too.
unsigned chosen_depth=3;

int main(int argc,char *argv[]) {
  bool rand_mode;
  if (argc==1) {
    rand_mode = false;
  } else if (argc == 2) {
    if (strcmp(argv[1], "BOB_RAND") == 0) {
      srand(time(0));
      rand_mode = true;
    } else {
      cerr << "Unknown option '" << argv[1] << "'!" << endl;
      cerr << "You can use 'BOB_RAND'or no option." << endl;
      exit(1);
  }
  } else {
    cerr << "To many arguments!" << endl;
    cerr << "The scripts only accepts one additional argument." << endl;
    exit(1);
  }

  unsigned wins_bob = 0, wins_alice = 0, scores_bob[ROUNDS], scores_alice[ROUNDS];

  cout << "Starting gameplay..." << endl;
  for (unsigned round = 0; round < ROUNDS; round++) {
    // The default constructor initializes the board to the state marked by the rules.
    Board b;
    if (DEB_2) {
      cout << "Initial board:\n";
      b.Show(cout);
    }


    // By conventions, Alice starts the game.
    Player pl=Alice;

    // Temporary variables used inside the loop.
    Move mv;
    float score;

    // Variables to store the final result, when the game is finished.
    Player winner;
    unsigned winner_score,looser_score;

    // If the game has not fnished (i.e.: at least one of the players can make a move) 
    while (!b.GameOver(winner,winner_score,looser_score)) {
      if (rand_mode) {
        // When it's Alices turn, the algorithm is used
        if (Name(pl) == "Alice" ) {
          // Minimax chooses a move (it will be stored in mv) and returns the score of that move.
          if (!PRUNE || COMPARE) {
            score = Minimax(pl,b,chosen_depth,true,mv);
            if (COMPARE) {
              cout << "==== Minimax Move and Score ===" << endl;
              cout << Name(pl) << " plays. The move is " << "(" << char(mv.second.first) << "," << char(mv.second.second) << ") with a score of ";
              if (score>1E30)
                cout << "+Infinity\n";
              else if (score<-1E30)
                cout << "-Infinity\n";
              else 
                cout << score << ".\n";
            }
          } else if (PRUNE || COMPARE) {
            score = AlphaBeta(pl, b, chosen_depth, true, mv, -FLT_MAX, FLT_MAX);
            if (COMPARE) {
              cout << "==== AlphaBeta Move and Score ===" << endl;
              cout << Name(pl) << " plays. The move is " << "(" << char(mv.second.first) << "," << char(mv.second.second) << ") with a score of ";
              if (score>1E30)
                cout << "+Infinity\n";
              else if (score<-1E30)
                cout << "-Infinity\n";
              else 
                cout << score << ".\n";
            }
          }

        // And when it's Bobs turn, he chooses a random valid move
        } else {
          Sqlist moves = b.ValidMoves(pl);
          // When there are valid moves, we choose one
          if (!moves.empty()) {
            Sqlist::iterator iter = moves.begin();
            // We sample one move out of our list, if it has more than one element
            if (moves.size() > 1) {
              unsigned i = 0, pos = rand() % (moves.size()-1);

              while (i != pos) {
                iter++;
                i++;
              }
              score = 0.0;
              mv = make_pair(pl, (*iter));
            // In other case, we choose the only element in the list.
            } else {
              score = 0.0;
              mv = make_pair(pl, (*iter));
            }
          // When the list is empty, we have to pass
          } else {
            score = 0.0;
            mv = make_pair(pl, pass);
          }
        }
      } else {
        score = Minimax(pl,b,chosen_depth,true,mv);
        if (DEB_2) {
          cout << "==== Minimax Move and Score ===" << endl;
          cout << Name(pl) << " plays. The move is " << "(" << char(mv.second.first) << "," << char(mv.second.second) << ") with a score of ";
          if (score>1E30)
            cout << "+Infinity\n";
          else if (score<-1E30)
            cout << "-Infinity\n";
          else 
            cout << score << ".\n";
        }
        score = AlphaBeta(pl, b, chosen_depth, true, mv, -FLT_MAX, FLT_MAX);
        if (DEB_2) {
          cout << "==== AlphaBeta Move and Score ===" << endl;
          cout << Name(pl) << " plays. The move is " << "(" << char(mv.second.first) << "," << char(mv.second.second) << ") with a score of ";
          if (score>1E30)
            cout << "+Infinity\n";
          else if (score<-1E30)
            cout << "-Infinity\n";
          else 
            cout << score << ".\n";
        }
      }
      // The move is actually executed (the state of the board is internally changed)
      b.MakeMove(mv);

      if (DEB) {
        cout << "The board is now\n";
        b.Show(cout);
      }

      // Next player is the opponent of the current one.
      pl=Opponent(pl);
    }

    if (DEB_2) {
      cout << "The ending board is\n";
      b.Show(cout);
      cout << "The winner is " << Name(winner) << " with a score of " << winner_score << " against the " << looser_score << " of " << Name(Opponent(winner)) << ".\n";
      if (winner_score==looser_score)
        cout << "Well, yeah, I know this is a tie, but in such a case ladies win, don't they?\n";
    }

    // Add the en of this round to the players statistics
    // For Bob
    if (Name(winner) == "Bob") {
      wins_bob++;
      scores_bob[round] = winner_score;
    } else {
      scores_bob[round] = looser_score;
    }
    // And for Alice
    if (Name(winner) == "Alice") {
      wins_alice++;
      scores_alice[round] = winner_score;
    } else {
      scores_alice[round] = looser_score;
    }

    // Output for control that the script is running
    if (round % 10 == 0) {
      cout << "Round #" << setw(3) << setfill('0') << round << " " << flush;
    }
    if ((round+1) % 100 == 0) cout << endl;
  }

  // Calculating and printing the results of the experiment
  unsigned sum_bob = 0, sum_alice = 0;
  sum_bob = accumulate(scores_bob,scores_bob+ROUNDS,sum_bob);
  sum_alice = accumulate(scores_alice,scores_alice+ROUNDS,sum_alice);
  cout << "\n\nPlayed rounds:\t\t" << ROUNDS << endl;
  cout << "Alpha-Beta-Pruning?\t" << PRUNE << endl;
  cout << "Search depth:\t\t" << chosen_depth << endl;
  cout  << endl;
  cout << "The results are:" << endl;
  cout << "Metric\t\tBob\tAlice" << endl;
  cout << "Avg. Score\t" << (sum_bob/ROUNDS)  << "\t" <<  (sum_alice/ROUNDS) << endl;
  cout << "Won Games\t" << wins_bob << "\t" << wins_alice << endl;
}

