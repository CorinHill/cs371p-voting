#include <cassert>
#include <iostream>
#include <set>

using namespace std;

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

int numbal;
int numcan;
int ballotmatrix[1000][20];
Entrant candidates[20];
int can_ordered[20];
set<int> losers;

void swap_entrants(int a, int b) {
    int tmp = can_ordered[a];
    can_ordered[a] = can_ordered[b];
    can_ordered[b] = tmp;
}

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


void print_winner(ostream& o, int index) {
    assert(index >= 0); assert(index < 20);
    char* c = candidates[index].name;
    while( *c )
        o << *(c++);
    o << endl;
}

void voting_print(ostream& o) {
    int i = 0;
    int v = candidates[can_ordered[0]].votes;
    while(i < 21 && candidates[can_ordered[i]].votes == v) {
        print_winner(o, can_ordered[i]);
        ++i;
    }
}

void reassign_ballots(int loser, int remaining) {
    assert(loser >= 0); assert(loser < 20);
    assert(remaining > 0); assert(remaining < 21);
    for(int it = 0; it < candidates[loser].votes; ++it) {
        for( int i = 0; i < numcan; ++i ) {
            int index = ballotmatrix[candidates[loser].ballots[it]][i] - 1;
            if(losers.count(index)==1)
                continue;
            candidates[index].ballots[candidates[index].votes] = it;
            candidates[index].votes += 1;
            break;
        }
    }
}

int count_votes(int remaining) {
    assert(remaining > 0); assert(remaining < 21);
    quicksort(0, remaining-1);

    int topsize = candidates[can_ordered[0]].votes;
    if (topsize > (numbal>>1) )
        return 1;

    int botsize = candidates[can_ordered[remaining-1]].votes;
    if (topsize == botsize)
        return 0;

    int it = remaining - 1;
    while( candidates[can_ordered[it]].votes == botsize && it > 0 ) {
        losers.insert( can_ordered[it] );
        reassign_ballots(it, --remaining);
        candidates[can_ordered[it]].votes = 0;
        --it;
    }
    return count_votes(remaining);
}

void read_ballot(istream& i) {
    if( !i ) return;
    int index;
    i >> index;

    ballotmatrix[numbal][0] = index--;
    candidates[index].ballots[candidates[index].votes] = numbal;
    candidates[index].ballots[candidates[index].votes + 1] = 0;
    candidates[index].votes += 1;

    for(int c = 1; c < numcan; ++c) {
        i >> ballotmatrix[numbal][c];
        i.ignore(); //space or newline
    }
}

void voting_solve(istream& i, ostream& o) {
    int elections;
    i >> elections;
    i.ignore(); //newline

    while(elections > 0) {
        i >> numcan;
        i.ignore(); //newline
        int n = 0;
        while (n < 20) {
            if(n < numcan) {
                i.getline(candidates[n].name, 81);
                can_ordered[n] = n;
            } else {
                can_ordered[n] = -1;
            }
            candidates[n].votes = 0;
            ++n;
        }
        numbal = 0;
        while(numbal < 1000) {
            if(!i || i.peek() == '\n') break;
            read_ballot(i);
            ++numbal;
        }
        count_votes(numcan);
        voting_print(o);
        --elections;
        if(elections != 0) o << endl;
    }
}

int main(int argc, char** argv) {
    voting_solve(cin, cout);
    return 0;
}
