#include "dataStorage.h"

dataStorage::dataStorage(){}

void dataStorage::clear(){readsMap.clear();}
void dataStorage::add(QString user,QVector<dataEntry> reads){readsMap.insert(user,reads);}
QVector<dataEntry> dataStorage::getReads(QString user){return readsMap.value(user);}
QVector<QString> dataStorage::getUsers(){return readsMap.keys().toVector();}
bool dataStorage::isUser(QString user){
  QVector<QString> usrs = this->getUsers();
  for(int i=0;i<usrs.size();++i)
      if(usrs[i] == user)
          return true;
  return false;
}
int dataStorage::size(){
    int val=0;
    for(int i=0;i<readsMap.size();++i)
        val+=readsMap.value(readsMap.keys().takeAt(i)).size();
    return val;
}
size_t dataStorage::memsize(){
    size_t size=0;
    if(!readsMap.empty()){
        size = readsMap.value(readsMap.keys().takeAt(0))[0].sizeOf();
        return size*this->size();
    }
    return 0;
}


