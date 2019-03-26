#include "workerFileCopy.h"

workerFileCopy::workerFileCopy(QObject *parent) : QObject(parent){}

void workerFileCopy::getSettings(dataStorage* getStorePtr,comms::fileCopyArgs getArgs){

    storePtr=getStorePtr;
    args=getArgs;
}

void workerFileCopy::start(){

    emit(progBar_max_out(0));

    QTime timer;
    QString textString;
    QFile fileReader(args.pathIn);
    QTextStream copyStream(&fileReader);
    QFile fileWriter(args.pathOut);
    QTextStream writeStream(&fileWriter);

    QVector<dataEntry> tempDataVector;
    QVector<QString> tempUserVector;
    int refCounter=0;
    int currLine=0;

    if(!args.pathIn.isEmpty() && !(args.makeBackup && args.pathOut.isEmpty())){
        emit(state_text_out("Retrieving data..."));
        emit(state_detail_out("Checking path...\n"));

        fileReader.open(QIODevice::ReadOnly);
        if(fileReader.isOpen()){
            timer.start();

            emit(state_detail_out("file found! Started to retrieve data\n"));

            while(!copyStream.atEnd()&&!doAbort){
                copyStream.readLine();

                ++currLine;
                ++refCounter;

                if(refCounter%args.refreshRate==0){
                    textString="found: "+textString.number(currLine+1)+" lines\n";
                    emit(state_detail_out(textString));
                }
            }

            if(args.makeBackup)
                emit(progBar_max_out(currLine*2+storePtr->size()));
            else
                emit(progBar_max_out(currLine));

            copyStream.seek(0);

            emit(state_text_out("Copia in corso..."));

            currLine=0;
            refCounter=0;

            while(!copyStream.atEnd()&&!doAbort){
                QString tempLine=copyStream.readLine();

                tempLine.remove(QChar('"'));
                QStringList tempLineParts=tempLine.split(',',QString::SkipEmptyParts);

                if(tempLineParts.size()==3){
                    tempDataVector.push_back(dataEntry(tempLineParts[0],tempLineParts[1],tempLineParts[2]));
                    if(!tempUserVector.contains(tempLineParts[2]))
                        tempUserVector.push_back(tempLineParts[2]);
                    ++refCounter;
                }
                else{
                    textString="WARNING: syntax error in line: "+textString.number(currLine)+"\n";
                    emit(state_detail_out(textString));
                }

                ++currLine;

                if(refCounter%args.refreshRate==0){
                    textString="elements copied: "+textString.number(currLine+1)+"\n";
                    emit(state_detail_out(textString));
                    emit(progBar_val_out(currLine));
                }
            }

            fileReader.close();

            emit(state_detail_out("copy completed!\n\n"));

            //check copy errors

            if(args.checkDataLen && !doAbort){
                emit(state_detail_out("checking data integrity...\n"));
                emit(state_text_out("Checking data integrity..."));

                for(int i=0;i<tempDataVector.size();++i)
                    if(tempDataVector[i].getDate().size()>args.defDateLen||
                            tempDataVector[i].getCons().size()>args.defConsLen||
                            tempDataVector[i].getUser().size()>args.defUserLen){

                        tempDataVector.erase(&tempDataVector[i]);

                        textString="found error in entry "+textString.number(i)+" string removed\n";
                        emit(state_detail_out(textString));
                    }
                emit(state_detail_out("integrity check completed!\n\n"));
            }

            //check readings

            if(!doAbort){
                emit(state_detail_out("optimizing data..."));

                if(args.makeSort){
                    emit(state_detail_out("sorting..."));
                    std::sort(tempDataVector.begin(),tempDataVector.end());
                    emit(state_detail_out(" done!"));
                }

                for(int i=0;i<tempUserVector.size() && !doAbort ;++i){
                    QVector<dataEntry> tmpVector;
                    dataEntry* prevVal = NULL;

                    tempDataVector[0].calcDelta(prevVal);
                    tmpVector.push_back(tempDataVector[0]);
                    prevVal = &tempDataVector[0];

                    for(int j=1;j<tempDataVector.size() && !doAbort ;++j){
                        if(tempDataVector[j].getUser() == tempUserVector[i]){
                            if(tempDataVector[j].getConsDouble() >= prevVal->getConsDouble()){
                                tempDataVector[j].calcDelta(prevVal);
                                tmpVector.push_back(tempDataVector[j]);
                                prevVal = &tempDataVector[j];
                            }
                            else{
                                emit(state_detail_out("inconsistent reading found for user "+tempUserVector[i]+" on date: "+tempDataVector[j].getDate()+"\n"));
                            }
                            ++refCounter;
                        }
                    }
                    storePtr->add(tempUserVector[i],tmpVector);
                }

                emit(state_detail_out(" done!"));
            }

            if(args.makeBackup&&!doAbort){
                emit(state_detail_out("creating backup file..."));
                emit(state_text_out("Creating backup file..."));

                fileWriter.open(QIODevice::ReadWrite|QIODevice::Text);
                if(fileWriter.isWritable()){

                    refCounter=0;

                    emit(state_detail_out("created new file at path "+args.pathIn+"\n"));

                    for(int i=0;i<tempDataVector.size();++i){

                        if(doAbort)
                            break;
                        writeStream << tempDataVector[i].getDate() << ";"
                                    << tempDataVector[i].getCons() << ";"
                                    << tempDataVector[i].getUser() << "\n";
                        ++refCounter;

                        if(refCounter%args.refreshRate==0){
                            textString=textString.number(i+1)+" elements written\n";
                            emit(state_detail_out(textString));
                            emit(progBar_val_out(currLine+(i+1)));
                        }
                    }
                    fileWriter.close();
                    emit(state_detail_out(" done!\n\n"));
                }
                else{
                    emit(state_detail_out("ERRORE: indirizzo di output invalido, ricontrolla e riprova\n"));
                }
            }
            size_t mem=storePtr->memsize();
            textString="All operations completed!\nTime taken: "+textString.number((timer.elapsed()/1000)+1)+
                    " seconds\nAllocated memory: "+textString.number(mem)+" bytes\n";
            emit(state_text_out("Operations completed"));
            emit(state_detail_out(textString));
        }
        else{
            emit(state_text_out("In attesa..."));
            emit(state_detail_out("ERRORE: file non trovato, ricontrolla i percorsi e riprova\n"));
        }
    }
    else{
        emit(state_text_out("In attesa..."));
        emit(state_detail_out("ERRORE: percorso invalido, ricontrolla i percorsi e riprova\n"));
    }

    emit(finished());
}

void workerFileCopy::abort(){doAbort=true;}
