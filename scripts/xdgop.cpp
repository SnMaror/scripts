#include<iostream>
#include "utils/runner.h"

unsigned int MICROSEC = 1000;//1000 000; //Para ms en vez de sec.
static char COMM[]= "xdg-open";

static const char* O_FILE= ".o";
#define O_EXT_LEN 2

//Puts on out the result and returns if should skip it.

int main(int argc, char* argv[]){
     std::string entry;
     char * args[] = { argc>1? argv[1] : &COMM[0]
                      , NULL, NULL};
     
     while(std::cin >> entry){
        args[1] = entry.data();
        
        if(entry.find(O_FILE,entry.length()-O_EXT_LEN) != std::string::npos){
              std::cout << "==Filtered: " << entry << "\n";
              continue;
        }
        
        int ret = Runner::blockexec(args);
        if (ret != 0) {
               std::cout << "==Failed open: "<<entry<<"\n";
        } else{
               std::cout << "==Opened: "<<entry<<"\n";
               usleep(200*MICROSEC);
        }
     }
     
     return 0;
}
