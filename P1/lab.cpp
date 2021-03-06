#include "Maze.h"
#include <map>
#include <limits>
#include <algorithm>
#include <fstream>

#define DEB true
#define SIDE MAX_SIDE

#include "heuristic.h"
// Don't forget to implement the heuristics in heuristic.cpp....

using namespace std;

const position invalid_pos = position(numeric_limits<unsigned>::max(), numeric_limits<unsigned>::max());

// Function to write the foundpath to disk
int WritePath (lp path, string fp) {
    ofstream file;
    string positions;

    for (auto & pos : path) {
        positions.append("[");
        positions.append(to_string(pos.first));
        positions.append(",");
        positions.append(to_string(pos.second));
        positions.append("],");
    }

    file.open(fp);
    if (!file) {
        cerr << "Could not open file for foundpath!" << endl;
    };

    file << "[" << endl;
    file << positions.substr(0,positions.length()-1) << endl;
    file << "]" << endl;
    file.close();
    return 1;
};

float getDistance (position current, position neighbor) {
    float ret;

    //-- Distance for north <> south and east <> west is: 1
    //-- Distance for diagonal is: sqrt(2)
    // Are we at the North neighbor?
    if (neighbor.first == current.first && neighbor.second < current.second ) ret = 1.0;
    // Are we at the North-East neighbor?
    if (neighbor.first > current.first && neighbor.second < current.second) ret = sqrt(2);
    // Are we at the East neighbor?
    if (neighbor.first > current.first && neighbor.second == current.second ) ret = 1.0;
    // Are we at the South-East neighbor?
    if (neighbor.first > current.first && neighbor.second > current.second ) ret = sqrt(2);
    // Are we at the South neighbor?
    if (neighbor.first == current.first && neighbor.second > current.second ) ret = 1.0;
    // Are we at the South-West neighbor?
    if (neighbor.first < current.first && neighbor.second > current.second ) ret = sqrt(2);
    // Are we at the West neighbor?
    if (neighbor.first < current.first && neighbor.second == current.second ) ret = 1.0;
    // Are we at the North-West neighbor?
    if (neighbor.first < current.first && neighbor.second < current.second ) ret = sqrt(2);

    return ret;
};

lp getNeighbors (Maze &maze, position curr) {
    position neighborsList[8];
    position curr_neighbor = invalid_pos;

    // Check if the curr pos is on the border of the board
    // 0 is thenorthern center field, others following clockwise
    if (curr.second == 0) neighborsList[7] = neighborsList[0] = neighborsList[1] = invalid_pos;
    if (curr.first == maze.GetNumCols()) neighborsList[1] = neighborsList[2] = neighborsList[3] = invalid_pos;
    if (curr.second == maze.GetNumRows()) neighborsList[3] = neighborsList[4] = neighborsList[5] = invalid_pos;
    if (curr.first == 0) neighborsList[5] = neighborsList[6] = neighborsList[7] = invalid_pos;

    // Iterate over all possible child positions
    for (int i = 0; i < 8; i++)
    {
        // If the current child we are evaluating is outside of the board: Skip this iteration
        if (neighborsList[i] == invalid_pos) continue;

        // In each iteration we come from the current position
        curr_neighbor = curr;

        // From the current position, we create our curr_neighbor position clockwise
        // Move one to north
        if (i == 7 || i == 0 || i == 1) curr_neighbor.second--;
        // Move one to east
        if (i == 1 || i == 2 || i == 3) curr_neighbor.first++;
        // Move one to south
        if (i == 3 || i == 4 || i == 5) curr_neighbor.second++;
        // Move one to west
        if (i == 5 || i == 6 || i == 7) curr_neighbor.first--;

        if (maze.IsObstacle(curr_neighbor))
        {
            // When it is an obstacle, it is an invalid position
            neighborsList[i] = invalid_pos;
        } else {
            // otherwise add the position to the neighborsList at the specific position
            neighborsList[i] = curr_neighbor;
        };
    };
    
    // To return a lp datatype
    lp returnList;
    for (int i = 0; i<8; i++)
    {
        if (neighborsList[i] != invalid_pos) returnList.push_back(neighborsList[i]);
    }

    return returnList;
};

// Write here the Astar and all other auxiliary functions you need...
bool AStar (Maze &map, lp &path)
{
    float temp_g, temp_h, gValues[SIDE][SIDE], hValues[SIDE][SIDE], fValues[SIDE][SIDE];
    lp neighbors, openList, closedList, goals = map.GetGoals();
    position neighbor, current = map.GetStart(), prevNodes[SIDE][SIDE];
    lp::iterator nei;
    // filling the arrays with range based for loops
    for (auto& col : gValues) { for (float& row : col) { row = numeric_limits<float>::infinity();}};
    for (auto& col : hValues) { for (float& row : col) { row = numeric_limits<float>::infinity();}};
    for (auto& col : fValues) { for (float& row : col) { row = numeric_limits<float>::infinity();}};
    for (auto& col : prevNodes) { for (position& row : col) { row = invalid_pos;}};

    prevNodes[current.first][current.second] = current;

    openList.push_back(current);

    gValues[current.first][current.second] = 0.0;
    hValues[current.first][current.second] = h(current, goals);
    fValues[current.first][current.second] = gValues[current.first][current.second]  + hValues[current.first][current.second] ;

    // Algorithm loop
    while (!openList.empty())
    {
        // We need to search the element with the best f value
        // in the openList, but only, if there is more than one element
        if (openList.size() > 1) {
            lp::iterator oli = openList.begin();
            position curr_pos, best_pos;
            float curr_f, best_f = numeric_limits<float>::infinity();
            while (oli != openList.end()) {
                curr_pos = *oli;
                curr_f = fValues[curr_pos.first][curr_pos.second];

                if (curr_f < best_f) {
                    best_f = curr_f;
                    best_pos = curr_pos;
                }

                oli++;
            }
            current = best_pos;
        } else {
            // Otherwise we can set current to the first element
            current = *openList.begin();
        };

        if (map.IsGoal(current)) {
            position temp_curr = current;
            while (prevNodes[temp_curr.first][temp_curr.second] != temp_curr) {
                path.push_front(temp_curr);
                temp_curr = prevNodes[temp_curr.first][temp_curr.second];
            }
            path.push_front(temp_curr);
            cout << "The cost of the found path is: \t\t" << gValues[current.first][current.second] << endl;
            return true;
        };

        // The current Node does not need to be expanded anymore
        openList.remove(current);
        // The current Node has been expanded
        closedList.push_back(current);
        neighbors = getNeighbors(map, current);

        nei = neighbors.begin();
        while (nei!=neighbors.end()) {
            neighbor = *nei;
            // Check if neighbor is not in closed list
            if (find(closedList.begin(), closedList.end(), neighbor) == closedList.end()) {
                // Calculate temp_g for neighbor: g(current) + d(current, neighbor)
                temp_g = gValues[current.first][current.second] + getDistance(current, neighbor);
                // Check if temp_g is lower than g(neighbor)
                if (temp_g < gValues[neighbor.first][neighbor.second]) {
                    // Store our current best g Value for the neighbor
                    gValues[neighbor.first][neighbor.second] = temp_g;
                    // Store the related previous Node (it is the current one)
                    prevNodes[neighbor.first][neighbor.second] = current;
                    // Cache for h() because it could be calculated multiple times
                    if (hValues[neighbor.first][neighbor.second] == numeric_limits<float>::infinity()) {
                        hValues[neighbor.first][neighbor.second] = temp_h = h(neighbor, goals);
                    } else {
                        temp_h = hValues[neighbor.first][neighbor.second];
                    };
                    // Store the current f Value for the neighbor
                    fValues[neighbor.first][neighbor.second] = temp_g + temp_h;
                    // Check if neighbor is not in openList
                    if (find(openList.begin(), openList.end(), neighbor) == openList.end()) {
                        // Add the neighbor to the openList to maybe expand it
                        openList.push_back(neighbor);
                    };
                };
            };
            nei++;
        };
    };

    if (DEB) cout << "No solution found :-(" << endl;
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
    } else {
        cout << "Path has been found!\n";
    }

    // You must write the WritePath funcion, too. Its second argument is a string with the file name to write the solution
    if (DEB)
    {
        string outfile = "foundpath";
        #ifdef ni_HEURISTIC
        outfile.append("_ni");
        #endif
        #ifdef euklid_HEURISTIC
        outfile.append("_euklid");
        #endif
        WritePath(foundpath, outfile);
    }

    if (DEB)
        lab.Print(cout,foundpath);

    return ( result ? 0 : 1);
};
