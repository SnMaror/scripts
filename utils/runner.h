#ifndef LIB_RUNNER_H
#define LIB_RUNNER_H
/*
#include<iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

*/

#include <string>

class Runner {
private:
      std::string root;
public:
      // General utility methods. 
      // Wait for pid and get return code.
      static int waitret(int pid);
      
      //Run on child and wait blocking and wait 
      static int blockexec(const char* rel, char *argc[]);
      static int asyncexec(const char* rel, char *argc[]);
      
      static int asyncpipe(char *argi[], char *argo[]);
      static int blockingpipe(char *argi[], char *argo[]);
      
      
      Runner(const char * _root): root(_root){}
      Runner();
      
      std::string pwd() const;
      bool cd(const char* relpath);
      
      int blokingexec(char * argc[]);
};

#endif
