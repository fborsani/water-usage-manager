#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "winCopy.h"
#include "winVisual.h"
#include "winQuery.h"
#include "winAnalyze.h"

#include "comms.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_commandLinkButton_clicked();

    void on_commandLinkButton_2_clicked();

    void on_commandLinkButton_3_clicked();

    void on_commandLinkButton_4_clicked();

private:
    Ui::MainWindow *ui;

    dataStorage container;

};

#endif // MAINWINDOW_H
