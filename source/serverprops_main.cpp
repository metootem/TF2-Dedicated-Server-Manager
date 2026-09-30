#include "serverprops_main.h"
#include "ui_serverprops_main.h"

ServerProps_Main::ServerProps_Main(QWidget *parent, QString name, QString directory)
    : QWidget(parent)
    , ui(new Ui::ServerProps_Main)
{
    ui->setupUi(this);

    OS = QSysInfo::productType();
    if (OS != "windows" && OS != "macos")
    {
        OS = "linux";
    }

    ServerFolder = directory;

    ui->lineServerName->setText(name);
    ui->lblFolderError->hide();
    ui->lblTip->hide();
    ui->PropsMain_Advanced->hide();

    SteamCMDProcess = nullptr;
    SteamCMDWindow = nullptr;
}

ServerProps_Main::~ServerProps_Main()
{
    delete ui;
}

void ServerProps_Main::LoadStyles(const QString colorTheme, const QString fullStyle)
{
    ui->btnAdvancedDropDown->setStyleSheet(QString("QPushButton {"
                                                   "border: none;"
                                                   "background-color: rgba(0, 0, 0, 0); } "
                                                   "QPushButton::hover { background-color: %0; }"
                                                   "QPushButton::pressed { background-color: %1; }"
                                                   "QPushButton:disabled { color: #3b3b3b; }").arg(colorTheme, QColor(colorTheme).darker(130).name()));

    AdditionalParametersWindow->LoadStyle(fullStyle);
}

void ServerProps_Main::FirstTimeSetup()
{
    ui->btnGotoServerFolder->setEnabled(false);
    ui->btnInstallServer->setEnabled(false);
    ui->lblTip->show();

    AdditionalParametersWindow = new AdditionalParametersDialog(this);
    AdditionalParametersWindow->FirstTimeSetup();
}

void ServerProps_Main::LoadServerConfig(const QDir directory)
{
    ServerFolder = directory.path();
    QSettings IniSettings(ServerFolder + "/server.ini", QSettings::Format::IniFormat);

    ui->lineServerName->setText(IniSettings.value("server_name").toString());

    ui->lineFolderName->setText(QDir(ServerFolder).dirName());

    ui->lineIP->setText(IniSettings.value("ip").toString());

    ui->linePort->setText(IniSettings.value("port").toString());

    ui->spinMaxPlayers->setValue(IniSettings.value("players").toInt());

    ui->lineMap->setText(IniSettings.value("map").toString());

    AdditionalParametersWindow = new AdditionalParametersDialog(this, &IniSettings);

    ui->lineToken->setText(IniSettings.value("token").toString());

    ui->linePassword->setText(IniSettings.value("password").toString());
}

QString ServerProps_Main::FormatDirectory(const QString path)
{
    return QString("%0/%1").arg(ServerFolder).arg(path, 1);
}

void ServerProps_Main::SetName(const QString name)
{
    ui->lineServerName->setText(name);
}

void ServerProps_Main::SetIP(const QString IP)
{
    //ui->lineIP->setText(IP);
    PublicIP = IP;
}

bool ServerProps_Main::SteamCMDExists()
{
    if (OS == "windows")
    {

        QFile SteamCMD(FormatDirectory("SteamCMD/steamcmd.exe"));
        return SteamCMD.exists();
    }
    else if (OS == "linux")
    {
        QFile SteamCMD(FormatDirectory("SteamCMD/steamcmd.sh"));
        return SteamCMD.exists();
    }

    return false;
}

bool ServerProps_Main::SteamCMDZipExists()
{
    if (OS == "windows")
    {
        QFile SteamCMD(FormatDirectory("SteamCMD/steamcmd.zip"));
        return SteamCMD.exists();
    }
    else if (OS == "linux")
    {
        QFile SteamCMD(FormatDirectory("SteamCMD/steamcmd_linux.tar.gz"));
        return SteamCMD.exists();
    }

    return false;
}

bool ServerProps_Main::SRCDSExists()
{
    if (OS == "windows")
    {
        QFile Srcds(FormatDirectory("Server/srcds.exe"));
        return Srcds.exists();
    }
    else if (OS == "linux")
    {
        QFile Srcds(FormatDirectory("Server/srcds_run"));
        return Srcds.exists();
    }

    return false;
}

void ServerProps_Main::DownloadSteamCMD()
{
    SetServerVisualState(VisualState::ServerDownloading);

    ui->btnInstallServer->setText("Downloading...");

    if (!SteamCMDZipExists())
    {
        qInfo() << "Downloading SteamCMD";

        QString SteamCMDPath = FormatDirectory("SteamCMD");

        QDir dir(SteamCMDPath);
        if (!dir.exists())
        {
            dir.mkdir(SteamCMDPath);
        }

        QString SteamCMDUrl;
        if (OS == "windows")
        {
            SteamCMDUrl = "https://steamcdn-a.akamaihd.net/client/installer/steamcmd.zip";
        }
        else if (OS == "linux")
        {
            SteamCMDUrl = "https://steamcdn-a.akamaihd.net/client/installer/steamcmd_linux.tar.gz";
        }

        auto download = new FileDownloader(this);
        connect(download, SIGNAL(finished()), SLOT(InstallSteamCMD()));
        download->downloadFile(QUrl(SteamCMDUrl), QDir(SteamCMDPath));
    }
    else
    {
        InstallSteamCMD();
    }
}


void ServerProps_Main::InstallSteamCMD()
{
    SetServerVisualState(VisualState::ServerInstalling);
    QString SteamCMDPath = FormatDirectory("SteamCMD");

    QString SteamCMDFile;
    if (OS == "windows")
    {
        SteamCMDFile = "steamcmd.zip";
    }
    else if (OS == "linux")
    {
        SteamCMDFile = "steamcmd_linux.tar.gz";
    }

    if (OS == "linux")
    {
        QMessageBox msgBox(QMessageBox::Icon::Question, "",
                           tr("Make sure you have the requirements:\n"
                              "https://wiki.teamfortress.com/wiki/Linux_dedicated_server#Requirements"), {}, this);
        auto *accept = msgBox.addButton("Continue", QMessageBox::ButtonRole::AcceptRole);
        msgBox.addButton("Cancel", QMessageBox::ButtonRole::RejectRole);
        msgBox.exec();
        if (msgBox.clickedButton() != accept)
        {
            return;
        }
    }

    ServerInstalling = true;

    ui->btnInstallServer->setText("Unpacking...");
    qInfo() << "unpacking" << SteamCMDPath;
    QProcess unpack;
    unpack.setWorkingDirectory(SteamCMDPath);
    unpack.start("tar", QStringList() << "zxf" << SteamCMDFile);

    if (!unpack.waitForFinished())
    {
        qInfo() << "There was an error unpacking steamcmd: " << unpack.errorString();
        qInfo() << "Aborting installation.";
        QMessageBox msgBox(QMessageBox::Icon::Critical, "Error unpacking",
                           tr("There was an error unpacking steamcmd: %0.").arg(unpack.errorString()), {}, this);
        msgBox.exec();
        SetServerVisualState();
        return;
    }

    // Check if this is needed
    if (QSysInfo::productType() == "nobara" || QSysInfo::productType() == "fedora")
    {
        QProcess::execute("mkdir", QStringList() << "-p" << "~/.steam/sdk32");

        QProcess::execute("ln", QStringList() << "-s" << SteamCMDPath + "/linux32/steamclient.so" << "~/.steam/sdk32");

        QProcess::execute("ln", QStringList() << "-s" << "/usr/lib/libcurl.so.4" << "/usr/lib/libcurl-gnutls.so.4");
    }

    InstallServer();
}

void ServerProps_Main::InstallServer()
{
    qInfo() << "Installing server";
    SetServerVisualState(VisualState::ServerInstalling);

    if (SteamCMDProcess == nullptr)
    {
        SteamCMDProcess = new QProcess(this);
        SteamCMDProcess->setWorkingDirectory(FormatDirectory("SteamCMD"));
        SteamCMDProcess->setProcessChannelMode(QProcess::MergedChannels);
    }

    if (SteamCMDWindow == nullptr)
    {
        SteamCMDWindow = new SteamCMDDialog(this, SteamCMDProcess, ui->lineServerName->text());
    }
    else
    {
        SteamCMDWindow->NewProcess(SteamCMDProcess);
    }

    ui->btnInstallServer->setText("Installing...");

    QDir dir(ServerFolder + "/Server");
    if (!dir.exists())
    {
        dir.mkdir(ServerFolder + "/Server");
    }

    QStringList betaList;

    if (ui->chkBeta->isChecked())
    {
        qInfo() << "Opting into beta.";
        betaList << "-beta" << "prerelease";
    }
    betaList << "+quit";

    if (OS == "linux")
    {
        SteamCMDProcess->start("./steamcmd.sh", QStringList() << "+force_install_dir" << ServerFolder + "/Server" << "+login" << "anonymous" << "+app_update" << "232250" << betaList);
    }
    else if (OS == "windows")
    {
        SteamCMDProcess->start(QString("%0/SteamCMD/steamcmd.exe").arg(ServerFolder), QStringList() << "+force_install_dir" << ServerFolder + "/Server" << "+login" << "anonymous" << "+app_update" << "232250" << betaList);
    }

    if (SteamCMDProcess->waitForStarted())
    {
        if (SteamCMDWindow->isHidden())
        {
            SteamCMDWindow->show();
        }
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
            {
                InstallServer();
            }
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

void ServerProps_Main::InstallServerFinished()
{
    SteamCMDProcess->deleteLater();
    SteamCMDProcess = nullptr;
    SetServerVisualState();

    QSettings mainIniSettings("tf2-dsm_config.ini", QSettings::IniFormat);
    if (!mainIniSettings.value(QString("%0/portForwardTip").arg(OS), false).toBool())
    {
        QMessageBox msgBox(QMessageBox::Icon::Warning, "PortForwarding",
                           tr("If you want people outside your network to join the server,\n"
                              "make sure you have port forwarding set up.\n"
                              "You may also not be able to join through Public IP without it."), {}, this);
        msgBox.addButton("Ok", QMessageBox::ButtonRole::AcceptRole);
        msgBox.exec();
        mainIniSettings.setValue(QString("%0/portForwardTip").arg(OS), true);
    }

    emit SystemNotification("Finished Running SteamCMD", ui->lineServerName->text(), 3000);
    emit ServerInstalled();
}

QString ServerProps_Main::GetName()
{
    return ui->lineServerName->text();
}

QString ServerProps_Main::GetIP(bool copyToClipboard)
{
    QString IP;
    if (ui->lineIP->text() == "0.0.0.0" || ui->lineIP->text().isEmpty())
    {
        IP = PublicIP + ":" + ui->linePort->text();
    }
    else
    {
        IP = ui->lineIP->text() + ":" + ui->linePort->text();
    }

    if (copyToClipboard)
    {
        QClipboard *clip = QApplication::clipboard();
        clip->setText(IP);

        emit SystemNotification("Copied Public IP to clipboard", IP, 3000);
    }

    return IP;
}

void ServerProps_Main::StartServer()
{
    if (!SRCDSExists())
    {
        on_btnInstallServer_clicked();
        return;
    }

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

    if (!ui->lineToken->text().isEmpty())
        args << "+sv_setsteamaccount" << ui->lineToken->text();

    if (!ui->linePassword->text().isEmpty())
        args << "+sv_password" << "\"" + ui->linePassword->text() + "\"";

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

    Process->setProcessChannelMode(QProcess::MergedChannels);
    Process->setWorkingDirectory(QString("%0/Server").arg(ServerFolder));

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
            for (const QString &term : std::as_const(Terminals))
            {
                QString exec = (term == "gnome-terminal" ? "--" : "-e");
                if (Process->startDetached(term, QStringList() << exec << Command << args))
                {
                    qInfo() << "Found terminal: " + term;
                    started = true;
                    break;
                }
            }
            if (!started)
            {
                bool ok = true;
                QSettings iniSettings(ServerFolder+"/server.ini", QSettings::IniFormat);
                QString term = iniSettings.value(QString("%0/terminal").arg(QSysInfo::productType())).toString();
                QString exec = iniSettings.value(QString("%0/exec").arg(QSysInfo::productType())).toString();

                if (term.isEmpty())
                {
                    term = QInputDialog::getText(this, tr("Linux Terminal Not Found"),
                                                 tr("Your Terminal:"), QLineEdit::Normal,
                                                 QDir::home().dirName(), &ok);
                }
                if (!ok || term.isEmpty())
                {
                    return;
                }

                if (exec.isEmpty())
                {
                    exec = QInputDialog::getText(this, tr("Terminal execute command"),
                                                 tr("Execute command:"), QLineEdit::Normal,
                                                 QDir::home().dirName(), &ok);
                }

                if (!ok)
                {
                    return;
                }

                iniSettings.setValue(QString("%0/terminal").arg(QSysInfo::productType()), term);
                iniSettings.setValue(QString("%0/exec").arg(QSysInfo::productType()), exec);
                Process->startDetached(term, QStringList() << exec << Command << args);
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
        {
            ServerConsole->show();
        }

        SetServerVisualState(ServerStarted);

        connect(Process, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(ServerStateChanged(QProcess::ProcessState)));

        emit ServerActivated();
    }
    else
    {
        SetServerVisualState(VisualState::ServerStopped);
    }
}

void ServerProps_Main::JoinServer()
{
    QString LocalServerAddress;
    QHostAddress host(QHostAddress::LocalHost);
    QList<QHostAddress> addressList = QNetworkInterface::allAddresses();
    for (const QHostAddress &address : std::as_const(addressList))
    {
        if (address.protocol() == QAbstractSocket::IPv4Protocol &&
            address != host &&
            !address.isLoopback() && address.toString().right(2) != ".1") // dirty workaround
        {
            LocalServerAddress = address.toString() + ":" + ui->linePort->text();
            break; // main local IP should always be first, right?
        }
    }

    QString PublicServerAddress = PublicIP + ":" + ui->linePort->text();

    QMessageBox msgBox(QMessageBox::Icon::Question, "",
                       tr("Through which IP to join server?\nPublic: %0\nLocal: %1").arg(PublicServerAddress).arg(LocalServerAddress, 1), {}, this);

    auto *publicIp = msgBox.addButton("Public IP", QMessageBox::ButtonRole::AcceptRole);
    if (PublicServerAddress.isEmpty())
    {
        publicIp->setEnabled(false);
    }

    auto *localIp = msgBox.addButton("Local IP", QMessageBox::ButtonRole::AcceptRole);
    auto *customIp = msgBox.addButton("Custom IP", QMessageBox::ButtonRole::AcceptRole);
    msgBox.addButton("Cancel", QMessageBox::RejectRole);

    msgBox.exec();

    if (msgBox.clickedButton() == publicIp)
    {
        QDesktopServices::openUrl(QUrl("steam://connect/" + PublicServerAddress));
    }
    else if (msgBox.clickedButton() == localIp)
    {
        QDesktopServices::openUrl(QUrl("steam://connect/" + LocalServerAddress));
    }
    else if (msgBox.clickedButton() == customIp)
    {
        bool ok;
        QString input = QInputDialog::getText(this, tr("Type Custom IP and Port"),
                                              tr("IP and Port:"), QLineEdit::Normal, QString(), &ok);
        if (ok && !input.isEmpty())
        {
            QDesktopServices::openUrl(QUrl("steam://connect/" + input));
        }
    }
}


void ServerProps_Main::on_btnInstallServer_clicked()
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


void ServerProps_Main::on_btnApply_clicked()
{
    ui->btnGotoServerFolder->setEnabled(false);
    ui->lblTip->hide();
    ui->lblFolderError->hide();

    bool apply = true;

    if (ui->lineServerName->text().isEmpty())
    {
        ui->lineServerName->setText("Team Fortress 2 Server");
    }
    qInfo() << "Server Name:" << ui->lineServerName->text();
    if (ui->lineFolderName->text().isEmpty())
    {
        ui->lblFolderError->show();
        qInfo() << "Folder is invalid!";
        apply = false;
    }
    /*else if (ui->lineFolderName->text().contains(" "))
    {
        ui->lblFolderError->setText("Folder name can't have spaces!");
        ui->lblFolderError->show();
        qInfo() << "Folder name can't have spaces!";
        apply = false;
    }*/
    else if (ui->lineFolderName->text().contains(":"))
    {
        ui->lblFolderError->setText("Folder name can't be a directory!");
        ui->lblFolderError->show();
        qInfo() << "Folder name can't be a directory!";
        apply = false;
    }
    else
    {
        qInfo() << "Folder:" << ui->lineFolderName->text();
    }


    if (ui->lineIP->text().isEmpty())
    {
        ui->lineIP->setText("0.0.0.0");
    }
    qInfo() << "IP:" << ui->lineIP->text();

    if (ui->linePort->text().isEmpty())
    {
        ui->linePort->setText("27015");
    }
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

        QDir parentDir(ServerFolder);
        if (settings.ServerDirectories.contains(ServerFolder))
        {
            ServerFolder = QString("%0/%1").arg(parentDir.path(), ui->lineFolderName->text());
        }
        else if (QDir(ServerFolder).dirName() != ui->lineFolderName->text())
        {
            parentDir.cdUp();
            if (QFile::rename(ServerFolder, QString("%0/%1").arg(parentDir.path(), ui->lineFolderName->text())))
            {
                ServerFolder = QString("%0/%1").arg(parentDir.path(), ui->lineFolderName->text());
            }
            else
            {
                ui->lblFolderError->setText("Server Folder cannot be renamed currently!");
                ui->lblFolderError->show();
                ui->lineFolderName->setText(QDir(ServerFolder).dirName());
            }
        }

        if (!QDir(ServerFolder).exists())
        {
            QDir dir;
            dir.mkdir(ServerFolder);
            qInfo() << "Making server directory:" << ServerFolder;
        }

        QSettings IniSettings(ServerFolder + "/server.ini", QSettings::Format::IniFormat);

        qInfo() << ServerFolder;
        qInfo() << "Saving server config.";

        IniSettings.setValue("server_name", ui->lineServerName->text());
        qInfo() << "server_name...";

        IniSettings.setValue("ip", ui->lineIP->text());
        qInfo() << "ip...";

        IniSettings.setValue("port", ui->linePort->text());
        qInfo() << "port...";

        IniSettings.setValue("players", ui->spinMaxPlayers->value());
        qInfo() << "players...";

        IniSettings.setValue("map", ui->lineMap->text());
        qInfo() << "map...";

        IniSettings.setValue("parameters", AdditionalParametersWindow->GetParameters());
        qInfo() << "parameters...";

        IniSettings.setValue("token", ui->lineToken->text());
        qInfo() << "token...";

        IniSettings.setValue("password", ui->linePassword->text());
        qInfo() << "password...";

        IniSettings.setValue("os", OS);
        qInfo() << "os...";

        qInfo() << "Saved.";

        ui->btnGotoServerFolder->setEnabled(true);
        SetServerVisualState();

        emit ServerApplied( ServerFolder );
        emit SystemNotification(ui->lineServerName->text(), "Settings applied", 3000);
    }
}

void ServerProps_Main::on_btnStartServer_clicked()
{
    StartServer();
}

/*
void ServerProps_Main::on_btnStopServer_clicked()
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


void ServerProps_Main::on_btnShowConsole_clicked()
{
    if (ServerProcess->state() != QProcess::Running)
    {
        return;
    }
    if (ServerConsole->isHidden())
    {
        ServerConsole->show();
    }
    else
    {
        ServerConsole->setFocus();
    }
}

void ServerProps_Main::on_btnConnectToServer_clicked()
{
    JoinServer();
}

void ServerProps_Main::on_btnGotoServerFolder_clicked()
{
    if (!ui->lineFolderName->text().isEmpty())
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile(ServerFolder));
    }
}

void ServerProps_Main::on_btnCopyIp_clicked()
{
    GetIP(true);
}

void ServerProps_Main::on_btnSelectMap_clicked()
{
    auto dialog = new SelectMapDialog(this, ServerFolder);
    if (dialog->exec() == QDialog::Accepted)
    {
        if (dialog)
        {
            ui->lineMap->setText(dialog->SelectMap());
        }
    }
    delete dialog;
}

void ServerProps_Main::on_btnParameters_clicked()
{
    AdditionalParametersWindow->show();
}

void ServerProps_Main::on_btnSteamCMDConsole_clicked()
{
    if (SteamCMDWindow != nullptr)
    {
        SteamCMDWindow->show();
    }
}

void ServerProps_Main::on_btnAdvancedDropDown_clicked()
{
    if (ui->PropsMain_Advanced->isHidden())
    {
        ui->btnAdvancedDropDown->setIcon(QIcon(":/icons/resources/Icons/DropOpen.svg"));
        ui->PropsMain_Advanced->show();
    }
    else
    {
        ui->btnAdvancedDropDown->setIcon(QIcon(":/icons/resources/Icons/DropClosed.svg"));
        ui->PropsMain_Advanced->hide();
    }
}
void ServerProps_Main::SetServerVisualState(VisualState state)
{
    switch (state)
    {
    case VisualState::ServerDefault:
    {
        ui->btnInstallServer->setText((SteamCMDExists() ? "Update" : "Install"));
        if (SRCDSExists())
        {
            ui->btnStartServer->setEnabled(true);
            ui->btnConnectToServer->setEnabled(true);
        }
        else
        {
            ui->btnStartServer->setEnabled(false);
            ui->btnConnectToServer->setEnabled(false);
        }

        if (!ui->lineFolderName->text().isEmpty())
            ui->btnInstallServer->setEnabled(true);
        else
            ui->btnInstallServer->setEnabled(false);
        ui->btnApply->setEnabled(true);

        ui->btnShowConsole->setEnabled(false);
        ui->btnStopServer->setEnabled(false);
        break;
    }
    case VisualState::ServerStarted:
    {
        ui->btnStopServer->setEnabled(true);
        //ui->btnConnectToServer->setEnabled(true);
        ui->btnShowConsole->setEnabled(true);

        ui->btnStartServer->setEnabled(false);
        ui->btnApply->setEnabled(false);
    }
    case VisualState::ServerStopped:
    {
        ui->btnStartServer->setEnabled(true);
        ui->btnApply->setEnabled(true);

        //ui->btnConnectToServer->setEnabled(false);
        ui->btnShowConsole->setEnabled(false);
        ui->btnStopServer->setEnabled(false);
    }
    case VisualState::ServerDownloading:
    {
        ui->btnSteamCMDConsole->setEnabled(false);
        ui->btnInstallServer->setEnabled(false);
        ui->btnStartServer->setEnabled(false);
        ui->btnStopServer->setEnabled(false);
        ui->btnApply->setEnabled(false);
    }
    case VisualState::ServerInstalling:
    {
        ui->btnSteamCMDConsole->setEnabled(true);

        ui->btnInstallServer->setEnabled(false);
        ui->btnStartServer->setEnabled(false);
        ui->btnStopServer->setEnabled(false);
        ui->btnApply->setEnabled(false);
    }
    case VisualState::ServerFinishedInstalling:
    {
        ui->btnInstallServer->setEnabled(true);
        ui->btnInstallServer->setText((SteamCMDExists() ? "Update" : "Install"));
        if (SRCDSExists())
        {
            ui->btnStartServer->setEnabled(true);
        }
        else
        {
            ui->btnStartServer->setEnabled(false);
        }

        ServerInstalling = false;

        ui->btnApply->setEnabled(true);
    }
    }
}