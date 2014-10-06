
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

   Ballot& operator=(const Ballot& other){
     if(this != &other){
        m_currentPreference = other.m_currentPreference;
        m_preferences.clear();
        for(unsigned int i = 0; i < other.m_preferences.size(); ++i){
           m_preferences.push_back(other.m_preferences[i]);
        }
      }
      return *this;
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
   
   Candidate& operator=(const Candidate& other){
     if(this != &other){
        m_name = other.m_name;
        m_eliminated = other.m_eliminated;
        m_ballots.clear();
        for(unsigned int i = 0; i < other.m_ballots.size(); ++i){
           m_ballots.push_back(other.m_ballots[i]);
        }
      }
      return *this;
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

/*
void read_candidatesA(std::istream& in, Candidate* candidates, int numberOfCandidates);
unsigned int read_ballotsA(std::istream& in, Ballot* ballots);
void organize_ballotsA(Candidate* cands, Ballot* bals, unsigned int balSize);
int above_marginA(Candidate* cands, unsigned int candSize, unsigned int margin);
int eliminate_candidatesA(Candidate* cands, unsigned int candSize, std::vector<Candidate*>& elim);
void redistribute_ballotsA(Candidate* cands, std::vector<Candidate*>& elims);*/
#endif
