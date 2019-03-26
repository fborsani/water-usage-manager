#ifndef winCopyOptions_H
#define winCopyOptions_H

#include <QDialog>
#include <QString>
#include <QFileDialog>

#include "comms.h"

namespace Ui {
class winCopyOptions;
}

class winCopyOptions : public QDialog
{
    Q_OBJECT

public:
    explicit winCopyOptions(QWidget *parent = 0);
    ~winCopyOptions();

private:
    Ui::winCopyOptions *ui;

signals:
    void argsStruct_out(comms::fileCopyArgs args);

private slots:
    void on_fileBrowser_clicked();
    void on_confirm_clicked();
    void on_reset_clicked();
};

#endif // winCopyOptions_H
