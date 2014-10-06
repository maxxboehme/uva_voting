
#ifndef __PRINTS__
#define __PRINTS__

#include "Voting.h"
#include <iostream>
#include <vector>
#include <list>

template <typename T>
inline std::ostream& operator<<(std::ostream& out, std::vector<T> v){
   out << "[";
   if(v.size() > 0){
      for(unsigned int i = 0; i < v.size()-1; ++i){
         out << v[i] << ", ";
      }
      out << v[v.size()-1];
   }
   return out << "]";
}

template <typename T>
inline std::ostream& ArrayToStream(std::ostream& out, T* v, unsigned int size){
   out << "[";
   if(size > 0){
      for(unsigned int i = 0; i < size-1; ++i){
         out << v[i] << ", ";
      }
      out << v[size-1];
   }
   return out << "]";
}

inline std::ostream& operator<<(std::ostream& out, std::vector<Candidate*> v){
   out << "[";
   if(v.size() > 0){
      for(unsigned int i = 0; i < v.size()-1; ++i){
         out << *v[i] << ", ";
      }
      out << *v[v.size()-1];
   }
   return out << "]";
}

inline std::ostream& operator<<(std::ostream& out, std::list<Candidate*> v){
   out << "[";
   for(std::list<Candidate*>::iterator it = v.begin(); it != v.end(); ++it){
      out << **it << ", ";
   }
   return out << "]";
}

inline std::ostream& operator<<(std::ostream& out, std::vector<Ballot*> v){
   out << "[";
   if(v.size() > 0){
      for(unsigned int i = 0; i < v.size()-1; ++i){
         out << *v[i] << ", ";
      }
      out << *v[v.size()-1];
   }
   return out << "]";
}

#endif
