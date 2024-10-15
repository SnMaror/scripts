#include <fstream>
#include <iostream>
#include <string>
#include "utils/replacer.h"

int main(int argc, const char* argv[]) {
    if(argc <=2){
      fprintf(stderr,"ERROR No original path arg or paths file.. <src replaces> <uri> are required.\n");
      return -1;
    }
    std::cout << begin_rep(argv[1], argv[2]);
    
    return 0;
}
