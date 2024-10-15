#include<iostream>
//#include "utils/runner.h"
//#include "utils/replacer.h"
#include <string>
#include <sstream>
#include <vector>

#include <signal.h>

#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>


int waitret(int pid){
      int status;
      waitpid(pid, &status, 0);
      if (WIFSIGNALED(status)) {
      return 1;
      } else if (WIFEXITED(status)) {
      return WEXITSTATUS(status);
      }
      fprintf(stdout, "UNKNOWN?\n");
      return 0;
}

int asyncwait(int pid){
      int status;
      waitpid(pid, &status, WNOHANG);
      
      if (WIFSIGNALED(status)) {
      return 1;
      } else if (WIFEXITED(status)) {
      return WEXITSTATUS(status);
      }
      fprintf(stdout, "UNKNOWN WKNP?\n");
      return 0;
}




void handle_end(int num){
    fprintf(stderr, "EXITED ABRUPTLY?! %d\n",num);

}

void sethandler(){
     struct sigaction sa;
     sa.sa_handler = &handle_end;
     sigemptyset(&sa.sa_mask);
     sa.sa_flags = 0;
     if(sigaction(SIGCHLD, &sa, NULL) < 0){
          _exit(1);
     }
}

unsigned int MILLISEC = 1000;//1000 000; //Para ms en vez de sec.

void procesoHijo(){
     fprintf(stdout, "INIT CHILD?! \n");
     usleep(200*MILLISEC);
     fprintf(stdout, "--> end CHILD?! \n");
    _exit(0);
}

void procesoPadre(){
    int frk = fork();
    if(frk < 0){
         fprintf(stderr, "FAILED FORK\n");
         _exit(1);
    }
    
    if(frk == 0){
         procesoHijo();
    }    
    // wait child?
    //int r = asyncwait(frk);
    int r = getpid();
    fprintf(stdout, "--> end PARENT?!?! %d \n", r);
    _exit(0);
}


int main(int argc, char* argv[]){
     sethandler();
     
     int frk = fork();
     if(frk < 0){
         fprintf(stderr, "FAILED FORK\n");
         _exit(1);
     }
     
     
     if(frk == 0){
         procesoPadre();
     }
     fprintf(stdout, "--> MAIN WAIT PARENT?!?! \n");
     
     int r = waitret(frk);
     fprintf(stdout, "--> MAIN WAIT END ?!?! %d \n", r);
     
     usleep(1100*MILLISEC);
     fprintf(stdout, "--> MAIN END ?!?!\n");
     
     return 0;
}
