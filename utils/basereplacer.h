#ifndef LIB_BASE_REPLACER_H
#define LIB_BASE_REPLACER_H

#include <utility>
#include <fstream>
#include <string>
#include <vector>

#include "./rawreplacement.h"


//typedef bool (*line_tester) (const std::string& line);
//typedef bool (*line_parser) (std::string& line);

class BaseReplacer {
private: 
     std::vector<RawReplacement> replaces;
public:
     BaseReplacer(std::vector<RawReplacement>& _replaces): replaces(std::move(_replaces)){}
     BaseReplacer();
     
     void addrep(const char * original, const char * rep);
     int addall(std::istream& in);
     int count() const;
     
     // Single replacer actions. 
     std::string rep_begin(const char* original) const;
     std::string rep(const char* original) const;
     virtual std::string repreg(const char* original) const = 0;
      
     
     typedef std::string (BaseReplacer::*rep_action) (const char* base) const;
     
     // Just obtain next part. NO replacing.
     bool nextPart(std::istream& in, std::string& out) const ;
     
     // Extracts part. Applying default replacement..
     virtual bool extract(std::istream& in, std::string& out) const = 0;
     
     // Extracts part. And applies action.
     bool extract(std::istream& in, std::string& out, rep_action action) const;
  
     //Returns quantity of replacements or -1 if eof.
     int doreplace(std::istream& in, std::string& out, rep_action action) const;
     int dofind(std::istream& in, std::string& out) const;
};

#endif
