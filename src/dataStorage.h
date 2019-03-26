#ifndef DATASTORAGE_H
#define DATASTORAGE_H

#include "dataEntry.h"
#include "essentials.h"
#include <QMap>

class dataStorage
{
public:
    dataStorage();

    void clear();
    void add(QString user,QVector<dataEntry> reads);
    void addUser(QString user);
    QVector<dataEntry> getReads(QString user);
    QVector<QString> getUsers();
    bool isUser(QString user);

    int size();
    size_t memsize();

private:
    QMap<QString,QVector<dataEntry>> readsMap;
};

#endif // DATA_STORAGE_H
