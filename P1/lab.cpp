#include "Maze.h"
#include <map>

#define DEB true

#include "heuristic.h"
// Don't forget to implement the heuristics in heuristic.cpp....

using namespace std;

const position invalid_pos = position(9999, 9999);

// Function to write the foundpath to disk
int WritePath (lp path, string fp) {
    // TODO:: Actually print the path into the file of fp
    return 0;
};

lp getChildren (Maze maze, position curr) {
    position childrenList[8];
    position curr_child = invalid_pos;

    // Check if the curr pos is on the border of the board
    // 0 is thenorthern center field, others following clockwise
    if (curr.second == 0) childrenList[7,0,1] = invalid_pos;
    if (curr.first == maze.GetNumCols()) childrenList[1,2,3] = invalid_pos;
    if (curr.second == maze.GetNumRows()) childrenList[3,4,5] = invalid_pos;
    if (curr.first == 0) childrenList[5,6,7] = invalid_pos;

    // Iterate over all possible child positions
    for (int i = 0; i < 8; i++)
    {
        // If the current child we are evaluating is outside of the board: Skip this iteration
        if (childrenList[i] == invalid_pos) continue;

        curr_child = curr;
        // Set the value for the current child object
        // Move one to north
        if (i == 7 || 0 || 1) curr_child.second--;
        // Move one to east
        if (i == 1 || 2 || 3) curr_child.first++;
        // Move one to south
        if (i == 3 || 4 || 5) curr_child.second++;
        // Move one to west
        if (i == 5 || 6 || 7) curr_child.first--;

        if (maze.IsObstacle(curr_child))
        {
            // When it is an obstacle, it is an invalid position
            childrenList[i] = invalid_pos;
        } else {
            // otherwise add the position to the childrenList at the specific position
            childrenList[i] = curr_child;
        };
    };
    
    // To return a lp datatype
    lp returnList;
    for (int i = 0; i<8; i++)
    {
        if (childrenList[i] != invalid_pos) returnList.push_back(childrenList[i]);
    }

    return returnList;
};

// Write here the Astar and all other auxiliary functions you need...
bool AStar (Maze map, lp& path)
{
    // Pseudocode from here: https://www.researchgate.net/figure/A-search-algorithm-Pseudocode-of-the-A-search-algorithm-operating-with-open-and-closed_fig8_232085273
    float f = 0.0, g = 0.0, gValues[map.GetNumCols()][map.GetNumRows()];
    lp openList, closedList, children, goals = map.GetGoals();
    position current = map.GetStart(), prevNodes[map.GetNumCols()][map.GetNumRows()];

    openList.push_front(current);
    f = g + h(current, goals);

    // Algorithm loop
    while (!openList.empty())
    {
        current = *openList.begin();
        openList.erase(openList.begin());
        // TODO: Generate the found path
        if (map.IsGoal(current)) return true;
        closedList.push_front(current);
        children = getChildren(map, current);

        // TODO: Implement to Logic for expanding every child Node
        lp::iterator child = children.begin();
        while (child!=children.end()) {
            // If Child in closed list
                // continue
            // cost = gValues[current.first][current.second] + getCost(current, child)
            // if child in open_list and cost < gValues[child.first][child.second]
                // open_list.erase(child)
            // if child in closed_list and cost < gValues[child.first][child.second]
                // closed_list.erase(child)
            // if child not in open_list and not in closed_list
                // 
        }
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
