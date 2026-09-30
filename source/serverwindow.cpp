#include "serverwindow.h"
#include "ui_serverwindow.h"

ServerWindow::ServerWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ServerWindow)
{
    ui->setupUi(this);
}

ServerWindow::ServerWindow(SettingsStruct Settings, QWidget *parent, QString name, QString directory)
    : QWidget(parent)
    , ui(new Ui::ServerWindow)
{
    ui->setupUi(this);

    OS = QSysInfo::productType();
    if (OS != "windows" && OS != "macos")
    {
        OS = "linux";
    }

    SrvMain = new ServerProps_Main(this, name, directory);

    connect(SrvMain, &ServerProps_Main::ServerApplied, this, &ServerWindow::ServerApplied);
    connect(SrvMain, &ServerProps_Main::ServerInstalled, this, &ServerWindow::ServerInstalled);

    SrvConfig = new ServerProps_Config(this, directory);

    QSettings mainIniSettings("tf2-dsm_config.ini", QSettings::IniFormat);
    QStringList dirList = mainIniSettings.value(QString("%0/server_directories").arg(OS)).toStringList();

    if (!dirList.contains(directory))
    {
        LoadServerConfig(QDir(directory));
    }
    else
    {
        LoadServerFirstTimeSetup();
    }
    ServerFolder = directory;

    qInfo() << "Loading Settings.";
    LoadStyles(Settings.ColorTheme);

    PublicIP = Settings.PublicIP;

    SetServerVisualState();

    AddPropToLayout(SrvMain, "");
    AddPropToLayout(SrvConfig, "");

    HidePropLayout(SrvConfig);

    SrvMain->SetIP(PublicIP);
}

ServerWindow::~ServerWindow()
{
    delete ui;
}

void ServerWindow::printInfo(const QString message)
{
    qInfo() << QString("ServerWindow: %0").arg(message);
}

void ServerWindow::SettingsChanged(SettingsStruct Settings)
{
    PublicIP = Settings.PublicIP;

    //LoadStyles(Settings.ColorTheme);
}

void ServerWindow::UpdateStyles(QString colorTheme, QString fullStyle)
{
    LoadStyles(colorTheme, fullStyle);
}

void ServerWindow::LoadStyles(QString colorTheme, QString fullStyle)
{
    this->setStyleSheet(QString("QInputDialog { background-color: #2b2b2b; }"
                   "QMessageBox { background-color: #2b2b2b; }"));

    ui->listProps->setStyleSheet(QString("QListWidget { border: none; border-left: 3px solid %0; selection-background-color: %0; } QListWidget::item:selected { background-color: %0; }").arg(colorTheme));

    SrvMain->LoadStyles(colorTheme, fullStyle);
    SrvConfig->LoadStyles(colorTheme, fullStyle);
}

void ServerWindow::LoadServerConfig(QDir directory)
{
    SrvMain->LoadServerConfig(directory);
    SrvConfig->CheckServerConfigFiles();
    SrvConfig->SelectConfigFile("server.cfg");
}

void ServerWindow::SystemNotification(const QString title, const QString message, int length)
{
    emit PassSystemNotification(title, message, length);
}

void ServerWindow::AddPropToLayout(QWidget *prop, QString title)
{
    ui->vertLayoutProps->addWidget(prop);
}

void ServerWindow::HidePropLayout(QWidget *prop)
{
    prop->hide();
}

void ServerWindow::ShowPropLayout(QWidget *prop)
{
    prop->show();
}

void ServerWindow::LoadServerFirstTimeSetup()
{
    qInfo() << "First time setup.";

    SrvMain->FirstTimeSetup();
}

void ServerWindow::on_listProps_currentRowChanged(int currentRow)
{
    //ui->PropsMain->hide();
    //ui->PropsConfigs->hide();
    HidePropLayout(SrvMain);
    HidePropLayout(SrvConfig);

    switch (currentRow)
    {
    case PropTab::Main:
    {
        //ui->PropsMain->show();
        ShowPropLayout(SrvMain);

        break;
    }
    case PropTab::Config:
    {
        //ui->PropsConfigs->show);
        ShowPropLayout(SrvConfig);

        break;
    }
    case PropTab::SM:
    {
        break;
    }
    }
}

void ServerWindow::SetServerVisualState(VisualState state)
{
    SrvMain->SetServerVisualState(state);
    switch (state)
    {
    case VisualState::ServerDefault:
    {
        if (SrvMain->SRCDSExists())
        {
            ui->listProps->setEnabled(true);
        }
        else
        {
            ui->listProps->setEnabled(false);
        }
        break;
    }
    case VisualState::ServerStarted:
    {

        break;
    }
    case VisualState::ServerStopped:
    {

        break;
    }
    case VisualState::ServerDownloading:
    {

        break;
    }
    case VisualState::ServerInstalling:
    {

        break;
    }
    case VisualState::ServerFinishedInstalling:
    {

        if (SrvMain->SRCDSExists())
        {
            ui->listProps->setEnabled(true);
        }
        else
        {
            ui->listProps->setEnabled(false);
        }

        break;
    }
    }
}

QString ServerWindow::GetServerName()
{
    return SrvMain->GetName();
}

QString ServerWindow::GetServerIP(bool copyToClipboard)
{
    return SrvMain->GetIP(copyToClipboard);
}

void ServerWindow::StartServer()
{
    SrvMain->StartServer();
    emit PassServerActivated();
}

void ServerWindow::JoinServer()
{
    SrvMain->JoinServer();
}

void ServerWindow::ServerApplied(QString directory)
{
    SetServerVisualState();
    //emit PassServerApplied(directory);
}

void ServerWindow::ServerInstalled()
{
    SetServerVisualState();
    SrvConfig->ServerInstalled();
}