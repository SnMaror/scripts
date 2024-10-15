#ifndef LIB_MCONTENT_H
#define LIB_MCONTENT_H

#include <utility>
#include <fstream>
#include <string>
#include "./basereplacer.h"


class MContent {
private:
      std::string tmpname;
      int fdtmp;
public:
     //I/O replacing actions. Static for now. To allow for usage outside temp file
     static bool copy(const char* in_file, const char* out_file);
     
     static void parse(std::istream& in, std::ostream& out, const BaseReplacer& repl);
     static bool parse(const char* in_file, const char* out_file, const BaseReplacer& repl);
     
     static bool rewrite(const char* file, const char * tmp, const BaseReplacer& repl);


     explicit MContent(const char* name_template);
     
     //Save tempfile content on out file.
     bool save(const char* out_file);
     
     //Save content on in file at tmp file.
     bool load(const char* in_file);
     
     //Parse in file content using replacer. To temp file.
     bool parse(const char* in_file, const BaseReplacer& repl);
     
     bool rewrite(const char* in_file, const BaseReplacer& repl);
     
     
     ~MContent();
};

#endif
