#include "winVisual.h"
#include "ui_winVisual.h"

winVisual::winVisual(QWidget *parent, dataStorage *storePtr) :
    QDialog(parent),ui(new Ui::winVisual),storePtr(storePtr){

    ui->setupUi(this);
    ui->calendar->setMaximumDate(HIGHTEST_DATE.date());
    ui->calendar->setMinimumDate(LOWEST_DATE.date());
    
    worker->moveToThread(thread);

    connect(thread,SIGNAL(started()),worker,SLOT(start()));
    connect(worker,SIGNAL(finished()),thread,SLOT(quit()));
    connect(worker,SIGNAL(results_out(comms::returnArgs)),this,SLOT(results_in(comms::returnArgs)));
}

winVisual::~winVisual()
{
    delete ui;
    delete worker;

    if(ui->graphWidget->chart() != NULL)
        delete ui->graphWidget->chart();
}

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

void winVisual::results_in(comms::returnArgs vals){
    
    ui->warnsBox->insertPlainText("\ndata reading completed\n");

    ui->totBox->setText(vals.total);
    ui->maxBox->setText(vals.fMax);
    ui->medBox->setText(vals.fAvg);
    ui->minBox->setText(vals.fMin);

    ui->warnsBox->insertPlainText("\nbuilding graph\n");

    //if(ui->graphWidget->chart() != NULL) delete ui->graphWidget->chart();

    QChart* chart = new QChart();

    QBarSeries* series = new QBarSeries(chart);
    QBarSet* barMax = new QBarSet("Max",series);
    QBarSet* barAvg = new QBarSet("Avg",series);
    QBarSet* barMin = new QBarSet("Min",series);

    chart->addSeries(series);

    QBarCategoryAxis *axisX = new QBarCategoryAxis(chart);
    axisX->setTitleText("Date");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);


    QValueAxis* axisY = new QValueAxis(chart);
    axisY->setTitleText("Readings");
    axisY->setRange(0,vals.fMax.toDouble());
    chart->addAxis(axisY,Qt::AlignLeft);
    series->attachAxis(axisY);

    for(int i=0;i<vals.vDates.size();++i){
        barMax->append(vals.vMax[i]);
        barAvg->append(vals.vAvg[i]);
        barMin->append(vals.vMin[i]);

        axisX->append(vals.vDates[i]);
    }

    series->append(barMax);
    series->append(barAvg);
    series->append(barMin);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    ui->graphWidget->setChart(chart);
    ui->graphWidget->setRenderHint(QPainter::Antialiasing);

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

void winVisual::on_graphWidget_rubberBandChanged(const QRect &viewportRect, const QPointF &fromScenePoint, const QPointF &toScenePoint){
    ui->graphWidget->chart()->zoomIn(viewportRect);
}

