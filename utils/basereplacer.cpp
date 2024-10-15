
#include <iostream>

#include <string>
#include <fstream>

#include "./basereplacer.h"

#include "./rawreplacement.h"

//BaseReplacer class...

// Just reserve some size for def constructor
BaseReplacer::BaseReplacer(){
    //std::cout << "BASE RESERVE"<<"\n";
    replaces.reserve(20);// An initial size.
}

// From stream. Asummed formart == "original replacement" and \n or another space. 
int BaseReplacer::addall(std::istream& repls){
    if(!repls){
      fprintf(stderr, "ERROR NOT VALID ISTREAM for repls\n");
      return 0 ;
    }
    
    int count;
    
    std::string original;
    std::string replacement;
    while(repls >> original >> replacement) { // extract?! For now only raw replacements.
          replaces.push_back(RawReplacement(original.c_str(), 
                                            replacement.c_str()));
          count+=1;
    }    
    
    return count;
}


//For now default is raw replacement.
void BaseReplacer::addrep(const char * original,const char * rep){
     this->replaces.push_back(RawReplacement(original, rep));
}

int BaseReplacer::count() const{
      return this->replaces.size();
}


//Simple begin with, more performant if you know.
std::string BaseReplacer::rep_begin(const char* base) const {
    std::string res(base);
    for(auto rep : this->replaces){
        rep.applyBegin(&res); //its inplace.
    }
    return res;
}

std::string BaseReplacer::rep(const char* base) const {
    //res = std::regex_replace(res, std::regex(rep.original), rep.replacement);
    std::string res(base);
    for(auto rep : this->replaces){
        rep.applyOn(&res); //its inplace.    
    }
    
    return res;
}


// Just get the part.
bool BaseReplacer::nextPart(std::istream& in, std::string& out) const {
     return std::getline (in, out, '\n')? true : false;
}

bool BaseReplacer::extract(std::istream& in, std::string& out, rep_action action) const {
    if(!std::getline (in, out, '\n')){
         return false;
    }
    
    out = (this->*action)(out.c_str())+"\n";
    
    return true;
}

int BaseReplacer::doreplace(std::istream& in, std::string& out, rep_action action) const {
    
    std::string tmp;
    if(!std::getline (in, tmp, '\n')){
         return -1;
    }
    
    out = (this->*action)(tmp.c_str());
    int r = out == tmp? 0 : 1;
    out.append("\n");
    return r;
}

int BaseReplacer::dofind(std::istream& in, std::string& out) const {
    
    if(!std::getline (in, out, '\n')){
         return -1;
    }
    
    std::string res = rep(out.c_str());
    int r = out == res? 0 : 1;
    out.append("\n");
    return r;
}

