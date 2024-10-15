#include<iostream>

#include "utils/replacer.h"
#include "utils/mcontent.h"
#include <fstream>
#include <cstring>


typedef bool (*mode_act)(std::string file, BaseReplacer& repl, std::ostream& out);

bool replacefrom(std::string file, BaseReplacer& repl, std::ostream& out)
{
    std::ifstream in(file.c_str());
    
    if(!in){
      fprintf(stderr, "ERROR INTERNAL OPEN IN FILE %s\n", file.c_str());
      return false;
    }
    std::string part;
    while(repl.extract(in , part)){
         out << part;
    }
    return true;
}


bool linesFilter(std::string file, BaseReplacer& repl, std::ostream& out)
{
    //fprintf(stderr, "LINE FILTER?!\n");
    std::ifstream in(file.c_str());
    
    if(!in){
      fprintf(stderr, "ERROR INTERNAL OPEN IN FILE %s\n", file.c_str());
      return false;
    }
    std::string part;
    int line = 1;
    int res = repl.doreplace(in , part, &BaseReplacer::rep);
    while(res >=0 ){
           if(res >0){
               out << file << ": "<< line<< " " << part;
           }
           line+=1;
           res = repl.doreplace(in , part, &BaseReplacer::rep);
    }
    return true;
}

bool filterPaths(std::string file, BaseReplacer& repl, std::ostream& out)
{
    std::ifstream in(file.c_str());
    
    if(!in){
      fprintf(stderr, "ERROR INTERNAL OPEN IN FILE %s\n", file.c_str());
      return false;
    }
    //std::cerr << "FILTER FILE?! " << file << std::endl;
    std::string part;
    int line = 1;
    int res = repl.dofind(in , part);
    bool found = false;
    while(res >=0){ //While did not find nor ended.
           if(res >0){
               found = true;
               std::cerr << file << ": "<< line<< " " << part;
           }
           line+=1;
           res = repl.dofind(in , part);
    }
    
    if(found){ //Found
         out << file<< std::endl;
         std::cerr<<"-------" << std::endl;
    //} else{
    //     std::cerr << "NOt found on "<< file << std::endl;
    }
    return true;
}








//Puts on out the result and returns if should skip it.

// Flag for receiving pairs to replace from stdin. and files input from argv. 
const char* FLAG_STDIN_PAIR = "-io"; 


// Flag to use line begin rep, instead of general one. 
const char* FLAG_BEGIN_REP = "-b";

// Flag to use argv for replacements,
// input for file content. Instead of file path
const char FLAG_MODE = '-';

const char M_STD = 'i';

const char CH_LINE_S = 'l';
const char CH_SEARCH_F = 's';

const char* REMOVE_REPL = "";


BaseReplacer::rep_action resolveAction(int* ind_start, char* argv[]){
     if(strncmp(argv[*ind_start], FLAG_BEGIN_REP, 2) == 0){
           *ind_start += 1; // used arg.
           return &BaseReplacer::rep_begin;
     }
     
     return &BaseReplacer::rep;     
}

int iter_stdin_files(Replacer& repl, mode_act action){
     std::string entry;
     
     while(std::cin >> entry){
        if (!action(entry,repl, std::cout)) {
               fprintf(stderr, "Failed exec of command!\n");
               return 1;
        }
     }
     return 0;
}

int main(int argc, char* argv[]){
     if(argc == 1){
          std::cout << "No arguments passed. usage is:\n"
               << ">frep -f <files...>\n"
               << "receiving pairs to replace from stdin formatted. and files input from argv.\n\n"
               << ">frep -i <pairs...>\n"
               << "receiving pairs to replace from argv <old> <new>. And lines from stdin\n\n"
               << ">frep -[mode] <elements...>\n"
               << "receiving elements that will pairs or single elems depending on mode.\n"
               << "Reads file paths from line on stdin, which will be read by line\n\n"
               << "mode 'l' for replacing/filtering lines from pairs\n"
               << "Showing only replaced lines.\n"
               << "mode 's' for filtering file paths that contains any individual elements\n"
               << "Showing containing lines on stderr.\n"
               << "mode <any>  replaces by pairs, showing every line, no numbering.\n\n"
               << "Also you cand add flag -b to only replace if line begins with pair old str\n";
          return 0;
     }
     
     int ind_start = 1;
     
     Replacer replacer(resolveAction(&ind_start, argv));
          
     char mode = argc > ind_start && *argv[ind_start] == FLAG_MODE ? (*(argv[ind_start]+1)) : 0;
     ind_start+=1;


     // Modes that reads elements.
     if(mode == CH_SEARCH_F){
         // Load elements from argv.
         while(ind_start < argc){ // Not ideal to have them be a replacements lol..
              replacer.addrep(argv[ind_start], REMOVE_REPL);
              //printf("added elem %s \n", argv[ind_start]);
              ind_start+=1;// 1 used...
         }     
         return iter_stdin_files(replacer, &filterPaths);
     }


     // Modes that reads pairs/load replacements from argv.
     while(ind_start < argc-1){ // This ensures 2 elems. begin and begin+1
          replacer.addrep(argv[ind_start], argv[ind_start+1]);
          ind_start+=2;//2 used...
     }
     
     if(mode == M_STD){
         // Show content stdin replaced.
         MContent::parse(std::cin, std::cout, replacer);
         return 0;
     }
     
     mode_act action = &replacefrom;
          
     if(mode == CH_LINE_S){
         action = &linesFilter; 
     }
     
     return iter_stdin_files(replacer, action);
}
