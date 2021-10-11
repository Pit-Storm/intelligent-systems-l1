#include "Maze.h"
#include <map>

#define DEB true

#include "heuristic.h"
// Don't forget to implement the heuristics in heuristic.cpp....

using namespace std;

// Function to write the foundpath to disk
int WritePath (lp path, string fp) {
    // TODO:: Actually print the path into the file of fp
    return 0;
};

lp getChildren (Maze maze, position curr) {
    lp childrenList;
    // For every neighbor of curr check:
    //  if not maze.isObstacle()
    //  if not is out of board
    //  else add to neighbors list
    return childrenList;
};

// Write here the Astar and all other auxiliary functions you need...
bool AStar (Maze map, lp& path)
{
    // Pseudocode from here: https://www.researchgate.net/figure/A-search-algorithm-Pseudocode-of-the-A-search-algorithm-operating-with-open-and-closed_fig8_232085273
    float f = 0.0, g = 0.0, gValues[map.GetNumRows()][map.GetNumCols()];
    lp openList, closedList, children, goals = map.GetGoals();
    position current = map.GetStart(), prevNodes[map.GetNumRows()][map.GetNumCols()];
    // TODO: Check if initializing of gValues and prevNodes is corret (Col x Rows or Rows x Cols?)

    openList.push_front(current);
    f = g + h(current, goals);

    // Algorithm loop
    while (!openList.empty())
    {
        current = openList.front();
        openList.pop_front();
        // TODO: Generate the found path
        if (map.IsGoal(current)) return true;
        closedList.push_front(current);
        children = getChildren(map, current);
        // TODO: Implement to Logic for expanding every child Node
    };

    return false;
};

// The main is left almost unchanged...
int main(int argc, char *argv[])
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
    bool result = AStar(lab, foundpath);
    if (result==false)
    {
        cerr << "No path has been found for that maze. Writing the empty list.\n";
        foundpath.clear();
    }

    // You must write the WritePath funcion, too. Its second argument is a string with the file name to write the solution
    if (DEB)
    {
        string outfile = "foundpath";
        WritePath(foundpath, outfile);
    }

    if (DEB)
        lab.Print(cout,foundpath);

    return ( result ? 0 : 1);
};
