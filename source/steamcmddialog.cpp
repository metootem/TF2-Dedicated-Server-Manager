#include "steamcmddialog.h"
#include "ui_steamcmddialog.h"

SteamCMDDialog::SteamCMDDialog(QWidget *parent, QProcess *process, QString name)
    : QDialog(parent)
    , ui(new Ui::SteamCMDDialog)
{
    ui->setupUi(this);
    Process = process;

    connect(process, SIGNAL(readyRead()), SLOT(ReadOutput()));
    connect(process, SIGNAL(finished(int,QProcess::ExitStatus)), SLOT(InstallFinished(int,QProcess::ExitStatus)));

    this->setWindowTitle("Installing Server For " + name);
    ui->txtOutput->setText("Running SteamCMD...\nOutput may take a while, please be patient.\n");
}

SteamCMDDialog::~SteamCMDDialog()
{
    delete ui;
}

void SteamCMDDialog::NewProcess(QProcess *process)
{
    ui->txtOutput->append("Running SteamCMD...\nOutput may take a while, please be patient.\n");
    ui->barProgress->setValue(0);
    connect(process, SIGNAL(readyRead()), SLOT(ReadOutput()));
    connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), SLOT(InstallFinished(int, QProcess::ExitStatus)));
    Process = process;
}

void SteamCMDDialog::ReadOutput()
{
    QByteArray output = Process->readAllStandardOutput();

    if (output.size() > 49)
    {
        if (QSysInfo::productType() == "windows")
        {

        }
        else if (QSysInfo::productType() != "macos")
        {
            if (output.first(19).last(4) == "0x61")
                ui->barProgress->setValue(output.first(49).last(5).toFloat());
        }
    }

    ui->txtOutput->moveCursor(QTextCursor::End);
    ui->txtOutput->insertPlainText(output);
}

void SteamCMDDialog::InstallFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (this->isHidden())
        this->show();

    qInfo() << "Finished installing server.";
    if (exitStatus == QProcess::NormalExit)
    {
        ui->barProgress->setValue(100);
        ui->txtOutput->append("Finished installing server.\n");
    }
    else
        ui->txtOutput->append(QString("There was an error instaling the server. Error: %0\n").arg(Process->errorString()));
}

void SteamCMDDialog::on_btnCancel_clicked()
{
    //Process->kill();
    emit KillSteamCMDProcess();
}

