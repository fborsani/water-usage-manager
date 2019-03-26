#include "winAnalyze.h"
#include "ui_winAnalyze.h"

winAnalyze::winAnalyze(QWidget *parent, dataStorage *storePtr) :
    QDialog(parent),
    ui(new Ui::winAnalyze),
    storePtr(storePtr)
{
    ui->setupUi(this);

    ui->dateStart->setDisplayFormat(DATE_TIME_FORMAT);
    ui->dateStart->setMaximumDateTime(HIGHTEST_DATE);
    ui->dateStart->setMinimumDateTime(LOWEST_DATE);

    ui->dateEnd->setDisplayFormat(DATE_TIME_FORMAT);
    ui->dateEnd->setMaximumDateTime(HIGHTEST_DATE);
    ui->dateEnd->setMinimumDateTime(LOWEST_DATE);

    workerHiCons->moveToThread(threadHiCons);

    connect(threadHiCons,SIGNAL(started()),workerHiCons,SLOT(start()));
    connect(workerHiCons,SIGNAL(finished()),threadHiCons,SLOT(quit()));
    connect(workerHiCons,SIGNAL(finished()),this,SLOT(threadHiConsFinished()));
    connect(workerHiCons,SIGNAL(message_out(const QString&)),this,SLOT(messageHiCons_in(const QString&)));

    workerLeaks->moveToThread(threadLeaks);

    connect(threadLeaks,SIGNAL(started()),workerLeaks,SLOT(start()));
    connect(workerLeaks,SIGNAL(finished()),threadLeaks,SLOT(quit()));
    connect(workerLeaks,SIGNAL(finished()),this,SLOT(threadLeaksFinished()));
    connect(workerLeaks,SIGNAL(message_out(const QString&)),this,SLOT(messageLeaks_in(const QString&)));
}

winAnalyze::~winAnalyze()
{
    delete ui;
    delete workerLeaks;
    delete workerHiCons;
}



void winAnalyze::on_hightConsScanStart_clicked(){

    ui->hightConsText->clear();

    if(ui->dateStart->dateTime()<ui->dateEnd->dateTime()){

        ui->hightConsScanStart->setEnabled(false);

        scan_timeSpan span;

        if(ui->doDailyScan->isChecked()) span=scan_timeSpan::DAY;
        else if(ui->doWeeklyScan->isChecked()) span=scan_timeSpan::WEEK;
        else span=scan_timeSpan::MONTH;

        comms::dataSearchArgs args;
        args.scanType=scan_type::HIGHTCONS;
        args.storePtr=storePtr;
        args.startDate=ui->dateStart->dateTime();
        args.endDate=ui->dateEnd->dateTime();
        args.span=span;

        workerHiCons->setScanParam(args);

        threadHiCons->start();

    }
    else
        ui->hightConsText->insertHtml("<b>ERRORE:</b> ricontrolla le date riprova<br>");
}

void winAnalyze::messageHiCons_in(const QString &msg){

    QTextCursor textCursor=ui->hightConsText->textCursor();
    textCursor.movePosition(QTextCursor::End);

    ui->hightConsText->insertHtml(msg+"<br>");
    ui->hightConsText->setTextCursor(textCursor);
}

void winAnalyze::threadHiConsFinished(){ui->hightConsScanStart->setEnabled(true);}

void winAnalyze::on_leaksScanStart_clicked(){

    ui->leaksText->clear();

    if(ui->timeStart->time()<ui->timeEnd->time()){
        ui->leaksScanStart->setEnabled(false);

        comms::dataSearchArgs args;
        args.scanType=scan_type::LEAKS;
        args.storePtr=storePtr;
        args.span=scan_timeSpan::MONTH;
        args.startTime=ui->timeStart->time();
        args.endTime=ui->timeEnd->time();
        args.thresh=ui->threshold->value();

        workerLeaks->setScanParam(args);

        threadLeaks->start();
    }
    else
        ui->leaksText->insertHtml("<b>ERRORE:</b> Check time values<br>");
}


void winAnalyze::messageLeaks_in(const QString &msg){
    QTextCursor textCursor=ui->leaksText->textCursor();
    textCursor.movePosition(QTextCursor::End);

    ui->leaksText->insertHtml(msg+"<br>");
    ui->leaksText->setTextCursor(textCursor);
}


void winAnalyze::threadLeaksFinished(){ui->leaksScanStart->setEnabled(true);}

void winAnalyze::reject(){
    workerHiCons->quit();
    threadHiCons->quit();
    if(!threadHiCons->wait(THREADTIMEOUT)){
        threadHiCons->terminate();
        threadHiCons->wait();
    }

    workerLeaks->quit();
    threadLeaks->quit();

    if(!threadLeaks->wait(THREADTIMEOUT)){
        threadLeaks->terminate();
        threadLeaks->wait();
    }

    delete this;
}
