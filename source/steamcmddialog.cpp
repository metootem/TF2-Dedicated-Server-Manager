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
    connect(process, SIGNAL(finished(int,QProcess::ExitStatus)), SLOT(InstallFinished(int,QProcess::ExitStatus)));
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
    {
        errorCode = Error_NoDiskSpace;
    }
    else if (output.contains("0x206"))
    {
        errorCode = Error_Unknown;
    }
    else if (output.contains("0x402"))
    {
        errorCode = Error_SteamDown;
    }
    else if (output.contains("0x426"))
    {
        errorCode = Error_Interrupted;
    }
    else if (output.contains("0x606"))
    {
        errorCode = Error_NoPermission;
    }
    else if (output.contains("0x6"))
    {
        errorCode = Error_NoConnection;
    }

}

void SteamCMDDialog::InstallFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (this->isHidden())
    {
        this->show();
    }

    qInfo() << "Finished Running SteamCMD." << exitCode;
    if (exitStatus == QProcess::NormalExit)
    {
        ui->barProgress->setValue(100);
        ui->txtOutput->append(tr("Finished Running SteamCMD."));
        if (!errorCode)
        {
            ui->txtOutput->append(tr("Check console output for any errors."));
        }
        else
        {
            QString error;
            switch (errorCode)
            {
            case Error_NoDiskSpace:
            {
                error = tr("No disk space available to install server. Code: 0x202");
                break;
            }
            case Error_Unknown:
            {
                error = tr("Unknown error occurred.");
                break;
            }
            case Error_SteamDown:
            {
                error = tr("Steam servers are currently down. Retry later. Code: 0x402");
                break;
            }
            case Error_Interrupted:
            {
                error = tr("SteamCMD process has been interrupted. Please wait for other processes to finish. Code: 0x462");
                break;
            }
            case Error_NoPermission:
            {
                error = tr("Unable to write to the disk. Code: 0x606");
                break;
            }
            case Error_NoConnection:
            {
                error = tr("Couldn't connect to content servers. Code: 0x6");
                break;
            }
            }
            ui->txtOutput->append(error);
        }
    }
    else
    {
        ui->txtOutput->append(tr("There was an error installing the server. Error: %0\n").arg(Process->errorString()));
    }
}

