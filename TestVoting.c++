// --------------------------------
// TestVoting.c++
// --------------------------------

// https://code.google.com/p/googletest/wiki/V1_7_Primer#Basic_Assertions

// --------
// includes
// --------

#include <iostream> // cout, endl
#include <sstream>  // istringtstream, ostringstream
#include <string>   // ==
#include <utility>  // pair

#include "gtest/gtest.h"

#include "Voting.h"

// -----------
// TestVoting
// -----------

TEST(Voting, solve_1) {
    std::istringstream r("1\n\n2\nA\nB\n1 2\n");
    std::ostringstream w;
    voting_solve(r, w);
    ASSERT_EQ("A\n", w.str());
}
TEST(Voting, solve_2) {
    std::istringstream r("2\n\n2\nA\nB\n1 2\n\n3\na\ns\nd\n1 2 3\n1 2 3\n2 1 3\n2 1 3\n2 1 3\n3 2 1\n3 1 2\n3 1 2\n3 1 2\n3 1 2\n");
    std::ostringstream w;
    voting_solve(r, w);
    ASSERT_EQ("A\n\ns\nd\n", w.str());
}
TEST(Voting, solve_3) {
    std::istringstream r("1\n\n2\nA\nB2345678901234567890123456789012345678901234567890123456789012345678901234567890\n2 1\n");
    std::ostringstream w;
    voting_solve(r, w);
    ASSERT_EQ("B2345678901234567890123456789012345678901234567890123456789012345678901234567890\n", w.str());
}
