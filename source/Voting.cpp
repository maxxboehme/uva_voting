#include <iostream>
#include <vector>

class Ballot {
public:
   int m_currentPreference;
   std::vector<int> m_preferences;

   Ballot(){
      m_currentPreference = 0;
   }

   int getPreference(){
      return m_preference[m_currentPreference];
   }

   int nextPreference(){
      return m_preference[++m_currentPreference];
   }

   virtual ~Ballot(){};
};

class Candidate {
   std::string m_name;
   std::vector<Ballot*> m_ballets;
}

void voting_solve(std::isteam& in, std::ostream& out){
   const int numberOfCases;
   in >> numberOfCases;
   
   int numberOfCandidates;
   for(int i = 0; i < numberOfCases; ++i){
      in >> numberOfCandidates;

   }
}
