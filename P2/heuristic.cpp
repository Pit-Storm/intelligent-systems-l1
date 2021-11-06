#include "heuristic.h"

float h(Player pl,Board b)
{
    
    // We could use the Maximum disks strategy
    // We count the number of disks for each player on the board and try to maximize them in each move.
    Token t=Tk(pl);
    unsigned disks=0;
    Square sq;
    sq.first = '1';
    sq.second = 'A';

    for (unsigned i=0;i<BOARD_SIZE;i++) {
        sq.second = 'A'+i;
        for (unsigned j=0;j<BOARD_SIZE;j++) {
            sq.first = '1'+j;
            if (b.Content(sq) ==t) {
                disks++;
            }
        }
    }
    
    // We could although use the positional strategy
    // This is based on the fact that if we occupied some squares, they will give the opponent access to their neighbors
    // As well we knoew that some squares are more stable (se below) e.g. the most stable are the edges, different squares
    // getting a different value of how good they are to have them occupied.

    // We could use the stability calculation
    // It says: How many disks of each player are not possible to flip over during the whole game in the future and maximizing this.

    // We could use the Mobility strategy
    // We count the possible moves every player has and with our move we try to minimize the mobility of the opponent in its next turn.
    // unsigned mobi = b.ValidMoves(pl).size();

    // We could use the evaporation strategy
    // It's related to the mobility strategy. So it exploits the mobility strategy in the early stages
    // by minimizing the own stones on the board. This indicates to getting more degrees of freedom.

    // Another strategy related to mobility is the frontiers strategy
    // you try to minimize the stones you have in neighborship to free squares (the edges).
    // This will minimize the degrees of freedom the oponent has.

    // 
    return disks;
}

