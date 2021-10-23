# P1 - A*-Algorithm

This folder contains the implementation for the first practice in intelligent systems.

## Prerequisites

* It compiles with `g++ (Debian 8.3.0-6) 8.3.0` and probably with other compilers on other systems.

## Compile and run

Just `cd` into the folder containing the source code and maze files. Than do:

1. `make lab` (for compiling and creating a binary)
2. `lab_ni.bin entgrande` (for running the algorithm)

For choosing another heuristic open the `Makefile` and set line #3 as a comment and take the comment from line #5 away. After that compile again. You have to run `lab_euklid.bin` for the other heuristic.

## Comments for the solution

### General

* In general I was trying to make the code as much efficient in computing time. This is why I am caching the hValues, because there is a great chance to calculate them more than once and it wont change over time.
* Although I was trying to use basic structures such as C++ basic arrays, because they are more the sufficient for that task.
* Comments to the heuristics:
    * The non-informative heuristic converts the A*-Search into the Dijkstra algorithm. Because the return Value for the ni_HEURISTIC is zero, it is the worst heuristic possible.
    * The euklidian heuristic is the best heuristic for that kind of problem, because the requirement for the heuristic for finding the optimal path with A* is, that it must not overestimate the distance between a node and the goal. If our problem would have been designed, that the robot could only move in four directions, we could have the manhattan distance as the best heuristic, than the euklidian as a medium good one and the ni one as the worst one.

### The found Paths

* The ni_Heuristic hops more diagonal and the euklid_Heuristic tries to abaid that.
* Nonetheless the ni_Heuristic and the euklid_Heuristic have to make 25 hops (moves) towards the goal.
* Additionally the cost for the path, they have been found is identical. It is 28,7279.

It would be interesting to discover how the different heuristics (Algorithms) would behave in a more complex maze. Although the runtime the need to discover the optimal path should be different in such a case.

## Instructions for solving the task

Entradas de prueba / test inputs:

* entprueba
* enttest1
* enttest2

Entrada para la práctica / input to be solved:

* entgrande

