#include "winQuery.h"
#include "ui_winQuery.h"

winQuery::winQuery(QWidget *parent, dataStorage *storePtr) :
    QDialog(parent),
    ui(new Ui::winQuery),
    storePtr(storePtr)
{
    ui->setupUi(this);

    ui->dateStart->setDisplayFormat(DATE_TIME_FORMAT);
    ui->dateStart->setMaximumDateTime(HIGHTEST_DATE);
    ui->dateStart->setMinimumDateTime(LOWEST_DATE);

    ui->dateEnd->setDisplayFormat(DATE_TIME_FORMAT);
    ui->dateEnd->setMaximumDateTime(HIGHTEST_DATE);
    ui->dateEnd->setMinimumDateTime(LOWEST_DATE);

    worker->moveToThread(thread);

    connect(thread,SIGNAL(started()),worker,SLOT(start()));
    connect(worker,SIGNAL(finished()),thread,SLOT(quit()));
    connect(worker,SIGNAL(finished()),this,SLOT(threadFinished()));
    connect(worker,SIGNAL(results_out(comms::returnArgs)),this,SLOT(results_in(comms::returnArgs)));
}

winQuery::~winQuery()
{
    delete ui;
    delete worker;
}

void winQuery::on_confirmButton_clicked(){

    ui->boxTot->clear();
    ui->boxHourMed->clear();
    ui->boxDayMed->clear();
    ui->boxWeekMed->clear();
    ui->boxMonthMed->clear();

    QDateTime startDate=ui->dateStart->dateTime();
    QDateTime endDate=ui->dateEnd->dateTime();

    if(!ui->userCode->text().isEmpty()&&startDate<endDate){

        comms::dataSearchArgs args;
        args.scanType=scan_type::CONS_MED;
        args.storePtr=storePtr;
        args.startDate=startDate;
        args.endDate=endDate;
        args.user=ui->userCode->text();

        worker->setScanParam(args);
        thread->start();
        ui->confirmButton->setEnabled(false);
    }
    else{
        ui->boxTot->setText("Incorrect parameters");
        ui->boxHourMed->setText("Incorrect parameters");
        ui->boxDayMed->setText("Incorrect parameters");
        ui->boxWeekMed->setText("Incorrect parameters");
        ui->boxMonthMed->setText("Incorrect parameters");
    }
}

void winQuery::results_in(comms::returnArgs vals){
    ui->boxTot->setText(vals.total);
    ui->boxHourMed->setText(vals.avgHour);
    ui->boxDayMed->setText(vals.avgDay);
    ui->boxWeekMed->setText(vals.avgWeek);
    ui->boxMonthMed->setText(vals.avgMonth);
}


void winQuery::threadFinished(){
    ui->confirmButton->setEnabled(true);
}

void winQuery::reject(){
    worker->quit();
    thread->quit();

    if(!thread->wait(THREADTIMEOUT)){
        thread->terminate();
        thread->wait();
    }

    delete this;
}
