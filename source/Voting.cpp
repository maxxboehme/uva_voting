#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <list>

#include "Voting.h"
#include "prints.h"

void read_candidates(std::istream& in, std::vector<Candidate>& candidates, int numberOfCandidates){
   std::string line;
   int i = 0;
   while(i < numberOfCandidates){
      Candidate c;
      getline(in, line);
      if(line != ""){
         c.m_name = line;
         candidates.push_back(c);
         ++i;
      }
   }
}

void read_candidates2(std::istream& in, std::vector<Candidate>& candidates, int numberOfCandidates, std::list<Candidate*>& validCands){
   std::string line;
   int i = 0;
   while(i < numberOfCandidates){
      Candidate c;
      getline(in, line);
      if(line != ""){
         c.m_name = line;
         candidates.push_back(c);
         validCands.push_back(&candidates.back());
         ++i;
      }
   }
}

void read_ballots(std::istream& in, std::vector<Ballot>& ballots){
   std::string line;
   while(getline(in, line)){
      if(line == "")
         break;
      std::istringstream isstream(line);
      Ballot b;
      unsigned int i;
      while(isstream >> i){
         b.m_preferences.push_back(i-1);
      }
      ballots.push_back(b);
   }
}

void organize_ballots(std::vector<Candidate>& cands, std::vector<Ballot>& bals){
   for(unsigned int i = 0; i < bals.size(); ++i){
      cands[bals[i].getPreference()].m_ballots.push_back(&bals[i]);
   }
}

int above_margin(std::vector<Candidate>& cands, unsigned int margin){
   for(unsigned int i = 0; i < cands.size(); ++i){
      if(cands[i].numberOfBallots() > margin){
         return i;
      }
   }
   return -1;
}

Candidate* above_margin2(std::list<Candidate*>& cands, unsigned int margin){
   std::list<Candidate*>::iterator it = cands.begin();
   std::list<Candidate*>::iterator end = cands.end();
   for(; it != end; ++it){
      if((*it)->numberOfBallots() > margin){
         return *it;
      }
   }
   return 0;
}

int eliminate_candidate2(std::list<Candidate*>& validCands, std::vector<Candidate*>& elim){
   unsigned int max = 0;
   unsigned int min = 1000;
   
   std::list<Candidate*>::iterator it = validCands.begin();
   std::list<Candidate*>::iterator end = validCands.end();
   for(; it != end; ++it){
      unsigned int numBals = (*it)->numberOfBallots();
      if(numBals < min){
         min = numBals;
      }
      if(numBals > max){
         max = numBals;
      }
   }

   if(max  == min){
      return 0;
   }

   it = validCands.begin();
   while(it != validCands.end()){
      if((*it)->numberOfBallots() == min){
         elim.push_back(*it);
         (*it)->m_eliminated = true;
         it = validCands.erase(it);
      } else {
         ++it;
      }
   }
   return elim.size();
}

int eliminate_candidates(std::vector<Candidate>& cands, std::vector<Candidate*>& elim){
   unsigned int max = 0;
   unsigned int min = 1000;
   for(unsigned int i = 0; i < cands.size(); ++i){
      if(cands[i].m_eliminated)
         continue;

      unsigned int numBals = cands[i].numberOfBallots();
      if(numBals < min){
         min = numBals;
      }
      if(numBals > max){
         max = numBals;
      }
   }

   if(max  == min){
      return 0;
   }

   for(unsigned int i = 0; i < cands.size(); ++i){
      if(cands[i].numberOfBallots() == min){
         elim.push_back(&cands[i]);
         cands[i].m_eliminated = true;
      }
   }
   return elim.size();
}

void redistribute_ballots(std::vector<Candidate>& cands, std::vector<Candidate*>& elims){
   for(unsigned int i = 0; i < elims.size(); ++i){
      std::vector<Ballot*>& bals = elims[i]->m_ballots;
      for(unsigned int j = 0; j < bals.size(); ++j){
         while(cands[bals[j]->nextPreference()].m_eliminated);
         cands[bals[j]->getPreference()].m_ballots.push_back(bals[j]);
      }
      bals.clear();
   }
}

void redistribute_ballots2(std::vector<Candidate>& cands, std::vector<Candidate*>& elims){
   std::vector<Candidate*>::iterator elimsb = elims.begin();
   std::vector<Candidate*>::iterator elimse = elims.end();
   while(elimsb != elimse){
      std::vector<Ballot*>& bals = (*elimsb)->m_ballots;
      std::vector<Ballot*>::iterator balsb = bals.begin();
      std::vector<Ballot*>::iterator balse = bals.end();
      while(balsb != balse){
         while(cands[(*balsb)->nextPreference()].m_eliminated);
         cands[(*balsb)->getPreference()].m_ballots.push_back(*balsb);
         ++balsb;
      }
      bals.clear();
      ++elimsb;
   }
}

int find_winners(std::vector<Candidate>& cands, unsigned int margin, std::vector<Candidate*>& winners){
   while(true){
       int above = above_margin(cands, margin);
       if(above >= 0){
          winners.push_back(&cands[above]);
          return 1;
       }
       std::vector<Candidate*> elim;
       elim.reserve(20);
       int result = eliminate_candidates(cands, elim);
       if(!result){
          for(unsigned int i = 0; i < cands.size(); ++i){
             if(!cands[i].m_eliminated){
                winners.push_back(&cands[i]);
             }
          }
          return winners.size();
       }
       redistribute_ballots(cands, elim);
   }
}

int find_winners2(std::vector<Candidate>& cands, unsigned int margin, std::list<Candidate*>& validCands, std::vector<Candidate*>& winners){
   while(true){
       Candidate* above = above_margin2(validCands, margin);
       if(above){
          winners.push_back(above);
          return 1;
       }
       std::vector<Candidate*> elim;
       elim.reserve(20);
       int result = eliminate_candidate2(validCands, elim);
       if(!result){
          for(std::list<Candidate*>::iterator it = validCands.begin(); it != validCands.end(); ++it){
             winners.push_back(*it);
          }
          return winners.size();
       }
       redistribute_ballots(cands, elim);
   }
}

void voting_solve2(std::istream& in, std::ostream& out){
   int numberOfCases = 0;
   in >> numberOfCases;
   
   std::vector<Ballot> ballots;
   ballots.reserve(1000);

   std::vector<Candidate> candidates;
   candidates.reserve(20);
   
   int numberOfCandidates = 0;
   for(int i = 0; i < numberOfCases; ++i){
      in >> numberOfCandidates;
      read_candidates(in, candidates, numberOfCandidates);
      read_ballots(in, ballots);
      organize_ballots(candidates, ballots);
      unsigned int margin = ballots.size()/2;
      std::vector<Candidate*> winners;
      find_winners(candidates, margin, winners);
      for(unsigned int j = 0; j < winners.size(); ++j){
         out << winners[j]->m_name << std::endl;
      }
      if(i < (numberOfCases - 1)){
         out << std::endl;
      }
      ballots.clear();
      candidates.clear();
   }
}

void voting_solve(std::istream& in, std::ostream& out){
   int numberOfCases = 0;
   in >> numberOfCases;
   
   std::vector<Ballot> ballots;
   ballots.reserve(1000);

   std::vector<Candidate> candidates;
   candidates.reserve(20);
   
   int numberOfCandidates = 0;
   for(int i = 0; i < numberOfCases; ++i){
      in >> numberOfCandidates;
      std::list<Candidate*> validCands;
      read_candidates2(in, candidates, numberOfCandidates, validCands);
      read_ballots(in, ballots);
      organize_ballots(candidates, ballots);
      unsigned int margin = ballots.size()/2;
      std::vector<Candidate*> winners;
      find_winners2(candidates, margin, validCands, winners);
      for(unsigned int j = 0; j < winners.size(); ++j){
         out << winners[j]->m_name << std::endl;
      }
      if(i < (numberOfCases - 1)){
         out << std::endl;
      }
      ballots.clear();
      candidates.clear();
   }
}
