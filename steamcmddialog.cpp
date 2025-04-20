#include "steamcmddialog.h"
#include "ui_steamcmddialog.h"

SteamCMDDialog::SteamCMDDialog(QWidget *parent, QProcess *process, QString name)
    : QDialog(parent)
    , ui(new Ui::SteamCMDDialog)
{
    ui->setupUi(this);
    Process = process;

    connect(process, SIGNAL(readyReadStandardOutput()), SLOT(ReadOutput()));
    connect(process, SIGNAL(finished(int,QProcess::ExitStatus)), SLOT(InstallFinished()));

    this->setWindowTitle("Installing Server For " + name);

}

SteamCMDDialog::~SteamCMDDialog()
{
    delete ui;
}

void SteamCMDDialog::NewProcess(QProcess *process)
{
    ui->txtOutput->append("Running SteamCMD...");
    ui->barProgress->setValue(0);
    connect(process, SIGNAL(readyReadStandardOutput()), SLOT(ReadOutput()));
    connect(process, SIGNAL(finished(int,QProcess::ExitStatus)), SLOT(InstallFinished()));
    Process = process;
}

void SteamCMDDialog::ReadOutput()
{
    QByteArray output = Process->readAllStandardOutput();
    qInfo() << output;

    if (output.size() > 49)
        if (output.first(19).last(4) == "0x61")
            ui->barProgress->setValue(output.first(49).last(5).toFloat());

    ui->txtOutput->append(output);
}

void SteamCMDDialog::InstallFinished()
{
    if (this->isHidden())
        this->show();

    qInfo() << "Finished installing server.";
    ui->barProgress->setValue(100);
}

void SteamCMDDialog::on_btnCancel_clicked()
{
    //Process->kill();
    emit KillSteamCMDProcess();
}

