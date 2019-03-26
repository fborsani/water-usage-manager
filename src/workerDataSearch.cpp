#include "workerDataSearch.h"

workerDataSearch::workerDataSearch(QObject *parent) : QObject(parent){}

void workerDataSearch::graphScan(QDateTime startDate, QDateTime endDate, scan_timeSpan span, QString user){
    QDateTime currDate = startDate;
    QDateTime prevDate;
    double dateDelta = 0, totalDateDelta = 0;
    QVector<dataEntry> reads = args.storePtr->getReads(user);


    QVector<QString> vDates;
    QVector<double> vMax,vAvg,vMin;
    double total = 0;

    while(currDate < endDate){
        double max = 0, min = std::numeric_limits<double>::max(), avg = 0,tempTot = 0;
        prevDate = currDate;
        bool minChanged = false;

        if(span==scan_timeSpan::HOUR)
            vDates.push_back(prevDate.time().toString(TIME_FORMAT));
        else
            vDates.push_back(prevDate.date().toString(DATE_FORMAT));

        switch (span) {
        case scan_timeSpan::MONTH:
            currDate = currDate.addMonths(1);
            dateDelta = 60*24*30;
            totalDateDelta = dateDelta*12;
            break;
        case scan_timeSpan::WEEK:
            currDate = currDate.addDays(7);
            dateDelta = 60*24*7;
            totalDateDelta = dateDelta*4;
            break;
        case scan_timeSpan::DAY:
            currDate = currDate.addDays(1);
            dateDelta = 60*24;
            totalDateDelta = dateDelta*30;
            break;
        case scan_timeSpan::HOUR:
            currDate = currDate.addSecs(3600);
            dateDelta = 60;
            totalDateDelta = dateDelta*24;
        }

        for(int i=0;i<reads.size();++i){
            if(reads[i].getDateTime() >= prevDate && reads[i].getDateTime() < currDate){    //non inclusive right border to avoid overlapping
                double val = reads[i].getDelta();

                tempTot += val;
                if(max < val) max = val;
                if(min > val) min = val;

                minChanged = true;
            }
        }
        avg = tempTot/(prevDate.secsTo(currDate)/dateDelta);
        total += tempTot;

        vMax.push_back(max);
        vAvg.push_back(avg);

        if(minChanged) vMin.push_back(min);
        else vMin.push_back(0.0);
    }

    comms::returnArgs vals;
    vals.vMax = vMax;
    vals.vMin = vMin;
    vals.vAvg = vAvg;
    vals.vDates = vDates;
    vals.fMax = QString::number(*std::max_element(vMax.begin(),vMax.end()));
    vals.fMin = QString::number(*std::min_element(vMin.begin(),vMin.end()));
    vals.fAvg = QString::number(total/(startDate.secsTo(endDate)/totalDateDelta));
    vals.total = QString::number(total);

    emit results_out(vals);
}

void workerDataSearch::fullMedConsScan(QString user, QDateTime startDate, QDateTime endDate){

    QVector<dataEntry> reads = args.storePtr->getReads(user);
    double tot=0;
    long dateDelta = startDate.secsTo(endDate);

    for(int i=0;i<reads.size();++i){
        if(reads[i].getDateTime()>=startDate && reads[i].getDateTime()<=endDate)
            tot+=reads[i].getDelta();
    }

    comms::returnArgs vals;
    vals.total = QString::number(tot);
    vals.avgHour = QString::number(tot/(dateDelta/60));
    vals.avgDay = QString::number(tot/(dateDelta/(60*24)));
    vals.avgWeek = QString::number(tot/(dateDelta/(60*24*7)));
    vals.avgMonth = QString::number(tot/(dateDelta/(60*24*30)));

    emit results_out(vals);
}

void workerDataSearch::leakScan(QTime startTime, QTime endTime,double thresh){

    QVector<QString> vectorUsers=args.storePtr->getUsers();

    emit message_out("Scan started...\n");

    if(vectorUsers.size()>0){

        msgString=msgString.number(vectorUsers.size())+" users found\n";
        emit message_out(msgString);

        for(int i=0;i<vectorUsers.size();++i){
            if(abort) break;

            bool empty=true;

            msgString="<b>Checking user "+vectorUsers[i]+"</b>:\n";
            emit message_out(msgString);

            QVector<dataEntry> vectorReads = args.storePtr->getReads(vectorUsers[i]);

            for(int j=0;j<vectorReads.size();++j)
                if(vectorReads[j].getDateTime().time()>=startTime && vectorReads[j].getDateTime().time()<=endTime&&
                        vectorReads[j].getDelta()>=thresh){

                    msgString="Possible leakage: "+vectorReads[j].getDate()+" water usage: "+QString::number(vectorReads[j].getDelta());
                    emit message_out(msgString);
                    empty=false;
                }
            if(empty)
                emit message_out("No leakage found\n");
        }
    }
    else
        emit message_out("<b>ERROR:</b> no user found! Check your data and retry\n");
}

void workerDataSearch::hiConsScan(QDateTime startDate, QDateTime endDate,scan_timeSpan span){

    emit message_out("Scan started...\n");
    QVector<QString> vecUsers=args.storePtr->getUsers();
    QVector<QPair<QString,double>> vecAvgs;
    double fullTot=0;
    long dateDelta=0;
    int totReadsCount=0;

    if(vecUsers.size()>0){
        msgString=msgString.number(vecUsers.size())+" users found\n";
        emit message_out(msgString);

        for(int i=0;i<vecUsers.size();++i,++totReadsCount){
            double singleAvg=0,singleTot=0;
            QVector<dataEntry> reads = args.storePtr->getReads(vecUsers[i]);
            if(abort) break;

            for(int i=0;i<reads.size();++i)
                if(reads[i].getDateTime()>=startDate && reads[i].getDateTime()<=endDate)
                    singleTot += reads[i].getDelta();

            switch (span) {
            case scan_timeSpan::MONTH:
                dateDelta = startDate.secsTo(endDate)/(60*24*30);
                break;
            case scan_timeSpan::WEEK:
                dateDelta = startDate.secsTo(endDate)/(60*24*7);
                break;
            case scan_timeSpan::DAY:
                dateDelta = startDate.secsTo(endDate)/(60*24);
                break;
            default:
                break;
            }

            singleAvg = singleTot/dateDelta;

            vecAvgs.push_back(QPair<QString,double>(vecUsers[i],singleAvg));

            fullTot+=singleTot;

            msgString="Average consumption user: "+vecUsers[i]+": "+QString::number(singleAvg)+"\n";
            emit message_out(msgString);
        }

        switch (span) {
        case scan_timeSpan::MONTH:
            fullTot /= startDate.secsTo(endDate)/(60*24*30);
            break;
        case scan_timeSpan::WEEK:
            fullTot /= startDate.secsTo(endDate)/(60*24*7);
            break;
        case scan_timeSpan::DAY:
            fullTot /= startDate.secsTo(endDate)/(60*24);
            break;
        default:
            break;
        }

        fullTot /= vecUsers.size();

        msgString="Average global consumptions: "+QString::number(fullTot)+"\n";
        emit message_out(msgString);

        emit message_out("<b>Deviating users:</b>");
        bool empty=true;

        for(int j=0;j<vecAvgs.size();++j){
            if(abort) break;

            if(vecAvgs[j].second>2*fullTot){
                msgString="User "+vecAvgs[j].first+" average: "+QString::number(vecAvgs[j].second)+"\n";
                emit message_out(msgString);
                empty=false;
            }
        }

        if(empty)
            emit message_out("No user found\n");
    }
    else
        emit message_out("<b>ERROR:</b> no user found! Check your data and retry\n");
}

void workerDataSearch::setScanParam(comms::dataSearchArgs args){this->args = args;}

void workerDataSearch::start(){

    switch(args.scanType){
    case scan_type::CONS_GRAPH:
        graphScan(args.startDate,args.endDate,args.span,args.user);
        break;
    case scan_type::CONS_MED:
        fullMedConsScan(args.user,args.startDate,args.endDate);
        break;
    case scan_type::HIGHTCONS:
        hiConsScan(args.startDate,args.endDate,args.span);
        break;
    case scan_type::LEAKS:
        leakScan(args.startTime,args.endTime,args.thresh);
        break;
    }

    emit finished();
}

void workerDataSearch::quit(){abort=true;}
