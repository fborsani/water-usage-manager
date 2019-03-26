#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qRegisterMetaType<comms::returnArgs>();
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_commandLinkButton_clicked(){
    winCopy* winCopyPtr=new winCopy(this,&container);
    winCopyPtr->show();
    winCopyPtr->setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::on_commandLinkButton_2_clicked(){
    winVisual* winVisPtr=new winVisual(this,&container);
    winVisPtr->show();
    winVisPtr->setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::on_commandLinkButton_3_clicked(){
    winQuery* winIntPtr=new winQuery(this,&container);
    winIntPtr->show();
    winIntPtr->setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::on_commandLinkButton_4_clicked(){
    winAnalyze* winAnPtr=new winAnalyze(this,&container);
    winAnPtr->show();
    winAnPtr->setAttribute(Qt::WA_DeleteOnClose);
}
