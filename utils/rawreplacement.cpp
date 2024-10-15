#include <string>
//#include <iostream>
#include "./rawreplacement.h"


RawReplacement::RawReplacement(const char* _old_str, const char* _new_str): old_str(_old_str), new_str(_new_str) {
     this->old_len = old_str.length(); //cache it? just in case...
}

// Can be there multiple times?
void RawReplacement::applyOn(std::string* base){
       size_t lstfound = base->find(this->old_str); 
       //std::cout << "FOUND IND?!"<< lstfound << " FOR REP"<< this->old_str<<"\n";
       // Each find, has to add to lst found for searching different ocurrences. 
       int step = this->new_str.length();
       
       while (lstfound != std::string::npos){
             base->replace(lstfound, this->old_len, this->new_str);
             
             lstfound =  base->find(this->old_str, lstfound+step); //check again.But avoid appended.
       }
}


// Not needed to iterate.
void RawReplacement::applyBegin(std::string* base){
      if (base->rfind(this->old_str, 0) != std::string::npos){ //Check only start.
         base->replace(0, this->old_len, this->new_str);
      }
}

void RawReplacement::applyEnd(std::string* base){
      if (base->find(this->old_str, base->length() - this->old_len) != std::string::npos){ //Check only end.
         base->replace(base->length() - this->old_len, this->old_len, this->new_str);
      }
}

