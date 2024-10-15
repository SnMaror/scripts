#include<iostream>

#include <fstream>
#include <cstring>
#include <unistd.h>


#include "utils/replacer.h"
#include "utils/mcontent.h"
#include <signal.h>

//Puts on out the result and returns if should skip it.
// Flag to use line begin rep, instead of general one. 
const char* FLAG_BEGIN_REP = "-b";


const char* FLAG_AUTO_TEMP = "-a";

// Flag to use argv for replacements,
// input for file content. Instead of file path
const char* FLAG_IN_OUT = "-i";

const char* TEMP_FILE_NAME = "./_temp_mod_XXXXXX";

// global variable for deleting on interrupt.
static MContent* contentm;
void handler_ctrl_c(int num){
    fprintf(stderr, "EXITED ABRUPTLY?! %d\n",num);
    delete contentm;
    exit(1);
}

int setCtrlCHandler(){
    struct sigaction sa;

    sa.sa_handler = &handler_ctrl_c;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART; /* Restart functions if
                                 interrupted by handler */
    return sigaction(SIGINT, &sa, NULL);
}




BaseReplacer::rep_action resolveAction(int* ind_start, char* argv[]){
     if(strncmp(argv[*ind_start], FLAG_BEGIN_REP, 2) == 0){
           *ind_start += 1; // used arg.
           return &BaseReplacer::rep_begin;
     }
     
     return &BaseReplacer::rep;     
}



void replacefiles(BaseReplacer& replacer){
     //Malloc?!
     //MContent contentm(TEMP_FILE_NAME);
     //contentm = new MContent(TEMP_FILE_NAME);
     std::string entry;
     
     while(std::cin >> entry){
          if(!contentm->rewrite(entry.c_str(), replacer)){
               fprintf(stderr, "Failed to rewrite %s",entry.c_str());
          }
          std::cout << "FINISHED rewriting "<< entry << "\n";
     }

}

int main(int argc, char* argv[]){
     if(argc == 1){
          std::cout << "No arguments passed...\n"
               << ">fmod <out>\n"
               << "Rreceives from stdin a file and copies it to out.\n\n"
               << ">fmod [-b] <pairs...>\n"
               << "receiving pairs to replace from argv <old> <new>. Reads filepath from stdin, and modifies its content making use of tmp as temporary file\n\n"
               << "you can add flag -b to only replace if line begins with pair old str\n";
          return 0;
     }
     
     if(argc == 2){ //Just copy from passed file to tmp.
        std::string pathTmp(argv[1]);
        std::string src;
        while(std::cin >> src){
             if(!MContent::copy(src.c_str(), pathTmp.c_str())){
                   fprintf(stderr, "Failed to write %s to %s", src.c_str(), pathTmp.c_str());
                   return 1;
             }
        }
     }
     
     //Automatic tmp handling.
     
     int ind_start = 1;
     Replacer replacer(resolveAction(&ind_start, argv));
     //BaseReplacer::rep_action action = ;
          
     // If not repls from stdin, then assumed argv are replacements.     
     while(ind_start < argc-1){ // This ensures 2 elems. begin and begin+1
          printf("NEW PAIR '%s' to '%s'\n",argv[ind_start], argv[ind_start+1]);
          replacer.addrep(argv[ind_start], argv[ind_start+1]);
          ind_start+=2; // 2 args used.
     }
     
     setCtrlCHandler(); // So we remove tmp file.?
     contentm = new MContent(TEMP_FILE_NAME);

     replacefiles(replacer);
     fprintf(stdout, "EXITED NORMALLY?! \n");
     delete contentm; //EXIT NORMALLY.
     
     return 0;
}
