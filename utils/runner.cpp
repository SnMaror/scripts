#include<iostream>

#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#include "./runner.h"


// Wait for pid and get return code.
int Runner::waitret(int pid){
      int status;
      waitpid(pid, &status, 0);
      if (WIFSIGNALED(status)) {
      return 1;
      } else if (WIFEXITED(status)) {
      return WEXITSTATUS(status);
      }
      return 0;
}

int Runner::blockexec(const char * rel, char *argc[]){
      int frk = fork();
      if (frk < 0) {
      fprintf(stderr, "Fork for new command exec failed");
      return 1;
      }
      if (frk == 0) {
      
          if(rel != NULL){
		if (chdir(rel) < 0) {
			fprintf(stderr, "ERROR: could not change to directory to %s\n", rel);
			exit(1);
		}
          }
          
          execvp(argc[0], argc);
          exit(errno);  // Hubo error.
      }
      // Wait ALWAYS?!!
      return waitret(frk);
}

int Runner::asyncexec(const char * rel, char *argc[]){
      int frk = fork();
      if (frk < 0) {
      fprintf(stderr, "Fork for new command exec failed");
      return 1;
      }
      if (frk == 0) {
      
          if(rel != NULL){
		if (chdir(rel) < 0) {
			fprintf(stderr, "ERROR: could not change to directory to %s\n", rel);
			exit(1);
		}
          }
          
          execvp(argc[0], argc);
          exit(errno);  // Hubo error.
      }
      // Wait ALWAYS?!!
      return frk;
}

// Run pipe first with argi second with argo params.
int Runner::blockingpipe(char *argi[],char *argo[]){
	int fdPipe[2];
	if (pipe(fdPipe) < 0) {
		fprintf(stderr, "Pipe creation for divide failed\n");
		_exit(EXIT_FAILURE);
	}

	int pidleft = fork();

	if (pidleft < 0) {
		fprintf(stderr, "Fork for left process failed\n");
		_exit(EXIT_FAILURE);
	}

	if (pidleft == 0) {
		close(fdPipe[0]);
		dup2(fdPipe[1], 1);
		close(fdPipe[1]);

                execvp(argi[0], argi);
                exit(errno);  // Hubo error.
	}

	close(fdPipe[1]);

	int pidright = fork();

	if (pidright < 0) {
		fprintf(stderr, "Fork for right process failed\n");
		_exit(EXIT_FAILURE);
	}

	if (pidright == 0) {
		dup2(fdPipe[0], 0);
		close(fdPipe[0]);

                execvp(argo[0], argo);
                exit(errno);  // Hubo error.
	}
	
	close(fdPipe[0]);

	waitret(pidleft);

	return waitret(pidright);
}


// Run pipe first with argi second with argo params.
int Runner::asyncpipe(char *argi[],char *argo[]){
      int frk = fork();
      if (frk < 0) {
           fprintf(stderr, "Fork for new pipe exec failed");
           return 1;
      }
      if (frk == 0) {
          exit(blockingpipe(argi, argo));
      }
      // Wait ALWAYS?!!
      return frk;
}




//char *cwd = getcwd(NULL, 0);
const char * REL_PATH = "./";
const char * PARENT_PATH = "../";

Runner::Runner(): root(REL_PATH){
}

std::string Runner::pwd() const {
     return this->root;
}

bool Runner::cd(const char* relpath){
     this->root.append(relpath);
     return true; // No validation.
}

int Runner::blokingexec(char * argc[]){
      return blockexec(this->root.c_str(), argc);
}

