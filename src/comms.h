#ifndef COMMS_H
#define COMMS_H

#include "essentials.h"
#include "dataStorage.h"

#include <QMetaType>

namespace  comms{

struct dataSearchArgs{
    dataStorage* storePtr;
    scan_type scanType;
    QDateTime startDate;
    QDateTime endDate;
    QString user="";
    scan_timeSpan span;
    QTime startTime;
    QTime endTime;
    double thresh;
};

struct fileCopyArgs
{
    QString pathIn="";
    QString pathOut="";

    bool makeBackup=false;
    bool makeLog=false;
    bool makeSort=false;
    bool checkDataCons=false;
    bool checkDataLen=false;

    int defDateLen=20;
    int defUserLen=4;
    int defConsLen=10;

    int refreshRate=1000;
};

struct returnArgs{
    returnArgs(){}
    ~returnArgs(){}

    QVector<double> vMax,vAvg,vMin;
    QVector<QString> vDates;
    QString fMax,fAvg,fMin,total;

    QString avgHour,avgDay,avgWeek,avgMonth;
};
}

Q_DECLARE_METATYPE(comms::returnArgs);

#endif // COMMS_H
