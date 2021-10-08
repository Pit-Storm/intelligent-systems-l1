#include "heuristic.h"


// You can include an option like -Dwhatever_HEURISTIC in the compilation line of the Makefile to choosa a heuristic, using here the ifdef...

// For example, for the non-informative,
#ifdef ni_HEURISTIC
float h(position p,lp goals)
{
 return 0.0;
}
#endif

// Here you can implement another one...
#ifdef anyname_HEURISTIC
float h(position p,lp goals)
{
 
 return 0.0;
}
#endif

// ... and similarly, as many as you want.
