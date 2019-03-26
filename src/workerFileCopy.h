#ifndef workerFileCopy_H
#define workerFileCopy_H

#include <QObject>

#include <QStringList>
#include <QFile>
#include <QTextStream>


#include "comms.h"

class workerFileCopy : public QObject
{
    Q_OBJECT
public:
    explicit workerFileCopy(QObject *parent = 0);

    void getSettings(dataStorage* getStorePtr,comms::fileCopyArgs getArgs);

signals:
    void state_text_out(QString val);
    void state_detail_out(QString val);

    void progBar_max_out(int val);
    void progBar_min_out(int val);
    void progBar_val_out(int val);

    void finished();

public slots:
    void start();
    void abort();

private:
    bool running=false;
    bool doAbort=false;

    dataStorage* storePtr;
    comms::fileCopyArgs args;
};

#endif // workerFileCopy_H
