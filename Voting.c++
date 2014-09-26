// --------
// includes
// --------
#include <cassert>
#include <iostream>
#include <cstring>
#include <sstream>
#include <set>

#include "Voting.h"

using namespace std;
// -------------------------
// Entrant class definitions
// -------------------------
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

void swap_entrants(int a, int b) {
    if( a==b ) return;
    int tmp = can_ordered[a];
    can_ordered[a] = can_ordered[b];
    can_ordered[b] = tmp;
}

// ---------
// quicksort
// ---------

void quicksort(int s, int e) {
    if(s < e) {
        int i = s, j = s;
        while( i < e ) {
            if( candidates[can_ordered[i]] > candidates[can_ordered[e]] ) {
                swap_entrants(i, j);
                ++j;
            }
            ++i;
        }
        swap_entrants(e, j);
        quicksort(s, j-1);
        quicksort(j+1, e);
    }
}

// ------------
// print_winner
// ------------

void print_winner(ostream& o, int slot) {
    assert(slot >= 0); assert(slot < 20);
    char* c = candidates[slot].name;
    while( *c ) {
        o.put(*c);
        c++;
    }
    o << endl;
}

// ------------
// voting_print
// ------------

void voting_print(ostream& o) {
    quicksort(0, numcan-1);
    int slot = 0;
    int v = candidates[can_ordered[0]].votes;
    while(slot < numcan) {
        if( candidates[slot].votes == v )
            print_winner(o, slot);
        ++slot;
    }
}

// ----------------
// reassign_ballots
// ----------------

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
        --it;
    }
    int r = remaining;
    while( ++it < r ) {
        losers.insert( can_ordered[it] );
        reassign_ballots(can_ordered[it], --remaining);
        candidates[can_ordered[it]].votes = 0;
    }
    count_votes(remaining);
}

// -----------
// voting_read
// -----------

void voting_read(istream& i) {
    i >> numcan;
    i.ignore();
    int n = 0, m = 0;
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

void voting_solve(istream& i, ostream& o) {
    int elections;
    i >> elections;
    while(elections > 0) {
        losers.clear();
        voting_read(i);
        for( int n = 0; n < numbal; ++n ) {
            int index = ballotmatrix[n][0] - 1;
            candidates[index].ballots[candidates[index].votes] = n;
            candidates[index].votes += 1;
        }    
        count_votes(numcan);
        voting_print(o);
        --elections;
        if(elections != 0) o << endl;
    }
}
