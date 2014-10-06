
#ifndef __Voting__
#define __Voting__

#include <iostream>
#include <vector>

class Ballot {
public:
   unsigned int m_currentPreference;
   std::vector<int> m_preferences;
   Ballot(){
      m_currentPreference = 0;
      m_preferences.reserve(20);
   }

   int getPreference(){
      return m_preferences[m_currentPreference];
   }

   int nextPreference(){
      return m_preferences[++m_currentPreference];
   }

   friend std::ostream& operator<<(std::ostream& out, const Ballot& b){
      out << "B(" << b.m_currentPreference << ", [";
      if(b.m_preferences.size() > 0){
         for(unsigned int i = 0; i < b.m_preferences.size()-1; ++i){
            out << b.m_preferences[i] << ", ";
         }
         out << b.m_preferences[b.m_preferences.size()-1];
      }
      return out << "])";
   }
};

class Candidate {
public:
   std::string m_name;
   bool m_eliminated;
   std::vector<Ballot*> m_ballots;

   Candidate(){
      m_eliminated = false;
      m_ballots.reserve(1000);
   }

   unsigned int numberOfBallots(){
      return m_ballots.size();
   }

   friend std::ostream& operator<<(std::ostream& strm, const Candidate& c){
      return strm << "C(" << c.m_name << ", " << c.m_eliminated << ", " << c.m_ballots.size() << ")";
   }
};

void voting_solve(std::istream& in, std::ostream& out);
void read_candidates(std::istream& in, std::vector<Candidate>& c, int num);
void read_ballots(std::istream& in, std::vector<Ballot>& b);
int above_margin(std::vector<Candidate>& cands, unsigned int margin);
int eliminate_candidates(std::vector<Candidate>& cands, std::vector<Candidate*>& elim);
void redistribute_ballots(std::vector<Candidate>& cands, std::vector<Candidate*>& elims);
int find_winners(std::vector<Candidate>& cands, unsigned int margin, std::vector<Candidate*>& winners);
void organize_ballots(std::vector<Candidate>& cands, std::vector<Ballot>& bals);

#endif
