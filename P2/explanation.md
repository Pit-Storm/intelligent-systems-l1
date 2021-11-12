# Explanation of the Work

## otello_test.cpp

At this file I added an additional loop to play more than only one game. The reason for this is, we need a statistical evaluation of the strength of the implemented heuristic. That would only be possible, if we aquire a sufficient number of ssamples. In this time I set it to 500 (See the definition of ROUNDS in the top of the file).

Also related to test the strength of the heuristic we need a Benchmark to measure against. For that in such fields a good first baseline is a player that makes random moves. So we can check, if the algorithm (in our case our heuristic) is able to defeat a player making random moves. If not, in average it would be better to play random moves and don't follow the algorithm.

To let Alice play against a random Bob, you pass `BOB_RAND` as an additional argument to the script. If so, every move that Bob is executing, will be randomly sample from all possible ones.

## alphabeta.cpp

## heuristic.cpp