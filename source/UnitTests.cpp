#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "gtest/gtest.h"

#include "prints.h"
#include "Voting.h"

TEST(Voting, read_cand){
   std::vector<Candidate> cands;
   std::istringstream r("John Doe\nJane Smith\nSirham Sirham");
   read_candidates(r, cands, 3);
   std::stringstream result;
   result << cands;
   ASSERT_EQ(result.str(),"[C(John Doe, 0, 0), C(Jane Smith, 0, 0), C(Sirham Sirham, 0, 0)]"); 
}

TEST(Voting, read_bal){
   std::vector<Ballot> bals;
   std::istringstream r("1 2 3\n2 1 3\n2 3 1\n");
   read_ballots(r, bals);
   std::stringstream result;
   result << bals;
   ASSERT_EQ(result.str(), "[B(0, [0, 1, 2]), B(0, [1, 0, 2]), B(0, [1, 2, 0])]");
}

TEST(Voting, organize){
   std::vector<Candidate> cands;
   std::vector<Ballot> bals;
   
   std::istringstream cr("John Doe\nJane Smith\nSirham Sirham");
   read_candidates(cr, cands, 3);

   std::istringstream br("1 2 3\n2 1 3\n2 3 1\n");
   read_ballots(br, bals);

   organize_ballots(cands, bals);

   std::stringstream result;
   result << cands;
   ASSERT_EQ(result.str(),"[C(John Doe, 0, 1), C(Jane Smith, 0, 2), C(Sirham Sirham, 0, 0)]");
}

TEST(Voting, above_margin1){
   std::vector<Candidate> cands;
   std::vector<Ballot> bals;
   
   std::istringstream cr("John Doe\nJane Smith\nSirham Sirham");
   read_candidates(cr, cands, 3);

   std::istringstream br("1 2 3\n2 1 3\n2 3 1\n");
   read_ballots(br, bals);

   organize_ballots(cands, bals);

   unsigned int margin = bals.size()/2;
   int result = above_margin(cands, margin);
   ASSERT_EQ(result, 1);
}

TEST(Voting, above_margin2){
   std::vector<Candidate> cands;
   std::vector<Ballot> bals;
   
   std::istringstream cr("John Doe\nJane Smith\nSirham Sirham");
   read_candidates(cr, cands, 3);

   std::istringstream br("1 2 3\n2 1 3\n3 2 1\n");
   read_ballots(br, bals);

   organize_ballots(cands, bals);

   unsigned int margin = bals.size()/2;
   int result = above_margin(cands, margin);
   ASSERT_EQ(result, -1);
}

TEST(Voting, above_margin3){
   std::vector<Candidate> cands;
   std::vector<Ballot> bals;
   
   std::istringstream cr("John Doe\nJane Smith");
   read_candidates(cr, cands, 3);

   std::istringstream br("1 2\n2 1\n2 1\n1 2");
   read_ballots(br, bals);

   organize_ballots(cands, bals);

   unsigned int margin = bals.size()/2;
   int result = above_margin(cands, margin);
   ASSERT_EQ(result, -1);
}

TEST(Voting, eliminate_candidates){
   std::vector<Candidate> cands;
   std::vector<Ballot> bals;

   std::istringstream cr("John Doe\nJane Smith\nSirham Sirham\nJohn Smith");
   read_candidates(cr, cands, 4);

   std::istringstream br("1 2 3 4\n2 1 3 4\n2 3 1 4\n");
   read_ballots(br, bals);

   organize_ballots(cands, bals);

   std::vector<Candidate*> elim;
   eliminate_candidates(cands, elim);

   std::stringstream result;
   result << elim;
   ASSERT_EQ(result.str(), "[C(Sirham Sirham, 1, 0), C(John Smith, 1, 0)]");

   std::stringstream cresult;
   cresult << cands;
   ASSERT_EQ(cresult.str(), "[C(John Doe, 0, 1), C(Jane Smith, 0, 2), C(Sirham Sirham, 1, 0), C(John Smith, 1, 0)]");
}

TEST(Voting, redistribute1){
   std::vector<Candidate> cands;
   std::vector<Ballot> bals;

   std::istringstream cr("John Doe\nJane Smith\nSirham Sirham\nJohn Smith");
   read_candidates(cr, cands, 4);

   std::istringstream br("1 2 3 4\n1 2 3 4\n2 1 3 4\n2 3 1 4\n3 2 1 4\n4 2 1 3\n");
   read_ballots(br, bals);

   organize_ballots(cands, bals);

   std::vector<Candidate*> elims;
   eliminate_candidates(cands, elims);

   redistribute_ballots(cands, elims);

   std::stringstream cresult;
   cresult << cands;
   ASSERT_EQ(cresult.str(), "[C(John Doe, 0, 2), C(Jane Smith, 0, 4), C(Sirham Sirham, 1, 0), C(John Smith, 1, 0)]");
}

TEST(Voting, redistribute2){
   std::vector<Candidate> cands;
   std::vector<Ballot> bals;

   std::istringstream cr("John Doe\nJane Smith\nSirham Sirham\nJohn Smith");
   read_candidates(cr, cands, 4);

   std::istringstream br("1 2 3 4\n1 2 3 4\n2 1 3 4\n2 3 1 4\n3 2 1 4\n4 3 2 1\n");
   read_ballots(br, bals);

   organize_ballots(cands, bals);

   std::vector<Candidate*> elims;
   eliminate_candidates(cands, elims);

   redistribute_ballots(cands, elims);

   std::stringstream cresult;
   cresult << cands;
   ASSERT_EQ(cresult.str(), "[C(John Doe, 0, 2), C(Jane Smith, 0, 4), C(Sirham Sirham, 1, 0), C(John Smith, 1, 0)]");
}

TEST(Voting, solve){
   std::istringstream r("1\n\n3\nJohn Doe\nJane Smith\nSirhan Sirhan\n1 2 3\n2 1 3\n2 3 1\n1 2 3\n3 1 2");
   std::ostringstream w;
   voting_solve(r, w);
   ASSERT_EQ("John Doe\n", w.str());
}

TEST(Voting, solve2){
   std::istringstream r("1\n\n3\nJohn Doe\nJane Smith\nSirhan Sirhan\n1 2 3\n2 1 3\n3 2 1");
   std::ostringstream w;
   voting_solve(r, w);
   ASSERT_EQ("John Doe\nJane Smith\nSirhan Sirhan\n", w.str());
}

TEST(Voting, solve3){
   std::istringstream r("1\n\n4\nJohn Doe\nJane Smith\nSirhan Sirhan\nJohn Smith\n1 2 3 4\n2 1 3 4\n3 2 1 4\n1 2 3 4\n2 1 3 4\n4 1 2 3\n");
   std::ostringstream w;
   voting_solve(r, w);
   ASSERT_EQ("John Doe\nJane Smith\n", w.str());
}

TEST(Voting, solve4){
   std::istringstream r("3\n\n3\nJohn Doe\nJane Smith\nSirhan Sirhan\n1 2 3\n2 1 3\n2 3 1\n1 2 3\n3 1 2\n\n"
                        "3\nJohn Doe\nJane Smith\nSirhan Sirhan\n1 2 3\n2 1 3\n3 2 1\n\n"
                        "4\nJohn Doe\nJane Smith\nSirhan Sirhan\nJohn Smith\n1 2 3 4\n2 1 3 4\n3 2 1 4\n1 2 3 4\n2 1 3 4\n4 1 2 3\n");
   std::ostringstream w;
   voting_solve(r, w);
   ASSERT_EQ("John Doe\n\n"
             "John Doe\nJane Smith\nSirhan Sirhan\n\n"
             "John Doe\nJane Smith\n", w.str());
}


