#ifndef winCopy_H
#define winCopy_H

#include <QDialog>
#include <QThread>
#include <QFileDialog>

#include "winCopyOptions.h"
#include "workerFileCopy.h"

namespace Ui {
class winCopy;
}

class winCopy : public QDialog
{
    Q_OBJECT

public:
    explicit winCopy(QWidget *parent = 0,dataStorage* container = NULL);
    ~winCopy();

private slots:

    void on_options_clicked();

    void on_openFileBrowser_clicked();

    void on_confirmButton_clicked();

    void reject();

public slots:
    void argsStruct_in(comms::fileCopyArgs args);

    void progBar_max_in(int val);
    void progBar_min_in(int val);
    void progBar_val_in(int val);

    void state_text_in(QString val);
    void state_detail_in(QString val);

    void threadFinished();

private:

    dataStorage* containerPtr;

    Ui::winCopy *ui;
    winCopyOptions* winCopyOptsPtr=new winCopyOptions(this);

    QThread* thread=new QThread(this);
    workerFileCopy* worker=new workerFileCopy;

    QString path_in;
    comms::fileCopyArgs args;

    QString state_text;
    QString state_detail;
};

#endif // winCopy_H
