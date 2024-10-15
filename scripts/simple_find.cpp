#include <iostream>
#include <filesystem>
#include <cstring>
#include "parseruri.h"

static char FLAG_REC[] = "-r";
static char FLAG_CONTAIN = '?';
static char FLAG_INIT = '^';
static const char PATHS_FILE[] = "/home/marcelo-origoni/Documents/mine/paths.txt";

class EndFilter{
private:
     char* ending;
     int length;
     bool (*tester)(const EndFilter* filter, const std::filesystem::path& path);
public:
     EndFilter();
     EndFilter(char * end);
     
     bool testInit(const char * str) const;
     bool testContain(const char * str) const;
     bool testEnd(const char * str) const;
     void testPath(const std::filesystem::path& path);
};


static bool verifyInit(const EndFilter* filter, const std::filesystem::path& path){
     return filter->testInit(path.filename().c_str()); 
}
static bool verifyEnd(const EndFilter* filter,const std::filesystem::path& path){
     return filter->testEnd(path.c_str()); 
}
static bool verifyContain(const EndFilter* filter, const std::filesystem::path& path){
     return filter->testContain(path.c_str()); 
}





EndFilter::EndFilter() : ending(NULL), length(0) {}

EndFilter::EndFilter(char * end){
      if(*(end) == FLAG_INIT){
          ending= end+1;//trim first
          tester = verifyInit;        
      } else if(*(end) == FLAG_CONTAIN){
          ending= end+1;//trim first
          tester = verifyContain;        
      } else {
          ending= end;//trim first
          tester = verifyEnd;        
      }
      
      length = strlen(ending);//Length of looking

}

bool EndFilter::testInit(const char * str) const{
     return strncmp(str, this->ending, this->length) == 0;
}

bool EndFilter::testEnd(const char * str) const{
     return strncmp(str+(strlen(str)-this->length), this->ending, this->length) == 0;
}

bool EndFilter::testContain(const char * str) const{
     return std::string(str).find(this->ending, 0, this->length) != std::string::npos; //Any match after 0 matching length. 
}


void EndFilter::testPath(const std::filesystem::path& path){
      if(this->tester(this, path)){
           printf("%s ",path.c_str());
      }
}

int main(int argc, char* argv[]){
     if(argc == 1){
          std::cout << "use case <filter> <dirbase> [-r for recursive!]\n"
          return;
     }

     EndFilter filter;
     bool isStart;
     filter = EndFilter(argv[1]);
     
     

     std::filesystem::path root{argc <=2? "./" : begin_rep(PATHS_FILE, argv[2])};
     
     if(!std::filesystem::is_directory(root)){
          filter.testPath(root);
          return 0;
     }
     
     if(argc> 3 && strncmp(argv[3], FLAG_REC, strlen(FLAG_REC)) == 0){
       for (auto const& entry : std::filesystem::recursive_directory_iterator(root)){
            if(entry.is_regular_file()){
                  filter.testPath(entry.path());
            }
       }
     } else {
          for (auto const& entry : std::filesystem::directory_iterator(root)){
                filter.testPath(entry.path());
          }
     }

     
}
