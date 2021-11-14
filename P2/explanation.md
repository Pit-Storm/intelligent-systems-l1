# Explanation of the Work

## otello_test.cpp

At this file I added an additional loop to play more than only one game. The reason for this is, we need a statistical evaluation of the strength of the implemented heuristic. That would only be possible, if we aquire a sufficient number of ssamples. In this time I set it to 500 (See the definition of ROUNDS in the top of the file).

Also related to test the strength of the heuristic we need a Benchmark to measure against. For that in such fields a good first baseline is a player that makes random moves. So we can check, if the algorithm (in our case our heuristic) is able to defeat a player making random moves. If not, in average it would be better to play random moves and don't follow the algorithm.

To let Alice play against a random Bob, you pass `BOB_RAND` as an additional argument to the script. If so, every move that Bob is executing, will be randomly sample from all possible ones.

## alphabeta.cpp

here the algorithm is nearly the same, except, that we compare the actual value against alpha (for maximizing player) or beta (for minimizing player) and if higher (or lower) we set the value as alpha/beta. Additionally we break the loop, if there is no better value to gain from that node on.

To achieve that, I added an alpha and beta parameter to the method and in the main program it is called with `-FLT_MAX` and `FLT_MAX` for alpha/beta. In the recursive call it's called with the actual alpha/beta value.

## heuristic.cpp

You can choose the heuristic to compile with in `makefile` line 2 to 5. The following are implemented:

* Number of own Disks on board -> Disks (`H_DISKS`)
* Number of Valid moves -> Mobility (`H_MOBI`)
* Value of Positions -> Positional (`H_POSI`)
* A Combination of single heuristics -> Combination (`H_COMBI`)
* EXTRA: Gametime related calculation of Combination -> Timerelated (`H_TIMEREL`)

For more explanation see in `heuristics.cpp` near the implementation.

## Experimental results

The experiments had been the following:

* Bob played a random move every round.
* Alice executed the Alpha-Beta-Algorithm with the heuristic to be measured.
* The chosen depth to search to for every experiment was 3.
* There had been played 500 rounds played every experiment.
* The deximal digits of the Avg. Score are cutted off.

### Experiment One: Disks

Metric          Bob     Alice
Avg. Score      35      27
Won Games       310     190

### Experiment Two: Mobility

Metric          Bob     Alice
Avg. Score      33      30
Won Games       273     227

### Experiment Three: Positional

Metric          Bob     Alice
Avg. Score      33      30
Won Games       263     237

### Experiment Four: Combination

Metric          Bob     Alice
Avg. Score      33      30
Won Games       254     246

## Interpretation and Conclusion

We can see, that Alice is getting better in every experiment (dpending on the heuristic that had been chosen). So we can sum up, that the basic counting of disks is worst than the number of moves the player has. That one is worst than a heuristic that calculated a weighted score for every position accupied on the board. And the best is a linear combination out of that heuristics.

All in all even the combination of basic heuristics is not able to beat a random player (in average over 500 games). So there has to be a time related component in the game. E.g. that a different strategy is affordable in the beginning, than in the middle, than in the end of the game.

For that I created an extra Heuristic that adds up a time related calculation of the heuristical score (`H_TIMEREL`). The additional assumption for that is, that in the end of the game (if no more free fields exist), the heuristic should return a high value for a won game and a very low value for a lost game. The results are below.

The results of the extra experiment showed us, that there is only a lowly significant influence to the time relation. Additionally it tends to be worst than the basic combination of strategies. Therefor more investigation and tuneing would be necessary. In reality I would tackle a GamePlay problem such as otello with a Deep Reinforcement-Learning Agent, because they are easiest and best for such problems.

### Results EXTRA Experiment five: Timerelated

Metric          Bob     Alice
Avg. Score      33      30
Won Games       258     242
