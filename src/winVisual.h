#ifndef winVisual_H
#define winVisual_H

#include <QDialog>
#include <QThread>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QtCharts>

//#include "qcustomplot.h"
#include "workerDataSearch.h"


namespace Ui {
class winVisual;
}

class winVisual : public QDialog
{
    Q_OBJECT

public:
    explicit winVisual(QWidget *parent = 0,dataStorage* storePtr=NULL);
    ~winVisual();

private slots:

    void on_confirm_clicked();
    void on_viewYearly_clicked();
    void on_viewMonthly_clicked();
    void on_viewDaily_clicked();
    void on_viewMonthly_30days_clicked();
    void on_viewMonthly_4weeks_clicked();

    void reject();

    void on_graphWidget_rubberBandChanged(const QRect &viewportRect, const QPointF &fromScenePoint, const QPointF &toScenePoint);

public slots:

    void results_in(comms::returnArgs vals);

private:
    Ui::winVisual *ui;

    scan_timeSpan span;

    QThread* thread=new QThread(this);
    workerDataSearch* worker=new workerDataSearch;

    dataStorage* storePtr=NULL;
};

#endif // winVisual_H
