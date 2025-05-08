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
    ui->PropsConfigs->setGeometry(0, 0, 631, 411);
    ui->PropsConfigs->hide();
    ui->lblFolderError->hide();

    if (!directory.isEmpty())
    {
        IniSettings = new QSettings(tr("%0/server.ini").arg(directory), QSettings::Format::IniFormat);
        LoadServerConfig(QDir(directory));
    }
    else
        LoadServerFirstTimeSetup();

    OS = QSysInfo::productType();
    if (OS != "windows" && OS != "macos")
        OS = "linux";

    QSettings mainIniSettings("tf2-dsm_config.ini", QSettings::Format::IniFormat);
    qInfo() << "Loading Main Ini Settings.";
    LoadStyles(mainIniSettings.value(OS + "/color_theme").toString());

    SteamCMDProcess = nullptr;
    ServerProcess = nullptr;
    SteamCMDWindow = nullptr;

    PublicIP = GetPublicIP();

    SetServerVisualState();
}

ServerWindow::~ServerWindow()
{
    delete ui;
}

void ServerWindow::SettingsChanged(SettingsStruct Settings)
{
    LoadStyles(Settings.ColorTheme);
}

void ServerWindow::LoadStyles(QString colorTheme)
{
    ui->listProps->setStyleSheet(QString("QListWidget {\n	border: none;\nborder-left: 3px solid %0;\n}\n\nQListWidget::item:selected {\n	background-color: %0;\n}").arg(colorTheme));
    if (OS == "windows")
        ui->cmbConfigFile->setStyleSheet("QComboBox {\n	color: #000000;\n}");
    else if (OS == "linux")
        ui->cmbConfigFile->setStyleSheet("QComboBox {\n	color: #ffffff;\n}");

    qInfo() << "Updated Styles.";
}

void ServerWindow::LoadServerConfig(QDir directory)
{
    ui->lineServerName->setText(IniSettings->value("server_name").toString());

    ServerFolder = directory.path();
    ui->lineFolderName->setText(QDir(ServerFolder).dirName());

    ui->lineIP->setText(IniSettings->value("ip").toString());

    ui->linePort->setText(IniSettings->value("port").toString());

    ui->spinMaxPlayers->setValue(IniSettings->value("players").toInt());

    ui->lineMap->setText(IniSettings->value("map").toString());

    AdditionalParametersWindow = new AdditionalParametersDialog(this, IniSettings);

    CheckServerConfigFiles();
}


void ServerWindow::LoadServerFirstTimeSetup()
{
    qInfo() << "First time setup.";

    ui->btnGotoServerFolder->setEnabled(false);

    AdditionalParametersWindow = new AdditionalParametersDialog(this);
    AdditionalParametersWindow->FirstTimeSetup();
}


QString ServerWindow::GetPublicIP()
{
    QProcess GetIP;
    GetIP.start("curl", QStringList() << "https://api.ipify.org");
    GetIP.waitForFinished(10000);
    QString IP = GetIP.readAllStandardOutput();
    GetIP.terminate();
    if (IP.isEmpty())
        qInfo() << "Couldn't get Public IP.";
    else
        qInfo() << "Got Public IP:" << IP;
    return IP;
}


bool ServerWindow::SteamCMDExists()
{
    if (OS == "windows")
    {
        QFile SteamCMD(ServerFolder+"/SteamCMD/steamcmd.exe");
        return SteamCMD.exists();
    }
    else if (OS == "linux")
    {
        QFile SteamCMD(ServerFolder+"/SteamCMD/steamcmd.sh");
        return SteamCMD.exists();
    }

    return false;
}

bool ServerWindow::SteamCMDZipExists()
{
    if (OS == "windows")
    {
        QFile SteamCMD(ServerFolder+"/SteamCMD/steamcmd.zip");
        return SteamCMD.exists();
    }
    else if (OS == "linux")
    {
        QFile SteamCMD(ServerFolder+"/SteamCMD/steamcmd_linux.tar.gz");
        return SteamCMD.exists();
    }

    return false;
}

bool ServerWindow::SRCDSExists()
{
    if (OS == "windows")
    {
        QFile Srcds(ServerFolder+"/Server/srcds.exe");
        return Srcds.exists();
    }
    else if (OS == "linux")
    {
        QFile Srcds(ServerFolder+"/Server/srcds_run");
        return Srcds.exists();
    }

    return false;
}

void ServerWindow::DownloadSteamCMD()
{
    SetServerVisualState(VisualState::ServerDownloading);

    ui->btnInstallServer->setText("Downloading...");

    if (!SteamCMDZipExists())
    {
        qInfo() << "Downloading SteamCMD";

        QString SteamCMDPath = ServerFolder + "/SteamCMD";

        QDir dir(SteamCMDPath);
        if (!dir.exists())
            dir.mkdir(SteamCMDPath);

        QString SteamCMDUrl;
        if (OS == "windows")
            SteamCMDUrl = "https://steamcdn-a.akamaihd.net/client/installer/steamcmd.zip";
        else if (OS == "linux")
            SteamCMDUrl = "https://steamcdn-a.akamaihd.net/client/installer/steamcmd_linux.tar.gz";

        auto download = new FileDownloader(this);
        connect(download, SIGNAL(finished()), SLOT(InstallSteamCMD()));
        download->downloadFile(QUrl(SteamCMDUrl), QDir(ServerFolder + "/SteamCMD"));
    }
    else
        InstallSteamCMD();

}


void ServerWindow::InstallSteamCMD()
{
    SetServerVisualState(VisualState::ServerInstalling);
    QString SteamCMDPath = ServerFolder + "/SteamCMD";

    QString SteamCMDFile;
    if (OS == "windows")
        SteamCMDFile = "steamcmd.zip";
    else if (OS == "linux")
        SteamCMDFile = "steamcmd_linux.tar.gz";

    if (OS == "linux")
    {
        QMessageBox msgBox(QMessageBox::Icon::Question, "",
                           tr("Make sure you have the requirements:\n"
                              "https://wiki.teamfortress.com/wiki/Linux_dedicated_server#Requirements"), {}, this);
        auto *accept = msgBox.addButton("Continue", QMessageBox::ButtonRole::AcceptRole);
        msgBox.addButton("Cancel", QMessageBox::ButtonRole::RejectRole);
        msgBox.exec();
        if (msgBox.clickedButton() != accept)
            return;
    }

    ServerInstalling = true;

    ui->btnInstallServer->setText("Unpacking...");
    qInfo() << "unpacking";
    QProcess unpack;
    unpack.setWorkingDirectory(SteamCMDPath);
    unpack.start("tar", QStringList() << "zxf" << SteamCMDFile);

    if (!unpack.waitForFinished())
    {
        qInfo() << "There was an error unpacking steamcmd: " << unpack.errorString();
        qInfo() << "Aborting installation.";
        QMessageBox msgBox(QMessageBox::Icon::Critical, "Error",
                           tr("There was an error unpacking steamcmd: %0.").arg(unpack.errorString()), {}, this);
        msgBox.show();
        SetServerVisualState();
        return;
    }

    if (QSysInfo::productType() == "nobara" || QSysInfo::productType() == "fedora")
    {
        QProcess::execute("mkdir", QStringList() << "-p" << "~/.steam/sdk32");

        QProcess::execute("ln", QStringList() << "-s" << SteamCMDPath + "/linux32/steamclient.so" << "~/.steam/sdk32");

        QProcess::execute("ln", QStringList() << "-s" << "/usr/lib/libcurl.so.4" << "/usr/lib/libcurl-gnutls.so.4");
    }
    else if (OS == "windows")
    {
        unpack.start("steamcmd.exe");
        if (unpack.waitForFinished())
        {
            qInfo() << "There was an error installing SteamCMD files:" << unpack.errorString();
            QMessageBox msgBox(QMessageBox::Icon::Critical, "Error",
                               tr("There was an error installing SteamCMD files: %0.").arg(unpack.errorString()), {}, this);
            msgBox.show();
            return;
        }
    }

    InstallServer();
}

void ServerWindow::InstallServer()
{
    SetServerVisualState(VisualState::ServerInstalling);

    if (SteamCMDProcess == nullptr)
    {
        SteamCMDProcess = new QProcess(this);
        SteamCMDProcess->setWorkingDirectory(ServerFolder + "/SteamCMD");
        SteamCMDProcess->setProcessChannelMode(QProcess::MergedChannels);
    }

    if (SteamCMDWindow == nullptr)
        SteamCMDWindow = new SteamCMDDialog(this, SteamCMDProcess, ui->lineServerName->text());
    else
        SteamCMDWindow->NewProcess(SteamCMDProcess);

    connect(SteamCMDWindow, SIGNAL(KillSteamCMDProcess()), SLOT(KillSteamCMDProcess()));

    ui->btnInstallServer->setText("Installing...");

    QDir dir(ServerFolder + "/Server");
    if (!dir.exists())
        dir.mkdir(ServerFolder + "/Server");

    QStringList betaList;

    if (ui->chkBeta->isChecked())
    {
        qInfo() << "Opting into beta.";
        betaList << "-beta" << "prerelease";
    }
    betaList << "+quit";

    if (OS == "linux")
        SteamCMDProcess->start("./steamcmd.sh", QStringList() << "+force_install_dir" << ServerFolder + "/Server" << "+login" << "anonymous" << "+app_update" << "232250" << betaList);
    else if (OS == "windows")
        SteamCMDProcess->start(QString("%0/SteamCMD/steamcmd.exe").arg(ServerFolder), QStringList() << "+force_install_dir" << ServerFolder + "/Server" << "+login" << "anonymous" << "+app_update" << "232250" << betaList);

    if (SteamCMDProcess->waitForStarted())
    {
        if (SteamCMDWindow->isHidden())
            SteamCMDWindow->show();
        connect(SteamCMDProcess, SIGNAL(finished(int,QProcess::ExitStatus)), SLOT(InstallServerFinished()));
    }
    else
    {
        if (OS == "windows")
        {
            SteamCMDProcess->start(QString("%0/SteamCMD/steamcmd.exe").arg(ServerFolder));
            if (!SteamCMDProcess->waitForFinished())
            {
                qInfo() << "There was an error installing the server:" << SteamCMDProcess->errorString();
                QMessageBox msgBox(QMessageBox::Icon::Critical, "Error",
                                   tr("Couldn't run SteamCMD. Error: %0").arg(SteamCMDProcess->errorString()), {}, this);
                msgBox.exec();
                return;
            }
            else
                InstallServer();
            return;
        }
        qInfo() << "Couldn't run steamcmd installation.";
        delete SteamCMDWindow;
        SetServerVisualState();
        QMessageBox msgBox(QMessageBox::Icon::Critical, "Error",
                           tr("Couldn't run SteamCMD. Error: %0").arg(SteamCMDProcess->errorString()), {}, this);
        msgBox.exec();
    }
}

void ServerWindow::KillSteamCMDProcess()
{
    qInfo() << "Attempting to terminate SteamCMD. " << SteamCMDProcess->processId();
    SteamCMDProcess->terminate();
}

void ServerWindow::InstallServerFinished()
{
    SteamCMDProcess->deleteLater();
    SteamCMDProcess = nullptr;
    SetServerVisualState();

    QFile serverCfg(ServerFolder + "/Server/tf/cfg/server.cfg");
    if (!serverCfg.exists())
    {
        serverCfg.open(QIODevice::WriteOnly);
        serverCfg.write(ServerCfgExample().toStdString().c_str());
        serverCfg.flush();
        serverCfg.close();
    }

    CheckServerConfigFiles();
}


void ServerWindow::on_btnInstallServer_clicked()
{
    if (!SteamCMDExists())
    {
        if (!SteamCMDZipExists())
        {
            QMessageBox msgBox(QMessageBox::Icon::Question, "",
                               tr("SteamCMD not found.\nDo you want to download automatically?"), {}, this);
            auto *accept = msgBox.addButton("Accept", QMessageBox::ButtonRole::AcceptRole);
            msgBox.addButton("Cancel", QMessageBox::ButtonRole::RejectRole);
            msgBox.exec();
            if (msgBox.clickedButton() == accept)
                DownloadSteamCMD();
        }
        else
        {
            qInfo() << "SteamCMD zip found. Installing SteamCMD.";
            InstallSteamCMD();
        }
    }
    else
    {
        qInfo() << "SteamCMD found. Installing server";
        InstallServer();
    }
}


void ServerWindow::on_btnApply_clicked()
{
    ui->btnGotoServerFolder->setEnabled(false);

    bool apply = true;

    if (ui->lineServerName->text().isEmpty())
        ui->lineServerName->setText("Team Fortress 2 Server");
    qInfo() << "Server Name:" << ui->lineServerName->text();
    if (ui->lineFolderName->text().isEmpty())
    {
        ui->lblFolderError->show();
        qInfo() << "Folder is invalid!";
        apply = false;
    }
    else
        qInfo() << "Folder:" << ui->lineFolderName->text();


    if (ui->lineIP->text().isEmpty())
        ui->lineIP->setText("0.0.0.0");
    qInfo() << "IP:" << ui->lineIP->text();

    if (ui->linePort->text().isEmpty())
        ui->linePort->setText("27015");
    qInfo() << "Port:" << ui->linePort->text();

    if (apply)
    {
        ui->lblFolderError->hide();

        //QDir(ServerFolder).dirName() = ui->lineFolderName->text();

        SettingsDialog* settingsDialog = new SettingsDialog(parentWidget());
        qInfo() << "Parsing settings.";
        SettingsStruct settings = settingsDialog->ParseSettings();
        if (!settings.valid)
        {
            qInfo() << "Settings invalid!";
            settingsDialog->show();
            return;
        }

        if (!ServerFolder.isEmpty())
        {
            delete IniSettings;
            IniSettings = nullptr;

            if (QFile::rename(ServerFolder, QString("%0/%1").arg(settings.ServerDirectory.path(), ui->lineFolderName->text())))
                ServerFolder = QString("%0/%1").arg(settings.ServerDirectory.path(), ui->lineFolderName->text());
            else
            {
                ui->lblFolderError->setText("Server Folder cannot be renamed currently!");
                ui->lblFolderError->show();
                ui->lineFolderName->setText(QDir(ServerFolder).dirName());
            }
        }
        else
            ServerFolder = QString("%0/%1").arg(settings.ServerDirectory.path(), ui->lineFolderName->text());

        if (IniSettings == nullptr)
            IniSettings = new QSettings(ServerFolder + "/server.ini", QSettings::Format::IniFormat);

        qInfo() << ServerFolder;
        qInfo() << IniSettings;
        qInfo() << "Saving server config.";
        IniSettings->setValue("server_name", ui->lineServerName->text());
        qInfo() << "server_name...";
        IniSettings->setValue("ip", ui->lineIP->text());
        qInfo() << "ip...";
        IniSettings->setValue("port", ui->linePort->text());
        qInfo() << "port...";
        IniSettings->setValue("players", ui->spinMaxPlayers->value());
        qInfo() << "players...";
        IniSettings->setValue("map", ui->lineMap->text());
        qInfo() << "map...";
        IniSettings->setValue("parameters", AdditionalParametersWindow->GetParameters());
        qInfo() << "parameters...";
        IniSettings->setValue("os", OS);
        qInfo() << "os...";

        qInfo() << "Saved.";

        SetServerVisualState();
        if (QSystemTrayIcon::isSystemTrayAvailable())
        {
            QSystemTrayIcon icon;
            icon.show();
            icon.showMessage(ui->lineServerName->text(), "Settings applied", QIcon(":/icons/resources/Images/tf2dsm_logo.png"));
            icon.hide();
        }
        ui->btnGotoServerFolder->setEnabled(true);
        emit ServerApplied( ServerFolder );
    }
}

void ServerWindow::on_btnStartServer_clicked()
{
    QString Command;
    if (OS == "linux")
        Command = QString("%0/Server/srcds_run").arg(ServerFolder);
    else
        Command = QString("%0/Server/srcds.exe").arg(ServerFolder);

    QStringList args = {"-console", "-game", "tf"};

    args << "+ip" << ui->lineIP->text();
    args << "-port" << ui->linePort->text();
    args << "+maxplayers" << ui->spinMaxPlayers->text();

    if (ui->lineMap->text().isEmpty())
        args << "+randommap";
    else
        args << "+map" << ui->lineMap->text();

    if (!ui->lineServerName->text().isEmpty())
        args << "+hostname" << "\"" + ui->lineServerName->text() + "\"";

    QStringList additionalParams = AdditionalParametersWindow->GetParameters();
    for (int i = 2; i < additionalParams.count(); i+=3)
    {
        if (additionalParams[i] != "True")
            continue;
        if (additionalParams[i-2].first(1) == "-" || additionalParams[i-2].first(1) == "+")
        {
            args << additionalParams[i-2];
            if (!additionalParams[i-1].isEmpty())
                args << additionalParams[i-1];
        }
    }

    qInfo() << "Running srcds: " << Command;
    qInfo() << "Arguments: " << args;

    auto Process = new QProcess(this);

    //ServerProcess = Process;
    Process->setProcessChannelMode(QProcess::MergedChannels);
    Process->setWorkingDirectory(QString("%0/Server").arg(ServerFolder));

    //QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    //env.insert("LD_LIBRARY_PATH", ".:bin:" + env.value("LD_LIBRARY_PATH"));
    //Process->setProcessEnvironment(env);
    //qInfo() << env.value("LD_LIBRARY_PATH");

    if (ui->chkConsole->isChecked())
    {
        qInfo() << "Running server in system console.";
        if (OS == "windows")
        {
            Process->startDetached("cmd.exe", QStringList() << "/k" << Command << args);
            Process->deleteLater();
        }
        else
        {
            QStringList Terminals = {"gnome-terminal", "konsole", "xterm"};
            bool started = false;
            for (QString term : Terminals)
            {
                if (Process->startDetached(term, QStringList() << "-e" << Command << args))
                {
                    qInfo() << "Found terminal: " + term;
                    started = true;
                    break;
                }
            }
            if (!started)
            {
                bool ok;
                QString term = QInputDialog::getText(this, tr("Linux Terminal Not Found"),
                                                     tr("Your Terminal:"), QLineEdit::Normal,
                                                     QDir::home().dirName(), &ok);
                if (!ok || term.isEmpty())
                    return;

                Process->startDetached(term, QStringList() << "-e" << Command << args);
            }
        }
        return;
    }

    Process->start(Command, args, QProcess::ReadWrite | QProcess::Text | QProcess::Unbuffered);

    if (Process->waitForStarted())
    {
        qInfo() << "server running";
        auto ServerConsoleDial = new ServerConsoleDialog(this, Process, ui->lineServerName->text());
        ServerConsole = ServerConsoleDial;

        if (!ui->chkConsole->isChecked())
            ServerConsole->show();

        SetServerVisualState(ServerStarted);

        connect(Process, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(ServerStateChanged(QProcess::ProcessState)));

        emit ServerActivated();
    }
    else
    {
        SetServerVisualState(VisualState::ServerStopped);
    }
}

/*
void ServerWindow::on_btnStopServer_clicked()
{
    if (ServerProcess->state() == QProcess::ProcessState::Running)
    {
        if (ui->chkConsole->isChecked())
            ServerProcess->kill();
        else
        {
            // Very roundabout and system specific because for SOME reason
            // the child process isn't terminated when main server process is started in App
            // (but is terminated when started outside)
            QProcess Process;
            Process.start("ps", QStringList() << "--ppid" << QString::number(ServerProcess->processId()));
            Process.waitForFinished();
            QString stdout = Process.readAllStandardOutput();
            qInfo() << stdout.right(36);
            QString srcds;
            for (QChar c : stdout.right(36))
            {
                if (c == ' ')
                    break;
                if (c.isDigit())
                    srcds.append(c);
            }
            qInfo() << srcds.prepend("kill -TERM ");

            if (!srcds.isEmpty())
            {
                ServerProcess->terminate();
                ServerProcess->waitForFinished();
                system(srcds.toStdString().c_str());
            }

            if (!ServerConsole->isHidden())
                ServerConsole->close();
        }

        emit ServerDeactivated();
    }
    SetServerVisualState(VisualState::ServerStopped);
}*/


void ServerWindow::on_btnShowConsole_clicked()
{
    if (ServerProcess->state() != QProcess::Running)
        return;
    if (ServerConsole->isHidden())
        ServerConsole->show();
    else
        ServerConsole->setFocus();
}


void ServerWindow::on_btnConnectToServer_clicked()
{
    QString LocalServerAddress;
    QHostAddress host(QHostAddress::LocalHost);
    for (QHostAddress address : QNetworkInterface::allAddresses())
    {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != host)
            LocalServerAddress = address.toString() + ":" + ui->linePort->text();
    }

    QString PublicServerAddress = PublicIP + ":" + ui->linePort->text();

    qInfo() << LocalServerAddress;
    qInfo() << PublicServerAddress;

    QMessageBox msgBox(QMessageBox::Icon::Question, "",
                       tr("Through which IP to join server?\nPublic: %0\nLocal: %1").arg(PublicServerAddress).arg(LocalServerAddress, 1), {}, this);

    auto *publicIp = msgBox.addButton("Public IP", QMessageBox::ButtonRole::DestructiveRole);
    if (PublicServerAddress.isEmpty())
        publicIp->setEnabled(false);
    auto *localIp = msgBox.addButton("Local IP", QMessageBox::ButtonRole::RejectRole);

    msgBox.exec();
    if (msgBox.clickedButton() == publicIp)
        QDesktopServices::openUrl(QUrl("steam://connect/" + PublicServerAddress));
    else if (msgBox.clickedButton() == localIp)
        QDesktopServices::openUrl(QUrl("steam://connect/" + LocalServerAddress));
}


void ServerWindow::on_btnGotoServerFolder_clicked()
{
    if (!ui->lineFolderName->text().isEmpty())
        QDesktopServices::openUrl(QUrl("file://" + ServerFolder));
}


void ServerWindow::on_btnCopyIp_clicked()
{
    QClipboard *clip = QApplication::clipboard();
    QString IP;
    if (ui->lineIP->text() == "0.0.0.0" || ui->lineIP->text().isEmpty())
        IP = PublicIP + ":" + ui->linePort->text();
    else
        IP = ui->lineIP->text() + ":" + ui->linePort->text();

    clip->setText(IP);

    if (QSystemTrayIcon::isSystemTrayAvailable())
    {
        QSystemTrayIcon icon;
        icon.show();
        icon.showMessage("Copied Public IP to clipboard", IP, QIcon(":/icons/resources/Images/tf2dsm_logo.png"));
        icon.hide();
    }
}


void ServerWindow::on_btnSelectMap_clicked()
{
    auto dialog = new SelectMapDialog(this, ServerFolder);
    if (dialog->exec() == QDialog::Accepted)
    {
        qInfo() << "accepted";
        if (dialog)
            ui->lineMap->setText(dialog->SelectMap());
    }
    delete dialog;
}

void ServerWindow::on_btnParameters_clicked()
{
    AdditionalParametersWindow->show();
}

void ServerWindow::on_btnSteamCMDConsole_clicked()
{
    if (SteamCMDWindow != nullptr)
        SteamCMDWindow->show();
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


void ServerWindow::SetServerVisualState(VisualState state)
{
    switch (state)
    {
    case ServerDefault:
    {
        ui->btnInstallServer->setText((SteamCMDExists() ? "Update" : "Install"));
        if (SRCDSExists())
        {
            ui->btnStartServer->setEnabled(true);
            ui->listProps->setEnabled(true);
        }
        else
        {
            ui->btnStartServer->setEnabled(false);
            ui->listProps->setEnabled(false);
        }


        ui->btnInstallServer->setEnabled(true);
        ui->btnApply->setEnabled(true);

        ui->btnShowConsole->setEnabled(false);
        ui->btnStopServer->setEnabled(false);
        break;
    }
    case ServerStarted:
    {
        ui->btnStopServer->setEnabled(true);
        //ui->btnConnectToServer->setEnabled(true);
        ui->btnShowConsole->setEnabled(true);

        ui->btnStartServer->setEnabled(false);
        ui->btnApply->setEnabled(false);
        break;
    }
    case ServerStopped:
    {
        ui->btnStartServer->setEnabled(true);
        ui->btnApply->setEnabled(true);

        //ui->btnConnectToServer->setEnabled(false);
        ui->btnShowConsole->setEnabled(false);
        ui->btnStopServer->setEnabled(false);
        break;
    }
    case ServerDownloading:
    {
        ui->btnSteamCMDConsole->setEnabled(false);
        ui->btnInstallServer->setEnabled(false);
        ui->btnStartServer->setEnabled(false);
        ui->btnStopServer->setEnabled(false);
        ui->btnApply->setEnabled(false);
        break;
    }
    case VisualState::ServerInstalling:
    {
        ui->btnSteamCMDConsole->setEnabled(true);

        ui->btnInstallServer->setEnabled(false);
        ui->btnStartServer->setEnabled(false);
        ui->btnStopServer->setEnabled(false);
        ui->btnApply->setEnabled(false);
        break;
    }
    case ServerFinishedInstalling:
    {
        ui->btnInstallServer->setEnabled(true);
        ui->btnInstallServer->setText((SteamCMDExists() ? "Update" : "Install"));
        if (SRCDSExists())
        {
            ui->btnStartServer->setEnabled(true);
            ui->listProps->setEnabled(true);
        }
        else
        {
            ui->btnStartServer->setEnabled(false);
            ui->listProps->setEnabled(false);
        }

        ServerInstalling = false;

        ui->btnApply->setEnabled(true);

        break;
    }
    }
}

void ServerWindow::CheckServerConfigFiles()
{
    ui->cmbConfigFile->clear();
    for (QFileInfo file : QDir(ServerFolder + "/Server/tf/cfg").entryInfoList(QStringList() << "*.cfg" << "*.txt", QDir::Files))
        ui->cmbConfigFile->addItem(file.fileName());
}

void ServerWindow::LoadServerConfigFileData()
{

}

void ServerWindow::on_btnAddConVar_clicked()
{
    QStringList parentItems;
    for (int i=0; i<ui->treeConfigFileData->topLevelItemCount(); i++)
    {
        QString name = ui->treeConfigFileData->topLevelItem(i)->text(0);
        if (name.first(2) == "//" && name.last(2) == "//" && name.length() > 3)
            parentItems << name;
    }

    auto dialog = new ConfigConVarDialog(parentItems, this);
    int code = dialog->exec();
    int selectedParentItemIndex = dialog->selectedIndex;
    dialog->deleteLater();

    if (!selectedParentItemIndex && code)
        AddConfigTreeItem("NewConVar", "Value", "");
    else if (code)
        AddConfigTreeItem("NewConvar", "Value", "", ui->treeConfigFileData->topLevelItem(selectedParentItemIndex-1));
}

void ServerWindow::on_btnDelConVar_clicked()
{
    auto item = ui->treeConfigFileData->currentItem();
    if (item != nullptr)
    {
        if (!item->childCount())
            delete item;
        else
        {
            QMessageBox msgBox(QMessageBox::Icon::Warning, "",
                               tr("Selection has multiple child convars.\nProceed to delete?"), {}, this);
            auto *accept = msgBox.addButton("Accept", QMessageBox::ButtonRole::AcceptRole);
            msgBox.addButton("Cancel", QMessageBox::ButtonRole::RejectRole);
            msgBox.exec();
            if (msgBox.clickedButton() == accept)
                delete item;
        }
    }
}

void ServerWindow::on_btnReloadConfig_clicked()
{
    on_cmbConfigFile_currentTextChanged(ui->cmbConfigFile->currentText());
}

void ServerWindow::on_btnOpenConfig_clicked()
{
    QString path = "file://" + ServerFolder + "/Server/tf/cfg/" + ui->cmbConfigFile->currentText();
    QDesktopServices::openUrl(QUrl(path));
}

void ServerWindow::on_btnConfigSpecial_clicked()
{
    QString fileName = ui->cmbConfigFile->currentText();
    if (fileName.length() > 8)
    {
        if (fileName.first(8) == "mapcycle")
        {
            QStringList parentItems;
            for (int i=0; i<ui->treeConfigFileData->topLevelItemCount(); i++)
                parentItems << ui->treeConfigFileData->topLevelItem(i)->text(0) + ".bsp";
            qInfo() << parentItems;
            for (QFileInfo fileInfo : QDir(ServerFolder + "/Server/tf/maps").entryInfoList(QStringList() << "*.bsp", QDir::Files))
            {
                qInfo() << fileInfo.fileName();
                if (!parentItems.contains(fileInfo.fileName()))
                    AddConfigTreeItem(fileInfo.fileName(), "", "");
            }
        }
    }
}

void ServerWindow::on_btnFindConVar_clicked()
{
    if (!ui->treeConfigFileData->topLevelItemCount())
        return;

    ui->treeConfigFileData->clearSelection();
    QString target = QInputDialog::getText(this, "Find ConVar", "ConVar to find:");

    if (target.isEmpty())
        return;

    for (int i=0; i<ui->treeConfigFileData->topLevelItemCount(); i++)
    {
        auto topItem = ui->treeConfigFileData->topLevelItem(i);
        if (topItem->text(0).contains(target))
        {
            ui->treeConfigFileData->scrollToItem(topItem);
            topItem->setSelected(true);
            return;
        }

        for (int j=0;j<topItem->childCount(); j++)
        {
            auto childItem = topItem->child(j);
            if (childItem->text(0).contains(target))
            {
                ui->treeConfigFileData->expandItem(topItem);
                ui->treeConfigFileData->scrollToItem(childItem);
                childItem->setSelected(true);
                return;
            }
        }
    }

    QMessageBox msgBox(QMessageBox::Icon::Critical, "",
                       tr("Couldn't find ConVar '%0'.").arg(target), {}, this);
    msgBox.addButton("Ok", QMessageBox::ButtonRole::AcceptRole);
    msgBox.exec();
}

void ServerWindow::on_btnSaveConfig_clicked()
{
    QString fileName = ui->cmbConfigFile->currentText();
    QFile file (ServerFolder + "/Server/tf/cfg/" + fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        int topLevelCount = ui->treeConfigFileData->topLevelItemCount();
        QString output;
        if (!topLevelCount)
        {
            file.flush();
            file.close();

            if (QSystemTrayIcon::isSystemTrayAvailable())
            {
                QSystemTrayIcon icon;
                icon.show();
                icon.showMessage("No data saved", fileName, QIcon(":/icons/resources/Images/tf2dsm_logo.png"));
                icon.hide();
            }

            return;
        }
        if (fileName.length() < 8)
        {
            for (int i=0; i<topLevelCount; i++)
            {
                QTreeWidgetItem *topItem = ui->treeConfigFileData->topLevelItem(i);

                QString comment;
                if (!topItem->toolTip(0).isEmpty())
                {
                    QString string = topItem->toolTip(0);
                    QTextStream in(&string);
                    while (!in.atEnd())
                        comment += "//" + in.readLine() + "\n";
                }

                output += comment + (topItem->text(0) == "//" ? "" : topItem->text(0)) + (!topItem->text(1).isEmpty() ? " " + topItem->text(1) : "") + "\n";

                for (int j=0; j<topItem->childCount(); j++)
                {
                    QTreeWidgetItem *childItem = topItem->child(j);

                    comment.clear();
                    if (!childItem->toolTip(0).isEmpty())
                    {
                        //comment = "\n";
                        QString string = childItem->toolTip(0);
                        QTextStream in(&string);
                        while (!in.atEnd())
                            comment += "//" + in.readLine() + "\n";
                    }

                    output += comment + (childItem->text(0) == "//" ? "" : childItem->text(0)) + " " + childItem->text(1) + "\n" + (!comment.isEmpty() ? "\n" : "");
                }
            }
        }
        else if (fileName.first(4) == "motd")
            output = ui->txtConfigFileData->toPlainText().toStdString().c_str();
        else if (fileName.first(8) == "mapcycle")
        {
            int topLevelCount = ui->treeConfigFileData->topLevelItemCount();
            output.clear();
            if (!topLevelCount)
            {
                file.flush();
                file.close();

                if (QSystemTrayIcon::isSystemTrayAvailable())
                {
                    QSystemTrayIcon icon;
                    icon.show();
                    icon.showMessage("No data saved", fileName, QIcon(":/icons/resources/Images/tf2dsm_logo.png"));
                    icon.hide();
                }

                return;
            }
            for (int i=0; i<topLevelCount; i++)
            {
                QTreeWidgetItem *topItem = ui->treeConfigFileData->topLevelItem(i);

                QString comment;
                if (!topItem->toolTip(0).isEmpty())
                {
                    QString string = topItem->toolTip(0);
                    QTextStream in(&string);
                    while (!in.atEnd())
                        comment += "//" + in.readLine() + "\n";
                }

                output += (i > 0 ? "\n" : "") + comment + (topItem->text(0) == "//" ? "" : topItem->text(0));
            }
        }
        else
        {
            for (int i=0; i<topLevelCount; i++)
            {
                QTreeWidgetItem *topItem = ui->treeConfigFileData->topLevelItem(i);

                QString comment;
                if (!topItem->toolTip(0).isEmpty())
                {
                    QString string = topItem->toolTip(0);
                    QTextStream in(&string);
                    while (!in.atEnd())
                        comment += "//" + in.readLine() + "\n";
                }

                output += comment + (topItem->text(0) == "//" ? "" : topItem->text(0)) + (!topItem->text(1).isEmpty() ? " " + topItem->text(1) : "") + "\n";

                for (int j=0; j<topItem->childCount(); j++)
                {
                    QTreeWidgetItem *childItem = topItem->child(j);

                    comment.clear();
                    if (!childItem->toolTip(0).isEmpty())
                    {
                        //comment = "\n";
                        QString string = childItem->toolTip(0);
                        QTextStream in(&string);
                        while (!in.atEnd())
                            comment += "//" + in.readLine() + "\n";
                    }

                    output += comment + (childItem->text(0) == "//" ? "" : childItem->text(0)) + " " + childItem->text(1) + "\n" + (!comment.isEmpty() ? "\n" : "");
                }
            }
        }
        file.write(output.toStdString().c_str());

        file.flush();
        file.close();

        if (QSystemTrayIcon::isSystemTrayAvailable())
        {
            QSystemTrayIcon icon;
            icon.show();
            icon.showMessage("Saved config", fileName, QIcon(":/icons/resources/Images/tf2dsm_logo.png"));
            icon.hide();
        }
    }
}

void ServerWindow::on_cmbConfigFile_currentTextChanged(const QString &arg1)
{
    ui->treeConfigFileData->clear();
    ui->txtConfigFileData->clear();

    QFile file(ServerFolder + "/Server/tf/cfg/" + arg1);
    if (file.open(QIODevice::ReadOnly))
    {
        if (arg1.length() < 8)
        {
            ui->treeConfigFileData->show();
            ui->txtConfigFileData->hide();

            ui->treeConfigFileData->setColumnCount(2);
            ui->treeConfigFileData->setHeaderLabels(QStringList() << "ConVar" << "Value");
            ui->treeConfigFileData->setColumnWidth(0, 200);

            ui->btnAddConVar->setEnabled(true);
            ui->btnDelConVar->setEnabled(true);
            ui->btnConfigSpecial->setEnabled(false);
            ui->btnConfigSpecial->hide();
        }
        else if (arg1.first(4) == "motd")
        {
            ui->treeConfigFileData->hide();
            ui->txtConfigFileData->show();

            ui->btnAddConVar->setEnabled(false);
            ui->btnDelConVar->setEnabled(false);
            ui->btnConfigSpecial->setEnabled(false);
            ui->btnConfigSpecial->hide();

            ui->txtConfigFileData->appendPlainText(file.readAll());
            file.close();
            ui->txtConfigFileData->verticalScrollBar()->setSliderPosition(ui->txtConfigFileData->verticalScrollBar()->minimum());
            ui->txtConfigFileData->horizontalScrollBar()->setSliderPosition(ui->txtConfigFileData->horizontalScrollBar()->minimum());
            return;
        }
        else if (arg1.first(8) == "mapcycle")
        {
            ui->treeConfigFileData->show();
            ui->txtConfigFileData->hide();

            ui->treeConfigFileData->setColumnCount(1);
            ui->treeConfigFileData->setHeaderLabel("Map");

            ui->btnAddConVar->setEnabled(true);
            ui->btnDelConVar->setEnabled(true);
            ui->btnConfigSpecial->setEnabled(true);
            ui->btnConfigSpecial->show();

            ui->btnConfigSpecial->setText("Load Maps");
            ui->btnConfigSpecial->setToolTip("Load all .bsp files from the maps folder.");
        }
        else
        {
            ui->treeConfigFileData->show();
            ui->txtConfigFileData->hide();

            ui->treeConfigFileData->setColumnCount(2);
            ui->treeConfigFileData->setHeaderLabels(QStringList() << "ConVar" << "Value");
            ui->treeConfigFileData->setColumnWidth(0, 200);

            ui->btnAddConVar->setEnabled(true);
            ui->btnDelConVar->setEnabled(true);
            ui->btnConfigSpecial->setEnabled(false);
            ui->btnConfigSpecial->hide();
        }

        int parentItemIndex = -1;
        QStringList conVarCommentLines;

        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            if (line.isEmpty())
            {
                if (!conVarCommentLines.isEmpty())
                {
                    QString toolTip;
                    for (QString comment : conVarCommentLines)
                        toolTip += comment;

                    AddConfigTreeItem("//", "", toolTip, (parentItemIndex >= 0 ? ui->treeConfigFileData->topLevelItem(parentItemIndex) : nullptr));
                }
                conVarCommentLines.clear();
            }
            if (line.length() < 2)
                continue;
            else if (line.first(2) == "//" && line.last(2) == "//" && line.length() > 2)
            {
                QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeConfigFileData);
                item->setText(0, line);

                item->setFlags(item->flags() | Qt::ItemIsEditable);
                ui->treeConfigFileData->addTopLevelItem(item);

                parentItemIndex++;
                ui->treeConfigFileData->setIndentation(20);
            }
            else if (line.first(2) == "//")
            {
                conVarCommentLines << (conVarCommentLines.count() > 0 ? "\n" : "") << line.last(line.length()-2);
            }
            else// if (line.first(2) != "//")
            {
                QString conVar;
                int charCount = 0;
                for (QChar c : line)
                {
                    charCount++;
                    if (c == ' ')
                        break;
                    conVar += c;
                }

                QString toolTip;
                for (QString comment : conVarCommentLines)
                    toolTip += comment;

                AddConfigTreeItem(conVar, line.last(line.length()-charCount), toolTip, (parentItemIndex >= 0 ? ui->treeConfigFileData->topLevelItem(parentItemIndex) : nullptr));

                conVarCommentLines.clear();
            }
        }
        file.close();

        if (parentItemIndex < 0)
            ui->treeConfigFileData->setIndentation(0);
    }
}

void ServerWindow::AddConfigTreeItem(QString ConVar, QString Value, QString Comment, QTreeWidgetItem* parent)
{
    if (parent == nullptr)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeConfigFileData);
        item->setText(0, ConVar);
        item->setText(1, Value);
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        if (!Comment.isEmpty())
        {
            item->setToolTip(0, Comment);
            item->setToolTip(1, Comment);
        }

        ui->treeConfigFileData->addTopLevelItem(item);
    }
    else
    {
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, ConVar);
        item->setText(1, Value);
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        if (!Comment.isEmpty())
        {
            item->setToolTip(0, Comment);
            item->setToolTip(1, Comment);
        }

        parent->addChild(item);
    }
}

QString ServerWindow::ServerCfgExample()
{
    return QString("// General Settings //\n"
           "\n"
           "// Hostname for server.\n"
           "hostname %0\n"
           "\n"
           "// Overrides the max players reported to prospective clients\n"
           "sv_visiblemaxplayers 32\n"
           "\n"
           "// Maximum number of rounds to play before server changes maps\n"
           "mp_maxrounds 5\n"
           "\n"
           "// Set to lock per-frame time elapse\n"
           "host_framerate 0\n"
           "\n"
           "// Set the pause state of the server\n"
           "setpause 0\n"
           "\n"
           "// Control where the client gets content from\n"
           "// 0 = anywhere, 1 = anywhere listed in white list, 2 = steam official content only\n"
           "sv_pure 0\n"
           "\n"
           "// Is the server pausable\n"
           "sv_pausable 0\n"
           "\n"
           "// Type of server 0=internet 1=lan\n"
           "sv_lan 0\n"
           "\n"
           "// Collect CPU usage stats\n"
           "sv_stats 1\n"
           "\n"
           "\n"
           "\n"
           "// Execute Banned Users //\n"
           "exec banned_user.cfg\n"
           "exec banned_ip.cfg\n"
           "writeid\n"
           "writeip\n"
           "\n"
           "\n"
           "\n"
           "// Contact & Region //\n"
           "\n"
           "// Contact email for server sysop\n"
           "sv_contact emailaddy@google.com\n"
           "\n"
           "// The region of the world to report this server in.\n"
           "// -1 is the world, 0 is USA east coast, 1 is USA west coast\n"
           "// 2 south america, 3 europe, 4 asia, 5 australia, 6 middle east, 7 africa\n"
           "sv_region -1\n"
           "\n"
           "\n"
           "\n"
           "// Rcon Settings //\n"
           "\n"
           "// Password for rcon authentication (Remote CONtrol)\n"
           "rcon_password yourpw\n"
           "\n"
           "// Number of minutes to ban users who fail rcon authentication\n"
           "sv_rcon_banpenalty 1440\n"
           "\n"
           "// Max number of times a user can fail rcon authentication before being banned\n"
           "sv_rcon_maxfailures 5\n"
           "\n"
           "\n"
           "\n"
           "// Log Settings //\n"
           "\n"
           "// Enables logging to file, console, and udp < on | off >.\n"
           "log on\n"
           "\n"
           "// Log server information to only one file.\n"
           "sv_log_onefile 0\n"
           "\n"
           "// Log server information in the log file.\n"
           "sv_logfile 1\n"
           "\n"
           "// Log server bans in the server logs.\n"
           "sv_logbans 1\n"
           "\n"
           "// Echo log information to the console.\n"
           "sv_logecho 1\n"
           "\n"
           "\n"
           "\n"
           "// Rate Settings //\n"
           "\n"
           "// Frame rate limiter\n"
           "fps_max 600\n"
           "\n"
           "// Min bandwidth rate allowed on server, 0 == unlimited\n"
           "sv_minrate 0\n"
           "\n"
           "// Max bandwidth rate allowed on server, 0 == unlimited\n"
           "sv_maxrate 20000\n"
           "\n"
           "// Minimum updates per second that the server will allow\n"
           "sv_minupdaterate 10\n"
           "\n"
           "// Maximum updates per second that the server will allow\n"
           "sv_maxupdaterate 66\n"
           "\n"
           "\n"
           "\n"
           "// Download Settings //\n"
           "\n"
           "// Allow clients to upload customizations files\n"
           "sv_allowupload 1\n"
           "\n"
           "// Allow clients to download files\n"
           "sv_allowdownload 1\n"
           "\n"
           "// Maximum allowed file size for uploading in MB\n"
           "net_maxfilesize 15\n"
           "\n"
           "\n"
           "\n"
           "// Team Balancing //\n"
           "\n"
           "// Enable team balancing\n"
           "mp_autoteambalance 1\n"
           "\n"
           "// Time after the teams become unbalanced to attempt to switch players.\n"
           "mp_autoteambalance_delay 60\n"
           "\n"
           "// Time after the teams become unbalanced to print a balance warning\n"
           "mp_autoteambalance_warning_delay 30\n"
           "\n"
           "// Teams are unbalanced when one team has this many more players than the other team. (0 disables check)\n"
           "mp_teams_unbalance_limit 1\n"
           "\n"
           "\n"
           "\n"
           "// Round and Game Times //\n"
           "\n"
           "// Enable timers to wait between rounds. WARNING: Setting this to 0 has been known to cause a bug with setup times lasting 5:20 (5 minutes 20 seconds) on some servers!\n"
           "mp_enableroundwaittime 1\n"
           "\n"
           "// Time after round win until round restarts\n"
           "mp_bonusroundtime 8\n"
           "\n"
           "// If non-zero, the current round will restart in the specified number of seconds\n"
           "mp_restartround 0\n"
           "\n"
           "// Enable sudden death\n"
           "mp_stalemate_enable 1\n"
           "\n"
           "// Timelimit (in seconds) of the stalemate round.\n"
           "mp_stalemate_timelimit 300\n"
           "\n"
           "// Game time per map in minutes\n"
           "mp_timelimit 35\n"
           "\n"
           "\n"
           "\n"
           "// Client CVars //\n"
           "\n"
           "// Restricts spectator modes for dead players\n"
           "mp_forcecamera 0\n"
           "\n"
           "// Toggles whether the server allows spectator mode or not\n"
           "mp_allowspectators 1\n"
           "\n"
           "// Toggles footstep sounds\n"
           "mp_footsteps 1\n"
           "\n"
           "// Toggles game cheats\n"
           "sv_cheats 0\n"
           "\n"
           "// Time it takes for players to auto-disconnect if your server stops responding.\n"
           "sv_timeout 60\n"
           "\n"
           "// Maximum time a player is allowed to be idle, in minutes.\n"
           "mp_idlemaxtime 5\n"
           "\n"
           "// Deals with idle players 1=send to spectator 2=kick\n"
           "mp_idledealmethod 2\n"
           "\n"
           "// Time (seconds) between decal sprays\n"
           "decalfrequency 30\n"
           "\n"
           "\n"
           "\n"
           "// Communications //\n"
           "\n"
           "// enable voice communications\n"
           "sv_voiceenable 1\n"
           "\n"
           "// Players can hear all other players, no team restrictions 0=off 1=on\n"
           "sv_alltalk 0\n"
           "\n"
           "// Amount of time players can chat after the game is over\n"
           "mp_chattime 10\n"
           "\n"
           "// Enable party mode\n"
           "tf_birthday 0\n").arg(ui->lineServerName->text());
}

