#ifndef winQuery_H
#define winQuery_H

#include <QDialog>
#include <QThread>

#include "workerDataSearch.h"

namespace Ui {
class winQuery;
}

class winQuery : public QDialog
{
    Q_OBJECT

public:
    explicit winQuery(QWidget *parent = 0, dataStorage *storePtr=0);
    ~winQuery();

private slots:
    void on_confirmButton_clicked();
    void results_in(comms::returnArgs in);
    void threadFinished();

    void reject();
private:
    Ui::winQuery *ui;

    dataStorage* storePtr=NULL;
    unsigned int outputScreen=0;

    QThread* thread=new QThread(this);
    workerDataSearch* worker=new workerDataSearch;
};

#endif // winQuery_H
