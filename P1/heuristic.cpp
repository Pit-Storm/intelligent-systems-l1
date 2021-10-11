#include "heuristic.h"
#include <list>


// You can include an option like -Dwhatever_HEURISTIC in the compilation line of the Makefile to choosa a heuristic, using here the ifdef...

// For example, for the non-informative,
#ifdef ni_HEURISTIC
// This is equivalent to the Dijkstra algorithm
float h(position p,lp goals)
{
    return 0.0;
}
#endif

// euklidian distance
#ifdef euklid_HEURISTIC
typedef std::list<double> gd;    
float h(position p,lp goals)
{
    double H = 0-0;
    gd goaldists;
    goaldists.clear();
    lp::iterator goal = goals.begin();

    // Calculate the euklidian distance to all of our goals
    while (goal!=goals.end())
    {
        goaldists.push_back(sqrt((p.first - goal->first)*(p.first - goal->first)
            + (p.second - goal->second)*(p.second - goal->second)));
        goal++;
    };

    // Select the shortest distance
    goaldists.sort();
    H = goaldists.front();

    // return the shortest distance to a goal
    return float(H);
}
#endif

// ... and similarly, as many as you want.
