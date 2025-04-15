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
    ServerProcess = NULL;
    PublicIP = GetPublicIP();
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

QString ServerWindow::GetPublicIP()
{
    QProcess GetIP;
    GetIP.start("curl", QStringList() << "https://api.ipify.org");
    GetIP.waitForFinished();
    QString IP = GetIP.readAllStandardOutput();
    GetIP.terminate();
    return IP;
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

    QString Command;
    if (OS == "linux")
        Command = tr("%0/Server/srcds_run").arg(ServerFolder);
    else
        Command = tr("%0/Server/srcds.exe").arg(ServerFolder);

    QStringList args = {"-game tf"};

    args << tr("+ip %0").arg(ui->lineIP->text());
    args << tr("-port %0").arg(ui->linePort->text());
    args << "+sv_pure 0";

    if (ui->lineMap->text().isEmpty())
        args << "+randommap";
    else
        args << tr("+map %0").arg(ui->lineMap->text());

    if (!ui->lineServerName->text().isEmpty())
        args << "+hostname \"" + ui->lineServerName->text() + "\"";

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

    if (ui->chkConsole->checkState())
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
            qInfo() << Process->processId();
        }
        return;
    }

    Process->start(Command, args, QProcess::ReadWrite | QProcess::Text);

    if (Process->waitForStarted())
    {
        qInfo() << "server running";
        auto ServerConsoleDial = new ServerConsoleDialog(this, Process, ui->lineServerName->text());
        ServerConsole = ServerConsoleDial;

        if (!ui->chkConsole->checkState())
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
        if (ui->chkConsole->checkState())
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

void ServerWindow::ServerStateChanged(QProcess::ProcessState state)
{
    qInfo() << state;
    qInfo() << ServerProcess->exitCode();
    qInfo() << ServerProcess->exitStatus();
    qInfo() << ServerProcess->processId();

    if (state == QProcess::NotRunning)
        SetServerVisualState(VisualState::ServerFinished);

}

void ServerWindow::ReadOutput()
{
    qInfo() << ServerProcess->state();
    qInfo() << ServerProcess->exitCode();
    qInfo() << ServerProcess->exitStatus();
    qInfo() << ServerProcess->processId();
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

