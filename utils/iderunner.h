#ifndef LIB_IDERUNNER_H
#define LIB_IDERUNNER_H
#include <map>
#include <string>
#include <cstdint>

#include <list>
#include <vector>

typedef std::string command_type;
typedef uint8_t pck_type;
typedef std::map<std::string, std::list<pck_type>> type_map_names;

typedef bool (*packet_chooser) (std::string& packet);

//template<class K>
class IdeRunner {
private:
    std::vector<std::string> packets; //or folders for cpp, packages for java. Etc...
    std::string extension; //Files extension...
    
    type_map_names mapnames;
    
protected:
    //notifyClassRemovedFrom(std::string&& classname, std::string packet);
    //notifyClassMovedFrom(std::string&& classname, std::string packet);

    // A la hora de hacer el map va a ser mas facil tener el tipo.
    typedef int (IdeRunner::*ide_action)(std::string& args);


    // Se define como constante ya que no depende de la instancia el map... por ahora almenos.
    static const std::map<command_type, ide_action> actions;  // cppcheck-suppress unusedStructMember
    
    int exec_ls(std::string& args);

public:
      IdeRunner();
            
      int exec(command_type command, std::string& args); // simple exec that resolves.
      
      
      
      //Add/Remove reference from the map, vector for the name.
      
      // 1. If the class doesnt exists it adds it.
      void addClass(std::string name, pck_type package);
      void addClass(std::string name, std::string packagename);
      
      // 2. If class package is the last one. It removes it.
      void rmClass(std::string name, pck_type package);
      
      bool rmClass(std::string name, packet_chooser chooser);
      
      std::list<pck_type>* getPackets(std::string classname);
      
      
      typedef bool (*item_receiver) (std::string&& itm);
      void iterateItems(item_receiver visit);
      
      
};

#endif
