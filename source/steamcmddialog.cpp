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

    errorCode = 0;
    this->setWindowTitle("Installing Server For " + name);
    ui->txtOutput->setText("Running SteamCMD...\nOutput may take a while, please be patient.\n\n");
}

SteamCMDDialog::~SteamCMDDialog()
{
    delete ui;
}

void SteamCMDDialog::NewProcess(QProcess *process)
{
    ui->txtOutput->append(tr("Running SteamCMD...\nOutput may take a while, please be patient.\n\n"));
    ui->barProgress->setValue(0);
    connect(process, SIGNAL(readyRead()), SLOT(ReadOutput()));
    connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), SLOT(InstallFinished(int, QProcess::ExitStatus)));
    Process = process;
    errorCode = 0;
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

    if (output.contains("0x202"))
        errorCode += 1;
}

void SteamCMDDialog::InstallFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (this->isHidden())
        this->show();

    qInfo() << "Finished Running SteamCMD.";
    if (exitStatus == QProcess::NormalExit)
    {
        ui->barProgress->setValue(100);
        ui->txtOutput->append(tr("Finished Running SteamCMD."));
        if (!errorCode)
            ui->txtOutput->append(tr("Check console output for any errors."));
        else
        {
            if (errorCode & Error_NoDiskSpace)
                ui->txtOutput->append(tr("No disk space available to install server."));

        }
    }
    else
        ui->txtOutput->append(tr("There was an error installing the server. Error: %0\n").arg(Process->errorString()));
}

