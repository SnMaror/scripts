//#include <filesystem>
//#include <cstring>
//#include <utility>
//#include <cstdint>
//#include <utility>
//#include <vector>


#include <regex>

#include <iostream>

#include <string>
#include <fstream>

#include "./replacer.h"

#include "./rawreplacement.h"

#define END_LINE_LEN 1

//Static method for simple replacement.
std::string begin_rep(const char * path, const char * base){

    std::string res(base);
    std::ifstream paths(path);
    
    if(!paths){
      fprintf(stderr, "ERROR INTERNAL OPEN FILE\n");
      return res;
    }

    std::string original;
    std::string replacement;
    while(paths >> original >> replacement) { // extract?!
      if (res.rfind(original, 0) != std::string::npos){ //Check only start.
         res.replace(0, original.length(), replacement);
      }
    }
    
    return res;
}


//Replacer class...

// Just reserve some size for def constructor
Replacer::Replacer(BaseReplacer::rep_action action): def_action(action){
    //Base already reserves.
    //replaces.reserve(20);// An initial size.
}

Replacer::Replacer(): def_action(&BaseReplacer::rep) {
}


Replacer::Replacer(BaseReplacer::rep_action action, const char* path) : def_action(action){

    //Base already reserves?!
    // Already initialized just reserver
    //replaces.reserve(20);
    
    std::ifstream paths(path);
    if(!paths){
      fprintf(stderr, "ERROR INTERNAL OPEN FILE\n");
      return;
    }
    
    this->addall(paths);
}

Replacer::Replacer(BaseReplacer::rep_action action, 
                  std::vector<RawReplacement>& _replaces) : BaseReplacer(_replaces), def_action(action){
                  
}

std::string Replacer::repreg(const char* base) const {    
    return rep(base);
}

bool Replacer::extract(std::istream& in, std::string& out) const {
       return this->BaseReplacer::extract(in,out, def_action);
}
