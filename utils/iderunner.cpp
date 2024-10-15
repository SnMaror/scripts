#include<iostream>

#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#include "./iderunner.h"

#include <algorithm>

int IdeRunner::exec_ls(std::string& args){
      std::cout << "LOLLL LS \n" << args << "\n";
      return 0;
}


const std::map<command_type, IdeRunner::ide_action> IdeRunner::actions = {
        {"ls", &IdeRunner::exec_ls}};



IdeRunner::IdeRunner(){

}


// 1. If the class doesnt exists it adds it.


void IdeRunner::addClass(std::string name, pck_type package){
      //std::pair<I,bool> const&
      auto res =mapnames.insert(type_map_names::value_type(name, std::list{package}));
      if (!res.second) { // value wasn't inserted because name already existed.
            (res.first->second).insert(res.first->second.begin(), package);
      }
}
void IdeRunner::addClass(std::string name, std::string packagename){
      auto pck_it = find(packets.begin(),packets.end(), packagename);
      int ind;
      if(pck_it == packets.end()){
           ind = packets.size();
           packets.push_back(packagename);
      } else{
           ind = pck_it- packets.begin();
      }
      
      addClass(name, ind);
}

// 2. If class package is the last one. It removes it.
void IdeRunner::rmClass(std::string name, pck_type package){
      auto search = mapnames.find(name);
      if (search == mapnames.end()) {
            return;
      }          
      
      for (auto it = search->second.begin(); it != search->second.end();)
      {
           if (*it == package){
              it = search->second.erase(it);
              
              if(search->second.size() == 0){
                    mapnames.erase(search);
              }
              
              return;
           }
           ++it;
      }
}


// 2. Attmp to remove class from map. Returns if its deleted.
bool IdeRunner::rmClass(std::string name, packet_chooser chooser){
      auto search = mapnames.find(name);
      if (search == mapnames.end()) {
            return true; //already deleted.
      }          
      
      if(search->second.size() > 1){
            for (auto it = search->second.begin(); it != search->second.end();)
            {
                 if (chooser(packets[*it])){
                    it = search->second.erase(it);
                    //notifyClassRemovedFrom(name, packets[*it]);
                    
                    if(search->second.size() == 0){
                          mapnames.erase(search);
                    }
                    
                    return true;
                 }
                 ++it;
            }
           //cannot delete if there is more than one... CHOOSE!!
           return false;
      }
      //notifyClassRemovedFrom(name, packets[*(search->second.begin())]);
      
      mapnames.erase(search);
      
      return true;
}



std::list<pck_type>* IdeRunner::getPackets(std::string classname){
    auto search = mapnames.find(classname);

    if (search == mapnames.end()) {
          return NULL;
    }
    
    return &(search->second);


}

void IdeRunner::iterateItems(item_receiver visit){


      for (auto classit = mapnames.begin(); classit != mapnames.end();)
      {

          for (auto idparent = classit->second.begin(); idparent != classit->second.end();)
          {
               if (!visit(packets[*idparent] + "." +classit->first)){                  
                  return;
               }
               ++idparent;
          }      
           ++classit;
      }
}


















int IdeRunner::exec(command_type action, std::string& args){ // simple exec that resolves.
    auto search = actions.find(action);

    if (search == actions.end()) {
          return -1;
    }
    return (this->*search->second)(args);
}
