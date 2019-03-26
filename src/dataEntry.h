#ifndef dataEntry_H
#define dataEntry_H

#include "essentials.h"

class dataEntry
{
public:
    dataEntry(QString date,QString cons,QString user);
    dataEntry(std::string stdDate,std::string stdCons,std::string stdUser);
    dataEntry();

    QString getDate() const;
    QDateTime getDateTime() const;
    QString getUser() const;
    QString getCons() const;
    double getConsDouble() const;
    size_t sizeOf() const;

    void calcDelta(dataEntry* prevRead);
    double getDelta();

    bool compare(dataEntry lt,dataEntry rt);

    bool operator ==(dataEntry& c);
    bool operator !=(dataEntry& c);
    bool operator <(dataEntry& c);
    bool operator >(dataEntry& c);
    bool operator <=(dataEntry& c);
    bool operator >=(dataEntry& c);

    bool operator ==(const dataEntry& c)const;
    bool operator !=(const dataEntry& c)const;
    bool operator <(const dataEntry& c)const;
    bool operator >(const dataEntry& c)const;
    bool operator <=(const dataEntry& c)const;
    bool operator >=(const dataEntry& c)const;
private:
    QString date;
    QString cons;
    QString user;

    double delta;
};

#endif // dataEntry_H
