#include "Maze.h"
#include <map>

#define DEB true

#include "heuristic.h"
// Don't forget to implement the heuristics in heuristic.cpp....

using namespace std;

// Write here the Astar and all other auxiliary functions you need...

// The main is left almost unchanged...
int main(int argc,char *argv[])
{
 if (argc!=2)
 {
  cerr << "       " << argv[0] << " <input_file>\n";
  exit(1);
 }

 string inputname=string(argv[1]);
 Maze lab(inputname);

 if (DEB)
  lab.Print(cout);

 lp foundpath;
 foundpath.clear();

 // Think here which argument should be passed to the Astar function.
 // and think how the last one, the foundpath, must be passed...
 bool result=AStar(....,foundpath);
 if (result==false)
 {
  cerr << "No path has been found for that maze. Writing the empty list.\n";
  foundpath.clear();
 }

 // You must write the WritePath funcion, too. Its second argument is a string with the file name to write the solution
 if (DEB)
  WritePath(foundpath,"a name for the output file...");

 if (DEB)
  lab.Print(cout,foundpath);

 return ( result ? 0 : 1);
}

