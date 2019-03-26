#ifndef workerDataSearch_H
#define workerDataSearch_H

#include <QObject>
#include <QPair>

#include "comms.h"

class workerDataSearch : public QObject{

    Q_OBJECT

public:  
    explicit workerDataSearch(QObject* parent=0);

    void leakScan(QTime startTime,
                  QTime endTime,
                  double thresh);

    void hiConsScan(QDateTime startDate,QDateTime endDate,
                    scan_timeSpan span);

    void fullMedConsScan(QString user,QDateTime startDate,QDateTime endDate);

    void graphScan(QDateTime startDate, QDateTime endDate, scan_timeSpan span, QString user);

    void setScanParam(comms::dataSearchArgs args);

signals:
    void message_out(QString out);
    void results_out(comms::returnArgs vals);
    void finished();

public slots:
    void start();
    void quit();
private:
    bool abort=false;

    //output vars
    double tot=0;
    QString msgString;

    comms::dataSearchArgs args;
};

#endif // workerDataSearch_H
