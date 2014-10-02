#include <sstream>
#include "gtest/gtest.h"

#include "Voting.h"




TEST(Voting, solve){
   std::istringstream r("");
   std::ostringstream w;
   voting_solve(r, w);
   ASSERT_EQ("", w.str());
}
