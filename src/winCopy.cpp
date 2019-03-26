#include "winCopy.h"
#include "ui_winCopy.h"

winCopy::winCopy(QWidget *parent,dataStorage* container):QDialog(parent),
    ui(new Ui::winCopy){

    ui->setupUi(this);

    containerPtr=container;

    connect(winCopyOptsPtr,SIGNAL(argsStruct_out(comms::fileCopyArgs)),this,SLOT(argsStruct_in(comms::fileCopyArgs)));

    worker->moveToThread(thread);

    connect(thread,SIGNAL(started()),worker,SLOT(start()));
    connect(worker,SIGNAL(finished()),this,SLOT(threadFinished()));
    connect(worker,SIGNAL(finished()),thread,SLOT(quit()));

    connect(worker,SIGNAL(progBar_max_out(int)),this,SLOT(progBar_max_in(int)));
    connect(worker,SIGNAL(progBar_min_out(int)),this,SLOT(progBar_min_in(int)));
    connect(worker,SIGNAL(progBar_val_out(int)),this,SLOT(progBar_val_in(int)));


    connect(worker,SIGNAL(state_detail_out(QString)),this,SLOT(state_detail_in(QString)));
    connect(worker,SIGNAL(state_text_out(QString)),this,SLOT(state_text_in(QString)));
}

winCopy::~winCopy(){
    delete ui;
    delete winCopyOptsPtr;
    delete worker;
}

void winCopy::on_openFileBrowser_clicked(){
    ui->pathInsert->setText(QFileDialog::getOpenFileName(this,tr("Select file"),"C://",tr("text (*.txt)"
                                                                                          ";;csv (*.csv)")));
}

void winCopy::on_options_clicked(){winCopyOptsPtr->exec();}
void winCopy::argsStruct_in(comms::fileCopyArgs args_in){args=args_in;}


void winCopy::progBar_max_in(int val){ui->progressBar->setMaximum(val);}
void winCopy::progBar_min_in(int val){ui->progressBar->setMinimum(val);}
void winCopy::progBar_val_in(int val){ui->progressBar->setValue(val);}

void winCopy::state_text_in(QString val){ui->state_label->setText(val);}
void winCopy::state_detail_in(QString val){
    QTextCursor textCursor=ui->state_detail->textCursor();
    textCursor.movePosition(QTextCursor::End);

    ui->state_detail->insertPlainText(val);
    ui->state_detail->setTextCursor(textCursor);
}

void winCopy::on_confirmButton_clicked(){
    ui->state_detail->clear();

    if(ui->pathInsert->text()!=""){
        args.pathIn=ui->pathInsert->text();

        ui->confirmButton->setEnabled(false);
        ui->options->setEnabled(false);
        ui->openFileBrowser->setEnabled(false);

        worker->getSettings(containerPtr,args);
        thread->start();
    }
    else
        ui->state_detail->insertPlainText("ERROR: path is empty\n");
}

//gestione thread

void winCopy::threadFinished(){
    ui->progressBar->setValue(ui->progressBar->maximum());
    ui->confirmButton->setEnabled(true);
    ui->options->setEnabled(true);
    ui->openFileBrowser->setEnabled(true);
}

void winCopy::reject(){
    worker->abort();
    thread->quit();

    if(!thread->wait(THREADTIMEOUT)){
        thread->terminate();
        thread->wait();
    }

   delete this;
}
