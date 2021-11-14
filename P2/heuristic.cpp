#include "heuristic.h"

// All strategies are from: http://www.samsoft.org.uk/reversi/strategy.htm
// In the following comments there are more possible strategies to base the heuristic on

// We could use the stability calculation
// It says: How many disks of each player are not possible to flip over during the whole game in the future and maximizing this.

// A strategy related to mobility is the frontiers strategy
// you try to minimize the stones you have in neighborship to free squares (the edges).
// This will minimize the degrees of freedom the oponent has.

// The explanation of every implemented heurstic can be found right next to the implementation


// DISKS
// We could use the Maximum disks strategy
// We count the number of disks for each player on the board and try to maximize them in each move.

#ifdef H_DISKS
float h(Player pl,Board b)
{
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
    
    return disks;
}
#endif


// MOBILITY
// We could use the Mobility strategy
// We count the possible moves every player has and the more mobility, the better a board is.

#ifdef H_MOBI
float h(Player pl,Board b)
{
    float score = b.ValidMoves(pl).size();
    return score;
}
#endif


// POSITIONAL
// We could although use the positional strategy
// This is based on the fact that if we occupied some squares, they will give the opponent access to their neighbors
// As well we knoew that some squares are more stable (se below) e.g. the most stable are the edges, different squares
// getting a different value of how good they are to have them occupied.

#ifdef H_POSI
float h(Player pl,Board b)
{
    int posi_scores[BOARD_SIZE][BOARD_SIZE];

    // Filling the helper board with the scores of every position
    for (unsigned letter = 0; letter<BOARD_SIZE; letter++) {
        for (unsigned number = 0; number<BOARD_SIZE; number++) {
            // Diagonal squares Score
            // Corner squares
            if ((letter == 0 && number == 0) || (letter == 7 && number == 0) || (letter == 7 && number == 7) || (letter == 0 && number == 7))
                posi_scores[letter][number] = 99;
            // First inner diagonal squares
            if ((letter == 1 && number == 1) || (letter == 6 && number == 1) || (letter == 1 && number == 6) || (letter == 6 && number == 6))
                posi_scores[letter][number] = -24;
            // Second inner diagonal squares
            if ((letter == 2 && number == 2) || (letter == 5 && number == 2) || (letter == 2 && number == 5) || (letter == 5 && number == 5))
                posi_scores[letter][number] = 7;
            // Starting squares
            if ((letter == 3 && number == 3) || (letter == 4 && number == 3) || (letter == 3 && number == 4) || (letter == 4 && number == 4))
                posi_scores[letter][number] = 1;

            // Outter frame of squares without diagonals
            // Direct corner neighbors (clockwise)
            if ((letter == 1 && number == 0) || (letter == 6 && number == 0) || (letter == 7 && number == 1) || (letter == 7 && number == 6) || (letter == 6 && number == 7) || (letter == 1 && number == 7) || (letter == 0 && number == 6) || (letter == 0 && number == 1))
                posi_scores[letter][number] = -8;
            // Neighbors of direct cornor neighbors (clockwise)
            if ((letter == 2 && number == 0) || (letter == 5 && number == 0) || (letter == 7 && number == 2) || (letter == 7 && number == 5) || (letter == 5 && number == 7) || (letter == 2 && number == 7) || (letter == 0 && number == 5) || (letter == 0 && number == 2))
                posi_scores[letter][number] = 8;
            // "D"+"C" and "4"+"5" cross of the outter frame (clickwise)
            if ((letter == 3 && number == 0) || (letter == 4 && number == 0) || (letter == 7 && number == 3) || (letter == 7 && number == 4) || (letter == 4 && number == 7) || (letter == 3 && number == 7) || (letter == 0 && number == 4) || (letter == 0 && number == 3))
                posi_scores[letter][number] = 6;
            
            // Second inner frame without diagonals
            // Direct diagonal neighbors
            if ((letter == 2 && number == 1) || (letter == 5 && number == 1) || (letter == 6 && number == 2) || (letter == 6 && number == 5) || (letter == 5 && number == 6) || (letter == 2 && number == 6) || (letter == 1 && number == 5) || (letter == 1 && number == 2))
                posi_scores[letter][number] = -4;
            // Neighbors of direct diagonal neighbors
            if ((letter == 3 && number == 1) || (letter == 4 && number == 1) || (letter == 6 && number == 3) || (letter == 6 && number == 4) || (letter == 4 && number == 6) || (letter == 3 && number == 6) || (letter == 1 && number == 4) || (letter == 1 && number == 3))
                posi_scores[letter][number] = -3;

            // Frame around starting positions without diagonals
            if ((letter == 3 && number == 2) || (letter == 4 && number == 2) || (letter == 5 && number == 3) || (letter == 5 && number == 4) || (letter == 4 && number == 5) || (letter == 3 && number == 5) || (letter == 2 && number == 4) || (letter == 2 && number == 3))
                posi_scores[letter][number] = 4;
        }
    }

    int score = 0;
    Token t = Tk(pl);
    Square sq;
    sq.first = '1';
    sq.second = 'A';

    for (unsigned l = 0; l<BOARD_SIZE; l++) {
        sq.second = 'A'+l;
        for (unsigned n = 0; n<BOARD_SIZE; n++) {
            sq.first = '1'+n;
            if (b.Content(sq) == t) {
                score += posi_scores[l][n];
            }
        }
    }

    return score;
}
#endif

// COMBINATION
// This combines the first three heuristics to one
// The linear combination is weighted like the fitness of the single strategy.

#ifdef H_COMBI
float h(Player pl,Board b)
{
    int posi_scores[BOARD_SIZE][BOARD_SIZE];

    // Filling the helper board with the scores of every position
    for (unsigned letter = 0; letter<BOARD_SIZE; letter++) {
        for (unsigned number = 0; number<BOARD_SIZE; number++) {
            // Diagonal squares Score
            // Corner squares
            if ((letter == 0 && number == 0) || (letter == 7 && number == 0) || (letter == 7 && number == 7) || (letter == 0 && number == 7))
                posi_scores[letter][number] = 99;
            // First inner diagonal squares
            if ((letter == 1 && number == 1) || (letter == 6 && number == 1) || (letter == 1 && number == 6) || (letter == 6 && number == 6))
                posi_scores[letter][number] = -24;
            // Second inner diagonal squares
            if ((letter == 2 && number == 2) || (letter == 5 && number == 2) || (letter == 2 && number == 5) || (letter == 5 && number == 5))
                posi_scores[letter][number] = 7;
            // Starting squares
            if ((letter == 3 && number == 3) || (letter == 4 && number == 3) || (letter == 3 && number == 4) || (letter == 4 && number == 4))
                posi_scores[letter][number] = 1;

            // Outter frame of squares without diagonals
            // Direct corner neighbors (clockwise)
            if ((letter == 1 && number == 0) || (letter == 6 && number == 0) || (letter == 7 && number == 1) || (letter == 7 && number == 6) || (letter == 6 && number == 7) || (letter == 1 && number == 7) || (letter == 0 && number == 6) || (letter == 0 && number == 1))
                posi_scores[letter][number] = -8;
            // Neighbors of direct cornor neighbors (clockwise)
            if ((letter == 2 && number == 0) || (letter == 5 && number == 0) || (letter == 7 && number == 2) || (letter == 7 && number == 5) || (letter == 5 && number == 7) || (letter == 2 && number == 7) || (letter == 0 && number == 5) || (letter == 0 && number == 2))
                posi_scores[letter][number] = 8;
            // "D"+"C" and "4"+"5" cross of the outter frame (clickwise)
            if ((letter == 3 && number == 0) || (letter == 4 && number == 0) || (letter == 7 && number == 3) || (letter == 7 && number == 4) || (letter == 4 && number == 7) || (letter == 3 && number == 7) || (letter == 0 && number == 4) || (letter == 0 && number == 3))
                posi_scores[letter][number] = 6;
            
            // Second inner frame without diagonals
            // Direct diagonal neighbors
            if ((letter == 2 && number == 1) || (letter == 5 && number == 1) || (letter == 6 && number == 2) || (letter == 6 && number == 5) || (letter == 5 && number == 6) || (letter == 2 && number == 6) || (letter == 1 && number == 5) || (letter == 1 && number == 2))
                posi_scores[letter][number] = -4;
            // Neighbors of direct diagonal neighbors
            if ((letter == 3 && number == 1) || (letter == 4 && number == 1) || (letter == 6 && number == 3) || (letter == 6 && number == 4) || (letter == 4 && number == 6) || (letter == 3 && number == 6) || (letter == 1 && number == 4) || (letter == 1 && number == 3))
                posi_scores[letter][number] = -3;

            // Frame around starting positions without diagonals
            if ((letter == 3 && number == 2) || (letter == 4 && number == 2) || (letter == 5 && number == 3) || (letter == 5 && number == 4) || (letter == 4 && number == 5) || (letter == 3 && number == 5) || (letter == 2 && number == 4) || (letter == 2 && number == 3))
                posi_scores[letter][number] = 4;
        }
    }

    // Calculate the Positional and Disks score
    int posi_score = 0, disks_score = 0;
    Token t = Tk(pl);
    Square sq;
    sq.first = '1';
    sq.second = 'A';

    for (unsigned l = 0; l<BOARD_SIZE; l++) {
        sq.second = 'A'+l;
        for (unsigned n = 0; n<BOARD_SIZE; n++) {
            sq.first = '1'+n;
            if (b.Content(sq) == t) {
                posi_score += posi_scores[l][n];
                disks_score++;
            }
        }
    }

    // Calculate the mobility score
    float mobi_score = b.ValidMoves(pl).size();

    // Calculate complete score
    float score = posi_score + (mobi_score*0.75) + (disks_score*0.5);

    return score;
}
#endif


// TIMERELATED
// This makes the H_COMBI also time related
// It also incorporate the evoporation strategy. It's related to the mobility strategy. So it exploits the mobility strategy in the early stages
// by minimizing the own stones on the board. This indicates to getting more degrees of freedom.

#ifdef H_TIMEREL
float h(Player pl,Board b)
{
    int posi_scores[BOARD_SIZE][BOARD_SIZE];

    // Filling the helper board with the scores of every position
    for (unsigned letter = 0; letter<BOARD_SIZE; letter++) {
        for (unsigned number = 0; number<BOARD_SIZE; number++) {
            // Diagonal squares Score
            // Corner squares
            if ((letter == 0 && number == 0) || (letter == 7 && number == 0) || (letter == 7 && number == 7) || (letter == 0 && number == 7))
                posi_scores[letter][number] = 99;
            // First inner diagonal squares
            if ((letter == 1 && number == 1) || (letter == 6 && number == 1) || (letter == 1 && number == 6) || (letter == 6 && number == 6))
                posi_scores[letter][number] = -24;
            // Second inner diagonal squares
            if ((letter == 2 && number == 2) || (letter == 5 && number == 2) || (letter == 2 && number == 5) || (letter == 5 && number == 5))
                posi_scores[letter][number] = 7;
            // Starting squares
            if ((letter == 3 && number == 3) || (letter == 4 && number == 3) || (letter == 3 && number == 4) || (letter == 4 && number == 4))
                posi_scores[letter][number] = 1;

            // Outter frame of squares without diagonals
            // Direct corner neighbors (clockwise)
            if ((letter == 1 && number == 0) || (letter == 6 && number == 0) || (letter == 7 && number == 1) || (letter == 7 && number == 6) || (letter == 6 && number == 7) || (letter == 1 && number == 7) || (letter == 0 && number == 6) || (letter == 0 && number == 1))
                posi_scores[letter][number] = -8;
            // Neighbors of direct cornor neighbors (clockwise)
            if ((letter == 2 && number == 0) || (letter == 5 && number == 0) || (letter == 7 && number == 2) || (letter == 7 && number == 5) || (letter == 5 && number == 7) || (letter == 2 && number == 7) || (letter == 0 && number == 5) || (letter == 0 && number == 2))
                posi_scores[letter][number] = 8;
            // "D"+"C" and "4"+"5" cross of the outter frame (clickwise)
            if ((letter == 3 && number == 0) || (letter == 4 && number == 0) || (letter == 7 && number == 3) || (letter == 7 && number == 4) || (letter == 4 && number == 7) || (letter == 3 && number == 7) || (letter == 0 && number == 4) || (letter == 0 && number == 3))
                posi_scores[letter][number] = 6;
            
            // Second inner frame without diagonals
            // Direct diagonal neighbors
            if ((letter == 2 && number == 1) || (letter == 5 && number == 1) || (letter == 6 && number == 2) || (letter == 6 && number == 5) || (letter == 5 && number == 6) || (letter == 2 && number == 6) || (letter == 1 && number == 5) || (letter == 1 && number == 2))
                posi_scores[letter][number] = -4;
            // Neighbors of direct diagonal neighbors
            if ((letter == 3 && number == 1) || (letter == 4 && number == 1) || (letter == 6 && number == 3) || (letter == 6 && number == 4) || (letter == 4 && number == 6) || (letter == 3 && number == 6) || (letter == 1 && number == 4) || (letter == 1 && number == 3))
                posi_scores[letter][number] = -3;

            // Frame around starting positions without diagonals
            if ((letter == 3 && number == 2) || (letter == 4 && number == 2) || (letter == 5 && number == 3) || (letter == 5 && number == 4) || (letter == 4 && number == 5) || (letter == 3 && number == 5) || (letter == 2 && number == 4) || (letter == 2 && number == 3))
                posi_scores[letter][number] = 4;
        }
    }

    // Calculate the Positional and Disks score
    int posi_score = 0, disks_score = 0, free_fields = 0;
    Token t = Tk(pl);
    Square sq;
    sq.first = '1';
    sq.second = 'A';

    for (unsigned l = 0; l<BOARD_SIZE; l++) {
        sq.second = 'A'+l;
        for (unsigned n = 0; n<BOARD_SIZE; n++) {
            sq.first = '1'+n;
            if (b.Content(sq) == t) {
                posi_score += posi_scores[l][n];
                disks_score++;
            }
            if (b.Content(sq) == Token::e) free_fields++;
        }
    }

    // Calculate the mobility score
    float mobi_score = b.ValidMoves(pl).size();

    // Calculate complete score
    float score = 0.0;
    if (free_fields >= 40) score = posi_score + mobi_score + (disks_score*0.5);
    if (40 > free_fields && free_fields >= 10) score = posi_score + (mobi_score*0.75) + (disks_score*0.5);
    if (10 > free_fields && free_fields > 0) score = posi_score + (mobi_score*0.75) + (disks_score*0.5);
    // In the end of the game there is only a won or lost evaluation to be included
    if (free_fields == 0) {
        if (disks_score <= 32) score = -999999;
        if (disks_score > 32) score = 999999;
    }

    return score;
}
#endif