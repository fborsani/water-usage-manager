#include "winVisual.h"
#include "ui_winVisual.h"

winVisual::winVisual(QWidget *parent, dataStorage *storePtr) :
    QDialog(parent),ui(new Ui::winVisual),storePtr(storePtr){

    ui->setupUi(this);
    ui->calendar->setMaximumDate(HIGHTEST_DATE.date());
    ui->calendar->setMinimumDate(LOWEST_DATE.date());

    maxBars=new QCPBars(ui->graphWidget->xAxis,ui->graphWidget->yAxis);
    maxGraph=new QCPGraph(ui->graphWidget->xAxis,ui->graphWidget->yAxis);
    medGraph=new QCPGraph(ui->graphWidget->xAxis,ui->graphWidget->yAxis);
    minGraph=new QCPGraph(ui->graphWidget->xAxis,ui->graphWidget->yAxis);

    worker->moveToThread(thread);

    connect(thread,SIGNAL(started()),worker,SLOT(start()));
    connect(worker,SIGNAL(finished()),thread,SLOT(quit()));
    connect(worker,SIGNAL(results_out(comms::returnArgs)),this,SLOT(results_in(comms::returnArgs)));
}

winVisual::~winVisual()
{
    delete ui;
    delete worker;

    delete maxBars;
    delete maxGraph;
    delete medGraph;
    delete minGraph;

}


//funzione per calcolo dei parametri per il grafico


void winVisual::on_confirm_clicked(){
    ui->warnsBox->clear();
    ui->confirm->setEnabled(false);

    QDateTime startDate;
    QDateTime endDate;

    switch(span){
    case scan_timeSpan::MONTH:

        startDate=LOWEST_DATE;
        endDate=HIGHTEST_DATE;
        break;
    case scan_timeSpan::WEEK:
    case scan_timeSpan::DAY:

        //startDate.setDate(ui->calendar->selectedDate());
        startDate.setDate(QDate(ui->calendar->selectedDate().year(),ui->calendar->selectedDate().month(),1));
        endDate=startDate.addMonths(1);
        break;
    case scan_timeSpan::HOUR:

        startDate.setDate(ui->calendar->selectedDate());
        endDate=startDate.addDays(1);
        break;
    }
    comms::dataSearchArgs args;
    args.scanType=scan_type::CONS_GRAPH;
    args.storePtr=storePtr;
    args.user=ui->userCode->text();
    args.startDate=startDate;
    args.endDate=endDate;
    args.span=span;

    if(startDate.isValid()&&endDate.isValid()&&ui->userCode->text()!=""){
        worker->setScanParam(args);
        thread->start();
    }
    else
        ui->warnsBox->insertPlainText("ATTENZIONE: dati inseriti invalidi, ricontrolla e riprova\n");
}

void winVisual::on_viewYearly_clicked(){
    ui->confirm->setEnabled(true);

    ui->viewMonthly_4weeks->hide(); ui->viewMonthly_4weeks->setChecked(false);
    ui->viewMonthly_30days->hide(); ui->viewMonthly_30days->setChecked(false);

    span=scan_timeSpan::MONTH;
}

void winVisual::on_viewMonthly_clicked(){
    ui->viewMonthly_4weeks->show();
    ui->viewMonthly_30days->show();
}

void winVisual::on_viewDaily_clicked(){
    ui->confirm->setEnabled(true);

    ui->viewMonthly_4weeks->hide(); ui->viewMonthly_4weeks->setChecked(false);
    ui->viewMonthly_30days->hide(); ui->viewMonthly_30days->setChecked(false);

    span=scan_timeSpan::HOUR;
}

void winVisual::on_viewMonthly_30days_clicked(){
    span=scan_timeSpan::DAY;
    ui->confirm->setEnabled(true);
}
void winVisual::on_viewMonthly_4weeks_clicked(){
    span=scan_timeSpan::WEEK;
    ui->confirm->setEnabled(true);
}

//slot per ricevere messaggi dal thread

void winVisual::results_in(comms::returnArgs vals){
    ui->warnsBox->insertPlainText("\ndata reading completed\n");

    ui->totBox->setText(vals.total);
    ui->maxBox->setText(vals.fMax);
    ui->medBox->setText(vals.fAvg);
    ui->minBox->setText(vals.fMin);

    QVector<double> tickVector;
    for(double i=0;i<=vals.vDates.size();++i)
        tickVector.push_back(i);

    ui->graphWidget->xAxis->setAutoTicks(false);
    ui->graphWidget->xAxis->setAutoTickLabels(false);
    ui->graphWidget->xAxis->setTickVector(tickVector);
    ui->graphWidget->xAxis->setTickVectorLabels(vals.vDates);
    ui->graphWidget->xAxis->setTickLabelRotation(60);
    ui->graphWidget->yAxis->setRangeLower(0);

    ui->graphWidget->xAxis->setLabel("Date");
    ui->graphWidget->yAxis->setLabel("Water usage");
    ui->graphWidget->legend->setVisible(true);
    ui->graphWidget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    maxBars->setName("Usage");
    maxBars->addToLegend();
    maxBars->setData(tickVector,vals.vMax);

    maxGraph->setName("Max usage");
    maxGraph->addToLegend();
    maxGraph->setData(tickVector,vals.vMax);
    maxGraph->setPen(QPen(Qt::red));


    medGraph->setName("Avg usage");
    medGraph->addToLegend();
    medGraph->setData(tickVector,vals.vAvg);
    medGraph->setPen(QPen(Qt::green));


    minGraph->setName("Min usage");
    minGraph->addToLegend();
    minGraph->setData(tickVector,vals.vMin);
    minGraph->setPen(QPen(Qt::blue));

    ui->graphWidget->rescaleAxes();
    ui->graphWidget->replot();

    ui->warnsBox->insertPlainText("\ngraph creation completed!\n");
}

void winVisual::reject(){
    worker->quit();
    thread->quit();

    if(!thread->wait(THREADTIMEOUT)){
        thread->terminate();
        thread->wait();
    }
    delete this;
}
