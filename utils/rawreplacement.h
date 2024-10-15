#ifndef LIB_RAW_REPLACEMENT_H
#define LIB_RAW_REPLACEMENT_H

#include <string>
#include <cstring>
#include "./replacement.h"

// Define Base replacement... interface basically.
// But it includes a simple restriction. Target of things is String. 
// Wether you replace strings directly or with regex. 
class RawReplacement : public Replacement {
protected:
     std::string old_str;
     std::string new_str;
     int old_len;
public:

     // Default constructor.
     RawReplacement(const char* _old_str, const char* _new_str);
     
     void applyOn(std::string* base) override;
     void applyBegin(std::string* base) override;
     void applyEnd(std::string* base) override;
};

#endif
