#ifndef LIB_REPLACEMENT_H
#define LIB_REPLACEMENT_H

#include <string>

// Define Base replacement... interface basically.
// But it includes a simple restriction. Target of things is apply on String. 
// Wether you replace strings directly or with regex. 
class Replacement {
public:
     // Default constructor.
     Replacement(){}
     
     virtual void applyOn(std::string* base) = 0;
     virtual void applyBegin(std::string* base)= 0;
     virtual void applyEnd(std::string* base)= 0;
};
#endif
