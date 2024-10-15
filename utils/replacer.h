#ifndef LIB_REPLACER_H
#define LIB_REPLACER_H

#include <utility>
#include <fstream>
#include <string>
#include <vector>

#include "./rawreplacement.h"
#include "./basereplacer.h"

std::string begin_rep(const char * path, const char * base);

class Replacer : public BaseReplacer {
private:
     rep_action def_action;
public:
     Replacer(rep_action action, const char* path);     
     Replacer(rep_action action, std::vector<RawReplacement>& _replaces);
     Replacer(rep_action action);
     Replacer();
               
     std::string repreg(const char* original) const;
               
     // Extracts part. Applying default replacement..
     bool extract(std::istream& in, std::string& out) const ;     
};

#endif
