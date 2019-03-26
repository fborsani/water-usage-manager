#ifndef winAnalyze_H
#define winAnalyze_H

#include <QDialog>
#include <QThread>

#include "workerDataSearch.h"

namespace Ui {
class winAnalyze;
}

class winAnalyze : public QDialog
{
    Q_OBJECT

public:
    explicit winAnalyze(QWidget *parent = 0,dataStorage* storePtr=NULL);
    ~winAnalyze();

public slots:
    void messageLeaks_in(const QString& msg);
    void messageHiCons_in(const QString& msg);

    void threadLeaksFinished();
    void threadHiConsFinished();

private slots:
    void on_leaksScanStart_clicked();

    void on_hightConsScanStart_clicked();

    void reject();

private:
    Ui::winAnalyze *ui;

    dataStorage* storePtr=NULL;

    workerDataSearch* workerLeaks=new workerDataSearch;
    workerDataSearch* workerHiCons=new workerDataSearch;

    QThread* threadLeaks=new QThread(this);
    QThread* threadHiCons= new QThread(this);
};

#endif // winAnalyze_H
