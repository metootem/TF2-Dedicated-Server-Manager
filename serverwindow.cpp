#include "serverwindow.h"
#include "ui_serverwindow.h"

ServerWindow::ServerWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ServerWindow)
{
    ui->setupUi(this);
}

ServerWindow::ServerWindow(QWidget *parent, QString name, QString directory)
    : QWidget(parent)
    , ui(new Ui::ServerWindow)
{
    ui->setupUi(this);
    ui->lineServerName->setText(name);
    ui->PropsConfigs->setGeometry(0, 0, 601, 411);
    ui->PropsConfigs->hide();
    ui->lblFolderError->hide();
    if (!directory.isEmpty())
        LoadServerConfig(QDir(directory));
    OS = QSysInfo::productType();
    if (OS != "windows" && OS != "macos")
        OS = "linux";
}

ServerWindow::~ServerWindow()
{
    delete ui;
}

void ServerWindow::LoadServerConfig(QDir directory)
{
    QSettings IniSettings(tr("%0/server.ini").arg(directory.path()), QSettings::Format::IniFormat);

    ui->lineServerName->setText(IniSettings.value("server_name").toString());

    ServerFolder = directory.path();
    ui->lineFolderName->setText(QDir(ServerFolder).dirName());

    ui->lineIP->setText(IniSettings.value("ip").toString());

    ui->linePort->setText(IniSettings.value("port").toString());

    ui->spinMaxPlayers->setValue(IniSettings.value("players").toInt());

    ui->lineMap->setText(IniSettings.value("map").toString());

    ui->lineParameters->setText(IniSettings.value("parameters").toString());
}

void ServerWindow::on_listProps_currentRowChanged(int currentRow)
{
    ui->PropsMain->hide();
    ui->PropsConfigs->hide();
    switch (currentRow)
    {
    case 0:
    {
        ui->PropsMain->show();

        break;
    }
    case 1:
    {
        ui->PropsConfigs->show();

        break;
    }
    }
}


void ServerWindow::on_btnApply_clicked()
{
    bool apply = true;
    if (ui->lineServerName->text().isEmpty())
        ui->lineServerName->setText("Team Fortress 2 Server");
    if (ui->lineFolderName->text().isEmpty())
    {
        ui->lblFolderError->show();
        apply = false;
    }
    if (ui->lineIP->text().isEmpty())
        ui->lineIP->setText("0.0.0.0");
    if (ui->linePort->text().isEmpty())
        ui->linePort->setText("27015");

    if (apply)
    {
        ui->lblFolderError->hide();

        QDir(ServerFolder).dirName() = ui->lineFolderName->text();

        SettingsDialog* settingsDialog = new SettingsDialog(parentWidget());
        SettingsStruct settings = settingsDialog->ParseSettings();
        if (!settings.valid)
        {
            settingsDialog->show();
            return;
        }
        QString path = tr("%0/%1/server.ini").arg(settings.ServerDirectory.path()).arg(ui->lineFolderName->text(), 1);
        ServerFolder = tr("%0/%1").arg(settings.ServerDirectory.path()).arg(ui->lineFolderName->text(), 1);

        QSettings IniSettings(path, QSettings::Format::IniFormat);
        IniSettings.setValue("server_name", ui->lineServerName->text());
        IniSettings.setValue("ip", ui->lineIP->text());
        IniSettings.setValue("port", ui->linePort->text());
        IniSettings.setValue("players", ui->spinMaxPlayers->value());
        IniSettings.setValue("map", ui->lineMap->text());
        IniSettings.setValue("parameters", ui->lineParameters->text());

        emit ServerApplied( ServerFolder );
    }
}


void ServerWindow::on_btnInstallServer_clicked()
{

}


void ServerWindow::on_btnStartServer_clicked()
{
    QProcess Process;
    QString Command;
    if (OS == "linux")
        Command = tr("%0/Server/srcds_run").arg(ServerFolder);
    else
        Command = tr("%0/Server/srcds_run").arg(ServerFolder);

    QStringList args =  {"-game tf "} ;

    if (ui->chkConsole->checkState())
        args << "-console ";

    args << tr("+ip %0 ").arg(ui->lineIP->text());
    args << tr("-port %0 ").arg(ui->linePort->text());

    if (ui->lineMap->text().isEmpty())
        args << "+randommap ";
    else
        args << tr("+map %0 ").arg(ui->lineMap->text());

    qInfo() << Command;
    qInfo() << args;

    Process.start(Command, args, QIODevice::OpenModeFlag::ReadOnly);
    Process.waitForFinished();

    qInfo() << Process.readAllStandardOutput();
    qInfo() << Process.readAllStandardError();
}

