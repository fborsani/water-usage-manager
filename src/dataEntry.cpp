#include "dataEntry.h"

dataEntry::dataEntry(QString date, QString cons, QString user):date(date),cons(cons),user(user){}
dataEntry::dataEntry(std::string stdDate, std::string stdCons, std::string stdUser):
    date(date.fromStdString(stdDate)),
    cons(cons.fromStdString(stdUser)),
    user(user.fromStdString(stdCons)){}

dataEntry::dataEntry(){}

QString dataEntry::getDate() const{return date;}
QDateTime dataEntry::getDateTime() const{return QDateTime::fromString(date,DATE_TIME_FORMAT);}
QString dataEntry::getUser() const{return user;}
QString dataEntry::getCons() const{return cons;}
double dataEntry::getConsDouble() const{return cons.toDouble();}
size_t dataEntry::sizeOf() const{return sizeof(date)+sizeof(user)+sizeof(cons);}

bool dataEntry::compare(dataEntry lt, dataEntry rt){return lt.getDateTime() < rt.getDateTime();}

void dataEntry::calcDelta(dataEntry* prevRead){
    if(prevRead==NULL) this->delta = this->getConsDouble();
    else this->delta = this->getConsDouble()-prevRead->getConsDouble();
}
double dataEntry::getDelta(){return delta;}

bool dataEntry::operator ==(dataEntry& c){return this->getDate()==c.getDate();}
bool dataEntry::operator !=(dataEntry& c){return !(*this==c);}
bool dataEntry::operator >(dataEntry& c){return this->getDateTime()>c.getDateTime();}
bool dataEntry::operator <(dataEntry& c){return this->getDateTime()<c.getDateTime();}
bool dataEntry::operator >=(dataEntry& c){return this->getDateTime()>=c.getDateTime();}
bool dataEntry::operator <=(dataEntry& c){return this->getDateTime()<=c.getDateTime();}

bool dataEntry::operator ==(const dataEntry& c) const{return this->getDate()==c.getDate();}
bool dataEntry::operator !=(const dataEntry& c) const{return !(*this==c);}
bool dataEntry::operator >(const dataEntry& c) const{return this->getDateTime()>c.getDateTime();}
bool dataEntry::operator <(const dataEntry& c) const{return this->getDateTime()<c.getDateTime();}
bool dataEntry::operator >=(const dataEntry& c) const{return this->getDateTime()>=c.getDateTime();}
bool dataEntry::operator <=(const dataEntry& c) const{return this->getDateTime()<=c.getDateTime();}
