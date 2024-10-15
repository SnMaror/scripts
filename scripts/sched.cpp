#include<iostream>
#include<unistd.h>
#include<vector>
#include<cstring>
#include<sstream>
#include "utils/runner.h"

const char* FLAG_IN = "-i";

const char* FLAG_PIPE = "-p";

std::string buildCommand(char ** argv, int len){
     std::stringstream comm;     
     comm << argv[0];
     
     int ind = 1;
     while(ind < len){
          //printf("ARG %d :: %s\n", ind, *(argv+ind));
          comm << " " << *(argv+ind);
          ind+=1;
     }
    
     
     
     return comm.str();

}

void runpipe(std::vector<int>& clients, int first, int argc, char* argv[]){
     char* argcl[2] = {NULL, NULL};
     
     int len = argc-first;
     if(len == 0){
         std::cerr << "No parameters passed for piped\n";
         return;
     }
     
     char ** argout = (char**) malloc(sizeof(char*)*(len+1));
     
     *(argout) = argv[first];
     
     int ind =1;
     while(ind < len){
          *(argout+ind) = argv[ind + first];
          ind+=1;
     }
     *(argout+ind) = NULL;
          
     std::string oth= buildCommand(argout, len);
     std::string file;
     ind = 1;
     while(std::cin>> file){
            argcl[0] = file.data();
            printf("Client %d TO: %s | %s\n", ind, argcl[0], oth.c_str());
            fflush(stdout);
            
            clients.push_back(Runner::asyncpipe(argcl, argout));
            ind+=1;
     }
     
     free(argout);
     
}


void runargv(std::vector<int>& clients, int argc, char* argv[]){
     char* argcl[2] = {NULL, NULL};
     for(int ind = 1; ind < argc; ind++){
            argcl[0] = argv[ind];
            printf("Client %d TO: %s\n", ind, argcl[0]);
            fflush(stdout);
            clients.push_back(Runner::asyncexec(NULL,argcl));
     }
}
void runstdin(std::vector<int>& clients){ // int argc, char* argv[]
     char* argcl[2] = {NULL, NULL};
     std::string file;
     int ind = 1;
     while(std::cin>> file){
            argcl[0] = file.data();
            printf("Client %d TO: %s\n", ind, argcl[0]);
            fflush(stdout);
            clients.push_back(Runner::asyncexec(NULL, argcl));
            ind+=1;
     }
}

int main(int argc, char* argv[]){
     if(argc == 1){
          printf("Nothing to execute...\n use -i if you want from stdin the scripts.");
          return 0;
     }
     std::vector<int> clients;
     
     if(strncmp(argv[1], FLAG_IN,2) == 0){
          std::cout << "Run from stdin\n";
          runstdin(clients);
     } else {
         if(strncmp(argv[1], FLAG_PIPE,2) == 0){
              std::cout << "Run piped from stdin\n";
              runpipe(clients, 2, argc, argv);
          } else{
              std::cout << "Run from argv\n";
              runargv(clients, argc, argv);
          }
     }
      //WAIT!
      int ind = 1;
      for (auto clientit = clients.begin(); clientit  != clients.end();)
      {
             //std::cout << "DISCONN "<< playerit->first<<"\n";
             int ret = Runner::waitret(*clientit);
             
             printf("Client %d exited: %d\n", ind, ret);
             
             ind++;
             ++clientit;
      }
      
      return 0;
}
