#include<iostream>

#include <fstream>
#include <cstring>
#include <unistd.h>


#include "utils/replacer.h"
#include "utils/mcontent.h"

#include <signal.h>

const char FLAG_LIB_F = 'l';
const char FLAG_LIB_D = 'd';
const char FLAG_EXEC = 'e';

const char modeinit = '-';
static const char* REL_SRC = "./";

void initmake(const char * name, Replacer& replacer, const char* src){
   std::cout << "cmake_minimum_required(VERSION 2.4)\n";
   std::cout << "project("<< name << ")\n";
   
   
   
   std::cout << "set(SRC_CODE ${PROJECT_SOURCE_DIR}";
   
   if(src != NULL && strncmp(REL_SRC, src, 2) != 0){
        std::cout << "/" << src;
   }
   
   std::cout << ")\n";
   
   replacer.addrep(REL_SRC, "${SRC_CODE}/");
}

void addlibf(const char * name, Replacer& rep){
     std::cout << "add_library(${PROJECT_NAME}"//<< proj
     << " "<< rep.rep_begin(name) <<")\n";
}

int main(int argc, char* argv[]){
     if(argc < 3){
         std::cerr << "No type of cmake passed. -l for library files\n -d for directories of libraries -e for executable\n";
         std::cerr << "Second parameter is name of library/project.\n";
         return 1;
     }
     
     if(strlen(argv[1]) != 2 || *(argv[1]) != modeinit){
          std::cerr << "Invalid type of cmake passed. -l for library files\n -d for directories of libraries -e for executable\n";
          return 1;
     }
     char mode = *(argv[1]+1);
     

     if(mode == FLAG_LIB_F){
           Replacer replacer;
           
           initmake(argv[2], replacer, NULL);
           std::string file;
           while(std::cin >> file){
                addlibf(file.c_str(), replacer);
           }
           return 0;
     }
     
     
     
     
     
     std::cerr << "Not implemented mode " << mode << "\n";
     return 1;
}
