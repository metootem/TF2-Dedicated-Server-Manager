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
}

ServerWindow::~ServerWindow()
{
    delete ui;
}

void ServerWindow::LoadServerConfig(QDir directory)
{
    QSettings IniSettings(tr("%0/server.ini").arg(directory.path()), QSettings::Format::IniFormat);

    ui->lineServerName->setText(IniSettings.value("server_name").toString());

    ServerFolder = directory.dirName();
    ui->lineFolderName->setText(ServerFolder);

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

        ServerFolder = ui->lineFolderName->text();

        SettingsDialog* settingsDialog = new SettingsDialog(parentWidget());
        SettingsStruct settings = settingsDialog->ParseSettings();
        if (!settings.valid)
        {
            settingsDialog->show();
            return;
        }
        QString path = tr("%0/%1/server.ini").arg(settings.ServerDirectory.path()).arg(ui->lineFolderName->text(), 1);
        qInfo() << path;

        QSettings IniSettings(path, QSettings::Format::IniFormat);
        IniSettings.setValue("server_name", ui->lineServerName->text());
        IniSettings.setValue("ip", ui->lineIP->text());
        IniSettings.setValue("port", ui->linePort->text());
        IniSettings.setValue("players", ui->spinMaxPlayers->value());
        IniSettings.setValue("map", ui->lineMap->text());
        IniSettings.setValue("parameters", ui->lineParameters->text());
    }
}

