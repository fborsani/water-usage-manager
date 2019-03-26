#include "winCopyOptions.h"
#include "ui_winCopyOptions.h"

winCopyOptions::winCopyOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::winCopyOptions)
{
    ui->setupUi(this);
}

winCopyOptions::~winCopyOptions()
{
    delete ui;
}

void winCopyOptions::on_fileBrowser_clicked(){
    QFileDialog FileDial;
    FileDial.setFileMode(QFileDialog::AnyFile);
    ui->path->setText(FileDial.getSaveFileName(this,tr("Save as"),"C://",tr("Text file(*.txt)")));
}

void winCopyOptions::on_confirm_clicked(){

    comms::fileCopyArgs args;

    args.pathOut = ui->path->text();
    args.makeBackup = ui->makeBackup->isChecked();
    args.makeLog = ui->makeLog->isChecked();
    args.makeSort = ui->Sort->isChecked();
    args.checkDataLen = ui->checkCopy->isChecked();

    args.defDateLen = ui->DateLen->value();
    args.defUserLen = ui->UserLen->value();
    args.defConsLen = ui->ConsLen->value();

    args.refreshRate = ui->RefreshRate->value();

    emit(argsStruct_out(args));

    this->close();
}

void winCopyOptions::on_reset_clicked(){
    this->close();
}
