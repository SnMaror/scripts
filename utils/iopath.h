#ifndef LIB_IOPATH_H
#define LIB_IOPATH_H

//#include <filesystem>
//#include <cstring>
//#include <utility>
//#include <cstdint>

#include <string>
#include <utility>
#include <fstream>

#include "parsing.h"


class IOPath {
private: 
     char* path;
public:
     IOPath(const char* _path) : path(_path){}
};
#endif
