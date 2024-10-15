
#include <fstream>
#include <cstring>
#include <unistd.h>

#include "./basereplacer.h"
#include "./mcontent.h"

#include <errno.h>
#include "./utilerror.h"

// Static utility methods....
bool MContent::copy(const char* in_file, const char* out_file) {

    std::ifstream src(in_file, std::ios::binary);
    
    if(!src){
      fprintf(stderr, "ERROR INTERNAL OPEN IN SRC\n");
      return false;
    }
    
    std::ofstream dst(out_file, std::ios::binary);
    if(!dst){
      fprintf(stderr, "ERROR INTERNAL OPEN OUT DST\n");
      return false;
    }
    
    dst << src.rdbuf();
    
    //Destroyed at end.
    return true;
}


void MContent::parse(std::istream& in, std::ostream& out, const BaseReplacer& repl){
    std::string part;    
    while(repl.extract(in, part)){
           out << part;
    }
}

bool MContent::parse(const char* in_file, const char* out_file, const BaseReplacer& replacer) {
    std::ifstream in(in_file);
    
    if(!in){
      fprintf(stderr, "ERROR INTERNAL OPEN IN FILE %s\n", in_file);
      return false;
    }
    std::ofstream out(out_file);
    
    if(!out){
      fprintf(stderr, "ERROR INTERNAL OPEN TEMP FILE\n");
      return false;
    }
    std::string part;
    
    while(replacer.extract(in, part)){
           out << part;
    }
    
    // destroyer does delete streams.
    return true;
}

//Temp file/ non static methods management

MContent::MContent(const char * name_template) : tmpname(name_template){
       printf("-----AUTO Temp file creating... at %s \n",tmpname.c_str());
       fdtmp = mkstemp(tmpname.data());
       if(fdtmp < 0){
            throw new UtilError(errno, "Failed to CREATE temporal file");
       }
       
       printf("-----fmod temp at %s \n",tmpname.c_str());

}

//Save tempfile content on out file.
bool MContent::save(const char* out_file){
     return copy(tmpname.c_str(), out_file);
}

//Save content on in file at tmp file.
bool MContent::load(const char* in_file){
     return copy(in_file, tmpname.c_str());
}


//Parse in file content using replacer. To temp file.
bool MContent::parse(const char* in_file, const BaseReplacer& repl){
     
     return parse(in_file, tmpname.c_str(), repl);
}

bool MContent::rewrite(const char* in_file, const BaseReplacer& repl){
    if(!parse(in_file, repl)){
           return false;
    }
    //Copy from tmp back to original file
    return save(in_file);
}



MContent::~MContent(){
      close(fdtmp);
      unlink(tmpname.c_str());
      printf("CLOSED TEMP FILE %s \n",tmpname.c_str());
}











