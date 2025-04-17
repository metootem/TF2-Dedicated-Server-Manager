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

    IniSettings = new QSettings(tr("%0/server.ini").arg(directory), QSettings::Format::IniFormat);

    if (!directory.isEmpty())
        LoadServerConfig(QDir(directory));
    else
        LoadServerFirstTimeSetup();
    OS = QSysInfo::productType();
    if (OS != "windows" && OS != "macos")
        OS = "linux";

    ServerProcess = nullptr;
    SteamCMDWindow = nullptr;

    PublicIP = GetPublicIP();

    ui->btnInstallServer->setText((SteamCMDExists() ? "Update" : "Install"));
    ui->btnStartServer->setEnabled(SRCDSExists());
}

ServerWindow::~ServerWindow()
{
    delete ui;
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

    //ui->lineParameters->setText(IniSettings.value("parameters").toString());
    //AdditionalParametersWindow->LoadParameters(IniSettings);
    AdditionalParametersWindow = new AdditionalParametersDialog(this, IniSettings);
}


void ServerWindow::LoadServerFirstTimeSetup()
{
    AdditionalParametersWindow->FirstTimeSetup();
}


QString ServerWindow::GetPublicIP()
{
    QProcess GetIP;
    GetIP.start("curl", QStringList() << "https://api.ipify.org");
    GetIP.waitForFinished();
    QString IP = GetIP.readAllStandardOutput();
    GetIP.terminate();
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

void ServerWindow::InstallSteamCMD()
{
    QString SteamCMDPath = ServerFolder + "/SteamCMD";
    if (OS == "linux")
    {
        QMessageBox msgBox(QMessageBox::Icon::Question, "",
                           tr("Make sure you have the requirements:\n"
                              "https://wiki.teamfortress.com/wiki/Linux_dedicated_server#Requirements"), {}, this);
        auto *accept = msgBox.addButton("Download", QMessageBox::ButtonRole::AcceptRole);
        msgBox.addButton("Cancel", QMessageBox::ButtonRole::RejectRole);
        msgBox.exec();
        if (msgBox.clickedButton() != accept)
            return;

        ServerInstalling = true;

        QProcess::execute("mkdir", QStringList() << SteamCMDPath);


        QProcess::execute("wget", QStringList() << "-P" << SteamCMDPath << "https://steamcdn-a.akamaihd.net/client/installer/steamcmd_linux.tar.gz" );
        QProcess unpack;
        unpack.setWorkingDirectory(SteamCMDPath);
        unpack.start("tar", QStringList() << "zxf" << "steamcmd_linux.tar.gz");
        unpack.waitForFinished();

        //QProcess::execute("tar", QStringList() << "zxf" << SteamCMDPath +  "/steamcmd_linux.tar.gz");

        if (QSysInfo::productType() == "nobara" || QSysInfo::productType() == "fedora")
        {
            QProcess::execute("mkdir", QStringList() << "-p" << "~/.steam/sdk32");

            QProcess::execute("ln", QStringList() << "-s" << SteamCMDPath + "/linux32/steamclient.so" << "~/.steam/sdk32");

            QProcess::execute("ln", QStringList() << "-s" << "/usr/lib/libcurl.so.4" << "/usr/lib/libcurl-gnutls.so.4");
        }

        InstallServer();

    }
}

void ServerWindow::InstallServer()
{
    if (!QFile(ServerFolder+"/SteamCMD/steamcmd.sh").exists() && OS == "linux")
        return;
    if (!QFile(ServerFolder+"/SteamCMD/steamcmd.exe").exists() && OS == "windows")
        return;

    auto process = new QProcess(this);
    process->setWorkingDirectory(ServerFolder + "/SteamCMD");
    process->setProcessChannelMode(QProcess::MergedChannels);

    if (SteamCMDWindow == nullptr)
        SteamCMDWindow = new SteamCMDDialog(this, process, QDir(ServerFolder).dirName());
    else
        SteamCMDWindow->NewProcess(process);

    if (SteamCMDWindow->isHidden())
        SteamCMDWindow->show();

    ui->btnSteamCMDConsole->setEnabled(true);
    ui->btnInstallServer->setEnabled(false);
    ui->btnInstallServer->setText("Installing...");

    QProcess::execute("mkdir", QStringList() << ServerFolder + "/Server");
/*
    QMessageBox msgBox(QMessageBox::Icon::Question, "",
                       tr("Opt into beta?"), {}, this);
    auto *accept = msgBox.addButton("Yes", QMessageBox::ButtonRole::AcceptRole);
    msgBox.addButton("No", QMessageBox::ButtonRole::RejectRole);
    msgBox.exec();
    if (msgBox.clickedButton() == accept)
        return;
*/
    if (OS == "linux")
    {
        process->start("./steamcmd.sh", QStringList() << "+force_install_dir" << ServerFolder + "/Server" << "+login" << "anonymous" << "+app_update" << "232250" << "+quit");
    }
    else if (OS == "windows")
        process->start("./steamcmd.exe", QStringList() << "+force_install_dir" << ServerFolder + "/Server" << "+login" << "anonymous" << "+app_update" << "232250" << "+quit");

    process->waitForStarted();
    connect(process, SIGNAL(finished(int,QProcess::ExitStatus)), SLOT(InstallServerFinished()));
}

void ServerWindow::InstallServerFinished()
{
    ui->btnInstallServer->setEnabled(true);
    ui->btnInstallServer->setText((SteamCMDExists() ? "Update" : "Install"));
    if (SRCDSExists())
        ui->btnStartServer->setEnabled(true);
    ServerInstalling = false;
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


void ServerWindow::on_btnInstallServer_clicked()
{
    if (!SteamCMDExists())
    {
        QMessageBox msgBox(QMessageBox::Icon::Question, "",
                           tr("SteamCMD not found.\nDo you want to download automatically?"), {}, this);
        auto *accept = msgBox.addButton("Accept", QMessageBox::ButtonRole::AcceptRole);
        msgBox.addButton("Cancel", QMessageBox::ButtonRole::RejectRole);
        msgBox.exec();
        if (msgBox.clickedButton() == accept)
            InstallSteamCMD();
    }
    else
        InstallServer();
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
        ServerFolder = tr("%0/%1").arg(settings.ServerDirectory.path()).arg(ui->lineFolderName->text(), 1);

        IniSettings->setValue("server_name", ui->lineServerName->text());
        IniSettings->setValue("ip", ui->lineIP->text());
        IniSettings->setValue("port", ui->linePort->text());
        IniSettings->setValue("players", ui->spinMaxPlayers->value());
        IniSettings->setValue("map", ui->lineMap->text());
        IniSettings->setValue("parameters", AdditionalParametersWindow->GetParameters());
        //IniSettings->setValue("os", OS);

        if (QSystemTrayIcon::isSystemTrayAvailable())
        {
            QSystemTrayIcon icon;
            icon.show();
            icon.showMessage(ui->lineServerName->text(), "Settings applied");
            icon.hide();
        }

        emit ServerApplied( ServerFolder );
    }
}

void ServerWindow::on_btnParameters_clicked()
{
    AdditionalParametersWindow->show();
}


void ServerWindow::on_btnStartServer_clicked()
{
    QString Command;
    if (OS == "linux")
        Command = tr("%0/Server/srcds_run").arg(ServerFolder);
    else
        Command = tr("%0/Server/srcds.exe").arg(ServerFolder);

    QStringList args = {"-game tf"};

    args << tr("+ip %0").arg(ui->lineIP->text());
    args << tr("-port %0").arg(ui->linePort->text());

    if (ui->lineMap->text().isEmpty())
        args << "+randommap";
    else
        args << tr("+map %0").arg(ui->lineMap->text());

    if (!ui->lineServerName->text().isEmpty())
        args << "+hostname \"" + ui->lineServerName->text() + "\"";

    QStringList additionalParams = AdditionalParametersWindow->GetParameters();
    for (int i = 2; i < additionalParams.count(); i+=3)
    {
        if (additionalParams[i] != "True")
            continue;
        args << additionalParams[i-2];
        if (!additionalParams[i-1].isEmpty())
            args << additionalParams[i-1];
    }

    qInfo() << Command;
    qInfo() << args;

    auto Process = new QProcess(this);

    ServerProcess = Process;

    Process->setProcessChannelMode(QProcess::MergedChannels);
    Process->setWorkingDirectory(tr("%0/Server").arg(ServerFolder));

    //QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    //env.insert("LD_LIBRARY_PATH", ".:bin:" + env.value("LD_LIBRARY_PATH"));
    //Process->setProcessEnvironment(env);
    //qInfo() << env.value("LD_LIBRARY_PATH");

    if (ui->chkConsole->isChecked())
    {
        if (OS == "windows")
            Process->startDetached(Command, QStringList() << args << "-console");
        else
        {
            QStringList Terminals = {"gnome-terminal", "konsole", "xterm"};
            bool started = false;
            for (QString term : Terminals)
            {
                if (Process->startDetached(term, QStringList() << "-e" << Command << args))
                {
                    qInfo() << term;
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

    Process->start(Command, args, QProcess::ReadWrite | QProcess::Text);

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
        SetServerVisualState(VisualState::ServerFinished);
    }

}


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
    SetServerVisualState(VisualState::ServerFinished);
}


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
    for (QHostAddress address: QNetworkInterface::allAddresses())
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
    auto *localIp = msgBox.addButton("Local IP", QMessageBox::ButtonRole::RejectRole);
    msgBox.exec();
    if (msgBox.clickedButton() == publicIp)
        QDesktopServices::openUrl(QUrl("steam://connect/" + PublicServerAddress));
    else if (msgBox.clickedButton() == localIp)
        QDesktopServices::openUrl(QUrl("steam://connect/" + LocalServerAddress));
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
        icon.showMessage("Copied Public IP to clipboard", IP);
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


void ServerWindow::SetServerVisualState(VisualState state)
{
    switch (state)
    {
    case ServerStarted:
    {
        ui->btnStopServer->setEnabled(true);
        //ui->btnConnectToServer->setEnabled(true);
        ui->btnShowConsole->setEnabled(true);

        ui->chkConsole->setEnabled(false);
        ui->btnStartServer->setEnabled(false);
        ui->btnApply->setEnabled(false);
        break;
    }
    case VisualState::ServerFinished: // ok
    {
        ui->btnStartServer->setEnabled(true);
        ui->btnApply->setEnabled(true);
        ui->chkConsole->setEnabled(true);

        //ui->btnConnectToServer->setEnabled(false);
        ui->btnShowConsole->setEnabled(false);
        ui->btnStopServer->setEnabled(false);
        break;
    }
    }
}

