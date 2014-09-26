// --------
// includes
// --------
#include <cassert>
#include <iostream>
#include <cstring>
#include <sstream>
#include <set>

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
bool Entrant::operator>(const Entrant& other) {
    return (this->votes > other.votes);
}
bool Entrant::operator<(const Entrant& other) {
    return (this->votes < other.votes);
}
bool Entrant::operator>=(const Entrant& other) {
    return (this->votes >= other.votes);
}
bool Entrant::operator<=(const Entrant& other) {
    return (this->votes <= other.votes);
}
bool Entrant::operator==(const Entrant& other) {
    return (this->votes == other.votes);
}
bool Entrant::operator!=(const Entrant& other) {
    return (this->votes != other.votes);
}

// ----------------
// global variables
// ----------------
int numbal;
int numcan;
int ballotmatrix[1000][20];
Entrant candidates[20];
int can_ordered[20];
set<int> losers;

// -------------
// swap_entrants
// -------------
/**
 * swaps two entrants in the ordered representation
 * @param a an integer, representing the index of one element to swap
 * @param b an integer, the index of the other element to swap
 */
void swap_entrants(int a, int b) {
    int tmp = can_ordered[a];
    can_ordered[a] = can_ordered[b];
    can_ordered[b] = tmp;
}

// ---------
// quicksort
// ---------
/**
 * recursive, in-place quicksort of the ordered representation of candidates
 * @param s an integer, the start of the range to sort
 * @param e an integer, the end of the range to sort
 */
void quicksort(int s, int e) {
    if(s < e) {
        int p = (s+e)>>1;
        int i=s, j=e;
        while( i<=j ) {
            while(i <= e && 
                candidates[can_ordered[i]] > candidates[can_ordered[p]]) ++i;
            while(j >= s && 
                candidates[can_ordered[j]] < candidates[can_ordered[p]]) --j;
            if (i <= j) {
                swap_entrants(i, j);
                ++i; --j;
            }
        }
        quicksort(s, p-1);
        quicksort(p+1, e);
    }
}

// ------------
// print_winner
// ------------
/**
 * prints one candidates name
 * @param o an ostream, to which the name of one candidate will be printed
 * @param index an integer, the index of the candidate whose name will be printed
 */
void print_winner(ostream& o, int index) {
    assert(index >= 0); assert(index < 20);
    char* c = candidates[index].name;
    while( *c )
        o << *(c++);
    o << endl;
}

// ------------
// voting_print
// ------------
/**
 * looks through all winners of this vote, calls print_winner to print the name of each
 * @param o an ostream, to which winners are printed
 */
void voting_print(ostream& o) {
    int i = 0;
    int v = candidates[can_ordered[0]].votes;
    while(i < 21 && candidates[can_ordered[i]].votes == v) {
        print_winner(o, can_ordered[i]);
        ++i;
    }
}

// ----------------
// reassign_ballots
// ----------------
/**
 * Reassigns the ballots associated with a candidate who lost to the next hightest
 * candidate that has not lost
 * @param loser an integer, representing the index of a candidate who has lost
 * @param remaining an integer, the number of remaining candidates after the loser has been removed
 */
void reassign_ballots(int loser, int remaining) {
    assert(loser >= 0); assert(loser < 20);
    assert(remaining > 0); assert(remaining < 21);
    for(int it = 0; it < candidates[loser].votes; ++it) {
        int ballot = candidates[loser].ballots[it];
        for( int i = 0; i < numcan; ++i ) {
            int index = ballotmatrix[ballot][i] - 1;
            if( losers.count(index)==1 )
                continue;
            if( candidates[index].votes > (numbal>>1) )
                return;
            candidates[index].ballots[candidates[index].votes] = ballot;
            candidates[index].votes += 1;
            break;
        }
    }
}

// -----------
// count_votes
// -----------
/**
 * for a given election, this calculates the winner in this australian voting system
 * recurses for each round of elimination
 * @param remaining an integer representing the remaining number of candidates after each round of elimination
 */
void count_votes(int remaining) {
    assert(remaining > 0); assert(remaining < 21);
    quicksort(0, remaining-1);

    int topsize = candidates[can_ordered[0]].votes;
    if (topsize > (numbal>>1) )
        return;

    int botsize = candidates[can_ordered[remaining-1]].votes;
    if (topsize == botsize)
        return;

    int it = remaining - 1;
    while( candidates[can_ordered[it]].votes == botsize && it > 0 ) {
        losers.insert( can_ordered[it] );
        reassign_ballots(can_ordered[it], --remaining);
        candidates[can_ordered[it]].votes = 0;
        --it;
    }
    count_votes(remaining);
}

// -----------
// voting_read
// -----------
/**
 * helper method that reads election information into memory
 * @param i an istream, from which the election information shall be read
 */
void voting_read(istream& i) {
    i >> numcan;
    i.ignore();
    int n = 0, m = 0;
    char c = '\000';
    string tmp;
    istringstream iss;
    while( n < 20 ) {
        if( n < numcan ) {
            getline(i, tmp);
            if( tmp == "") tmp = " ";
            tmp.resize((size_t) 80);
            strcpy(candidates[n].name, tmp.c_str());
            can_ordered[n] = n;
        } else
            can_ordered[n] = -1;
        candidates[n].votes = 0;
        ++n;
    }
    numbal = 0;
    while( numbal <= 1000 ) {
        if ( !i ) break;
        getline(i, tmp);
        if (tmp == "") break;
        iss.str(tmp);
        iss.seekg(0);
        m = 0;
        while(m < numcan) {
            iss >> n;
            ballotmatrix[numbal][m] = n;
            ++m;
        }
        ++numbal;
     }
}

// ------------
// voting_solve
// ------------
/**
 * solves the australian voting problem
 * reads input, assigns first round votes, determines the winner(s) and prints their names for each election
 * @param i an istream, from which election information is read
 * @param o an ostream, to which results shall be printed
 */
void voting_solve(istream& i, ostream& o) {
    int elections;
    i >> elections;
    while(elections > 0) {
        voting_read(i);
        for( int n = 0; n < numbal; ++n ) {
            int index = ballotmatrix[n][0] - 1;
            if( candidates[index].votes > (numbal>>1) ) break;
            candidates[index].ballots[candidates[index].votes] = n;
            candidates[index].votes += 1;
        }    
        count_votes(numcan);
        voting_print(o);
        --elections;
        if(elections != 0) o << endl;
    }
}

// -------------
//     main
// -------------
int main(int argc, char** argv) {
    voting_solve(cin, cout);
    return 0;
}
