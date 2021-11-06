#include "minimax.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>

using namespace std;

// This is a global variable, since it might have to be used by heuristic, too.
unsigned chosen_depth=3;

int main(int argc,char *argv[])
{
 bool rand_mode;
 if (argc==1)
 {
   srand(time(0));
   rand_mode = true;
 } else if (argc == 2) {
    if (strcmp(argv[1], "BOB_RANDOM") == 0) {
      srand(time(0));
      rand_mode = true;
    } else if (strcmp(argv[1], "BOB_HUMAN") == 0) {
      rand_mode = false;
    } else {
      cerr << "Unknown option!" << endl;
      exit(1);
    }
 } else {
   cerr << "To many arguments!" << endl;
   exit(1);
 }

 // The default constructor initializes the board to the state marked by the rules.
 Board b;
 if (DEB)
 {
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
 while (!b.GameOver(winner,winner_score,looser_score))
 {
  if (rand_mode) {
    if (Name(pl) == "Alice" ) {
      // Minimax chooses a move (it will be stored in mv) and returns the score of that move.
      score=Minimax(pl,b,chosen_depth,true,mv);
    } else {
      Sqlist moves = b.ValidMoves(pl);
      if (!moves.empty())
      {
        Sqlist::iterator iter = moves.begin();
        if (moves.size() > 1) {
          unsigned i = 0, pos = rand() % (moves.size()-1);

          while (i != pos) {
            iter++;
            i++;
          }
        }
        score = 0.0;
        mv = make_pair(pl, (*iter));
      } else {
        score = 0.0;
        mv = make_pair(pl, pass);
      }
    }
  } else {
    cerr << "Alice against Human is not implemented!" << endl;
    exit(1);
  }


  if (DEB)
  {
   cout << Name(pl) << " plays. The move is " << "(" << char(mv.second.first) << "," << char(mv.second.second) << ") with a score of ";
   if (score>1E30)
    cout << "+Infinity\n";
   else
    if (score<-1E30)
     cout << "-Infinity\n";
    else 
      cout << score << ".\n";
  }

  // The move is actually executed (the state of the board is internally changed)
  b.MakeMove(mv);

  if (DEB)
  {
   cout << "The board is now\n";
   b.Show(cout);
  }
  
  // Next player is the opponent of the current one.
  pl=Opponent(pl);
 }

 if (DEB)
 {
  cout << "The winner is " << Name(winner) << " with a score of " << winner_score << " against the " << looser_score << " of " << Name(Opponent(winner)) << ".\n";
  if (winner_score==looser_score)
   cout << "Well, yeah, I know this is a tie, but in such a case ladies win, don't they?\n";
 }
}

