#ifndef Voting_h
#define Voting_h

// --------
// includes
// --------

#include <iostream> // istream, ostream

using namespace std;
// -------------
// Entrant class
// -------------
class Entrant {
public:
    char name[81];
    int ballots[501];
    int votes;

    bool operator>(const Entrant& other);
    bool operator<(const Entrant& other);
    bool operator>=(const Entrant& other);
    bool operator<=(const Entrant& other);
    bool operator==(const Entrant& other);
    bool operator!=(const Entrant& other);
};

// -------------
// swap_entrants
// -------------

/**
 * swaps two entrants in the ordered representation
 * @param a an integer, representing the index of one element to swap
 * @param b an integer, the index of the other element to swap
 */
void swap_entrants(int a, int b);


// ---------
// quicksort
// ---------

/**
 * recursive, in-place quicksort of the ordered representation of candidates
 * @param s an integer, the start of the range to sort
 * @param e an integer, the end of the range to sort
 */
void quicksort(int s, int e);


// ------------
// print_winner
// ------------

/**
 * prints one candidates name
 * @param o an ostream, to which the name of one candidate will be printed
 * @param index an integer, the index of the candidate whose name will be printed
 */
void print_winner(ostream& o, int slot);


// ------------
// voting_print
// ------------

/**
 * looks through all winners of this vote, calls print_winner to print the name of each
 * @param o an ostream, to which winners are printed
 */
void voting_print(ostream& o);


// ----------------
// reassign_ballots
// ----------------

/**
 * Reassigns the ballots associated with a candidate who lost to the next hightest
 * candidate that has not lost
 * @param loser an integer, representing the index of a candidate who has lost
 * @param remaining an integer, the number of remaining candidates after the loser has been removed
 */
void reassign_ballots(int loser, int remaining);


// -----------
// count_votes
// -----------

/**
 * for a given election, this calculates the winner in this australian voting system
 * recurses for each round of elimination
 * @param remaining an integer representing the remaining number of candidates after each round of elimination
 */
void count_votes(int remaining);


// -----------
// voting_read
// -----------

/**
 * helper method that reads election information into memory
 * @param i an istream, from which the election information shall be read
 */
void voting_read(istream& i);


// ------------
// voting_solve
// ------------

/**
 * solves the australian voting problem
 * reads input, assigns first round votes, determines the winner(s) and prints their names for each election
 * @param i an istream, from which election information is read
 * @param o an ostream, to which results shall be printed
 */
void voting_solve(istream& i, ostream& o);


#endif //Voting_h

