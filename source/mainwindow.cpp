#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    AppVersion = "v1.01";
    AppVersionDate = "September 2026";
    OS = QSysInfo::productType();
    if (OS != "windows" && OS != "macos")
        OS = "linux";

    IniSettings = new QSettings("tf2-dsm_config.ini", QSettings::Format::IniFormat);
    ui->lblVersion->setText(tr("%0 %1").arg(AppVersion, AppVersionDate));

    /*if (QSystemTrayIcon::isSystemTrayAvailable())
    {
        SystemTrayIcon = new QSystemTrayIcon(this);
        SystemTrayIcon->setIcon(QIcon(":/tf2dsm.ico"));
        SystemTrayIcon->setVisible(true);

        QMenu *menu = new QMenu();

        menu->addSeparator();

        QAction *quitAct = new QAction();
        quitAct->setText("Close");

        connect(quitAct, &QAction::triggered, []() {
            QApplication::quit();
        });
        menu->addAction(quitAct);
        SystemTrayIcon->setContextMenu(menu);

        connect(SystemTrayIcon, &QSystemTrayIcon::activated, this, &MainWindow::FocusWindow);
    }*/

    SysTrayHandler = new SystemTrayHandler(this);
    if (SysTrayHandler->Exists())
    {
        connect(SysTrayHandler, &SystemTrayHandler::Clicked, this, &MainWindow::FocusWindow);
        connect(SysTrayHandler, &SystemTrayHandler::Close, this, &MainWindow::CloseApp);
        connect(SysTrayHandler, &SystemTrayHandler::RequestServerNick, this, &MainWindow::sysTrayRequestServerNick);
        connect(this, &MainWindow::ServerNickChanged, SysTrayHandler, &SystemTrayHandler::ServerNickChanged);
    }

    PublicIP = GetPublicIP();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::FocusWindow()
{
    this->showNormal();
    this->raise();
    this->activateWindow();
    this->setFocus();
}

void MainWindow::CloseApp()
{
    QApplication::quit();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (!event->spontaneous())
        return;

    delete SysTrayHandler;
    event->accept();
}

QString MainWindow::GetPublicIP()
{
    QProcess GetIP;
    GetIP.start("curl", QStringList() << "https://api.ipify.org");
    GetIP.waitForFinished(5000);
    QString IP = GetIP.readAllStandardOutput();
    GetIP.terminate();
    if (IP.isEmpty())
    {
        qInfo() << "Couldn't get Public IP.";
    }
    else
    {
        qInfo() << "Got Public IP:" << IP;
    }
    return IP;
}

bool MainWindow::LoadConfig()
{
    qInfo() << "Loading app settings.";
    SettingsDialog* settingsDialog = new SettingsDialog(this);
    Settings = settingsDialog->ParseSettings();
    if (!Settings.valid)
    {
        settingsDialog->show();

        connect(settingsDialog, &SettingsDialog::SettingsChanged, this, &MainWindow::SettingsChanged);
        return false;
    }
    delete settingsDialog;

    Settings.PublicIP = PublicIP;
    ServerDirs = Settings.ServerDirectories;
    RefreshServerTab();

    LoadStyles(Settings.ColorTheme);

    return true;
}

void MainWindow::SettingsChanged( SettingsStruct settings )
{
    Settings = settings;
    Settings.PublicIP = PublicIP;
    ServerDirs = settings.ServerDirectories;
    RefreshServerTab();
    LoadStyles(settings.ColorTheme);

    ShowSystemNotification("Settings changed", "", 3000);

    emit PassSettingsChanged(Settings);
}

void MainWindow::LoadStyles(QString colorTheme)
{

    this->setStyleSheet(QString("QInputDialog { background-color: #2b2b2b; }"
                                "QMessageBox { background-color: #2b2b2b; }"
                                "QLabel { color: #ffffff; }"
                                "QToolTip { color: #ffffff; background-color: #2b2b2b; border: 1px solid #5e5e5e;}"
                                ""
                                "QTabBar::tab {"
                                "border: 0px solid;"
                                "background-color: #2b2b2b;"
                                "padding: 5px;"
                                "border-top-left-radius: 3px;"
                                "border-top-right-radius: 3px;"
                                "color: #ffffff; }"
                                ""
                                "QTabBar::tab:selected { background-color: %0; }"
                                ""
                                "QPushButton {"
                                "border: none;"
                                "border-bottom: 2px solid %0;"
                                "background-color: rgba(0, 0, 0, 0);"
                                "font: 13pt \"Noto Sans\";"
                                "color: #ffffff; }"
                                ""
                                "QCheckBox { background-color: #2b2b2b; }"
                                ""
                                "QPushButton::hover { background-color: %1; }"
                                "QPushButton::pressed { background-color: %2; }"
                                "QPushButton:disabled { color: #3b3b3b; }"
                                "QListWidget { border: none; selection-background-color: %0; }"
                                "QListWidget::item:selected { background-color: %0; }"
                                "QTreeWidget { selection-background-color: %0; }"
                                "QTreeWidget::item:selected { background-color: %0; }"
                                "QProgressBar { text-align: center; }"
                                "QProgressBar::chunk { background-color: %0; }"
                                ).arg(colorTheme, QColor(colorTheme).lighter(130).name(), QColor(colorTheme).darker(130).name()));

    ui->lblAddServer->setText("");
    if (!ui->tabServers->count())
    {
        ui->lblAddServer->setText("Add a new server!");
        ui->tabServers->setStyleSheet("QTabWidget::pane { border: none; background-color: #2b2b2b; }");
    }
    else
    {
        ui->tabServers->setStyleSheet(QString("QTabWidget::pane { border-bottom: 0px solid #5a5a5a;"
                                              "border-top: 2px solid %0;"
                                              "background-color: #2b2b2b; }").arg(colorTheme));
        for (int tabIndex=0; tabIndex < ui->tabServers->count(); tabIndex++)
        {
            ServerWindow *server = (ServerWindow*)ui->tabServers->widget(tabIndex);

            server->UpdateStyles(colorTheme, this->styleSheet());
        }
    }
}

// System Tray Handler
void MainWindow::ShowSystemNotification(QString title, QString message, int length)
{
    if (SysTrayHandler->Exists())
    {
        SysTrayHandler->ShowMessage(title, message, length);
    }
}

QString MainWindow::sysTrayRequestServerNick(ServerWindow* server)
{
    return GetServerNick(server);
}

void MainWindow::AddServerToSysTray(ServerWindow *server)
{
    if (SysTrayHandler->Exists())
    {
        SysTrayHandler->AddServerToSysTray(server);
    }
}

void MainWindow::RemoveServerFromSysTray(ServerWindow *server)
{
    if (SysTrayHandler->Exists())
    {
        SysTrayHandler->RemoveServerFromSysTray(server);
    }
}

// Servers
QString MainWindow::GetServerNick(ServerWindow* target)
{
    int count = ui->tabServers->count();
    for (int srvIndex=0; srvIndex < count; srvIndex++)
    {
        ServerWindow *server = (ServerWindow*)ui->tabServers->widget(srvIndex);
        if (server == target)
        {
            return ui->tabServers->tabText(srvIndex);
        }
    }
    return "";
}

bool MainWindow::ServerTabExists(QString name)
{
    for (int i=0; i<ui->tabServers->count(); i++)
    {
        if (ui->tabServers->tabText(i) == name)
        {
            return true;
        }
    }
    return false;
}

void MainWindow::FocusServer(ServerWindow *server)
{
    qInfo() << server->GetServerName();
}

void MainWindow::AddServer(QString servername, QString serverFolder)
{
    QString name = servername;
    int found_count = 0;
    while (ServerTabExists(name))
    {
        found_count++;
        if (found_count == 1)
        {
            name = tr("%0 %1").arg(servername).arg(found_count, 1);
        }
    }

    qInfo() << " ";
    qInfo() << "Adding Server Tab: " + name;
    ServerWindow *newServerWindow = new ServerWindow(Settings, this, name, serverFolder);
    int index = ui->tabServers->addTab(newServerWindow, name);

    connect(this, SIGNAL(PassSettingsChanged(SettingsStruct)), newServerWindow, SLOT(SettingsChanged(SettingsStruct)));

    connect(newServerWindow, SIGNAL(PassServerApplied(QString)), this, SLOT(ServerApplied(QString)));
    connect(newServerWindow, SIGNAL(PassServerActivated()), this, SLOT(ServerActivated()));
    connect(newServerWindow, SIGNAL(PassServerDeactivated()), this, SLOT(ServerDeactivated()));
    connect(newServerWindow, SIGNAL(PassSystemNotification(QString,QString,int)), SLOT(ShowSystemNotification(QString,QString,int)));

    if (serverFolder.isEmpty())
    {
        ui->tabServers->setTabIcon(index, QIcon(":/icons/resources/Icons/Add.svg"));
    }
    else
    {
        ui->tabServers->setTabIcon(index, QIcon(":/icons/resources/Icons/ServerInactive.svg"));
    }

    LoadStyles(Settings.ColorTheme);

    if (QSystemTrayIcon::isSystemTrayAvailable())
    {
        AddServerToSysTray(newServerWindow);
    }
}

void MainWindow::RemoveServer(int index, bool removeFiles)
{
    ServerWindow *server = (ServerWindow*)ui->tabServers->widget(index);

    if (QSystemTrayIcon::isSystemTrayAvailable())
    {
        RemoveServerFromSysTray(server);
    }

    QString path = server->ServerFolder;
    QDir dir(path);
    if (removeFiles)
    {
        if (server->ServerDirectoryExists())
        {
            dir.removeRecursively();
        }

        IniSettings->remove(dir.dirName());
    }
    else
    {
        QFile serverFile(QString("%0/server.ini").arg(path));
        serverFile.remove();
    }
    IniSettings->remove(dir.dirName());
    ui->tabServers->removeTab(index);

    if (!ui->tabServers->count())
    {
        ui->lblAddServer->setText("Add a new server!");
    }
}

void MainWindow::ServerApplied(QString ServerFolder)
{
    QString folder = QDir(ServerFolder).dirName();
    int index = ui->tabServers->currentIndex();

    ui->tabServers->setTabIcon(index, QIcon(":/icons/resources/Icons/ServerInactive.svg"));

    IniSettings->setValue(QString("%0/nick").arg(folder), ui->tabServers->tabText(index));
    IniSettings->setValue(QString("%0/os").arg(folder), OS);
}

void MainWindow::ServerActivated()
{
    int index = ui->tabServers->currentIndex();
    ui->tabServers->setTabIcon(index, QIcon(":/icons/resources/Icons/ServerActive.svg"));
}

void MainWindow::ServerDeactivated()
{
    int index = ui->tabServers->currentIndex();
    ui->tabServers->setTabIcon(index, QIcon(":/icons/resources/Icons/ServerInactive.svg"));
}

void MainWindow::RefreshServerTab()
{
    for (int i=0; i<ui->tabServers->count(); i++) // Remove server tabs
    {
        auto server = ((ServerWindow*)ui->tabServers->widget(i));

        if (QSystemTrayIcon::isSystemTrayAvailable())
        {
            RemoveServerFromSysTray(server);
        }

        if (!server->ServerDirectoryExists())
        {
            continue;
        }
        qInfo() << "Removed server tab" << ui->tabServers->tabText(i);
        ui->tabServers->removeTab(i--);
    }

    for (const QString &srvDir : std::as_const(ServerDirs)) // Find new servers
    {
        qInfo() << "Searching in" << srvDir;
        QDir ServerDir(srvDir);
        QFileInfoList fileList = ServerDir.entryInfoList(QDir::Filter::Dirs);
        for (const QFileInfo &file : std::as_const(fileList))
        {
            QString iniPath = QString("%0/server.ini").arg(file.absoluteFilePath());
            if (!QFile(iniPath).exists())
            {
                continue;
            }

            QString folderName = file.fileName();
            QSettings fileIniSettings(iniPath, QSettings::IniFormat);
            if (!IniSettings->contains(QString("%0/nick").arg(folderName)) && fileIniSettings.value("os").toString() == OS)
            {
                IniSettings->setValue(QString("%0/nick").arg(folderName), folderName);
                IniSettings->setValue(QString("%0/os").arg(folderName), OS);
            }

            QString serverNick = IniSettings->value(QString("%0/nick").arg(folderName)).toString();
            if (ServerTabExists(serverNick) || fileIniSettings.value("os").toString() != OS)
            {
                continue;
            }
            if (serverNick.isEmpty())
            {
                serverNick = folderName;
            }
            AddServer(serverNick, file.filePath());
            ui->tabServers->setCurrentIndex(ui->tabServers->count()-1);
        }
    }

    QStringList childGroups = IniSettings->childGroups();

    for (const QString &server : std::as_const(childGroups)) // Remove unused servers from config
    {
        if (server == OS)
        {
            continue;
        }
        bool remove = true;
        for (const QString &parentDir : std::as_const(ServerDirs))
        {
            QString con = QString("%0/%1").arg(parentDir, server);
            if (QDir(con).exists() && server != ".")
            {
                remove = false;
            }
        }
        if (remove)
        {
            IniSettings->remove(server);
        }
    }
}


void MainWindow::on_btnSettings_clicked()
{
    SettingsDialog* settingsDialog = new SettingsDialog(this);
    settingsDialog->ParseSettings();
    settingsDialog->show();

    connect(settingsDialog, &SettingsDialog::SettingsChanged, this, &MainWindow::SettingsChanged);
}

void MainWindow::on_btnAddServer_clicked()
{
    if (ServerDirs.isEmpty())
    {
        on_btnSettings_clicked();
        return;
    }

    bool ok = true;
    QString strDir;
    if (ServerDirs.count() == 1)
        strDir = ServerDirs.first();
    else
        strDir = QInputDialog::getItem(this, "Select servers directory", "Servers directory:", ServerDirs, 0, false, &ok);

    qInfo() << strDir;
    if (!QDir(strDir).exists() && !strDir.isEmpty())
    {
        QMessageBox msgBox(QMessageBox::Icon::Question, "Directory doesn't exist",
                           tr("Directory '%0' doesn't exist.\n"
                              "Do you want to create it?").arg(strDir), {}, this);
        auto *accept = msgBox.addButton("Accept", QMessageBox::ButtonRole::AcceptRole);
        msgBox.addButton("Cancel", QMessageBox::ButtonRole::RejectRole);
        msgBox.exec();
        if (msgBox.clickedButton() == accept)
        {
            QDir dir;
            if (!dir.mkdir(strDir))
            {
                qInfo() << "There was an error creating server directory!";
                QMessageBox msgBox(QMessageBox::Icon::Critical, "Couldn't create server directory",
                                   tr("Couldn't create server directory.\nSelect a different one."), {}, this);
                msgBox.addButton("Ok", QMessageBox::ButtonRole::RejectRole);
                msgBox.exec();
                return;
            }
        }
        else
            return;
    }
    else if (!ok)
        return;

    QString servername = QInputDialog::getText(this, "Give Server Nickname",
                                               "Server Nickname:", QLineEdit::Normal,
                                               "New Server", &ok);
    if (!ok)
        return;
    else if (servername.isEmpty())
        servername = "New Server";

    AddServer(servername, strDir);
}

void MainWindow::on_tabServers_tabCloseRequested(int index)
{
    ServerWindow *server = (ServerWindow*)ui->tabServers->widget(index);
    if (!server->ServerDirectoryExists())
    {
        RemoveServer(index, false);
        return;
    }

    QMessageBox msgBox(QMessageBox::Icon::Warning, "Removing Server",
                       tr("Are you sure you want to remove the server \"%0\"?").arg(ui->tabServers->tabText(index)), {}, this);
    auto *full = msgBox.addButton("Delete server AND files", QMessageBox::ButtonRole::DestructiveRole);
    auto *keepFiles = msgBox.addButton("Keep server files", QMessageBox::ButtonRole::AcceptRole);
    msgBox.addButton("Cancel", QMessageBox::ButtonRole::RejectRole);
    msgBox.exec();

    if (msgBox.clickedButton() == full)
    {
        QMessageBox msgBox2(QMessageBox::Icon::Warning, "Are you double sure?",
                            "Remove ALL server files?", {}, this);
        auto *remove = msgBox2.addButton("I'm sure.", QMessageBox::DestructiveRole);
        msgBox2.addButton("Nevermind.", QMessageBox::RejectRole);
        msgBox2.exec();

        if (msgBox2.clickedButton() == remove)
        {
            RemoveServer(index, true);
        }
    }
    else if (msgBox.clickedButton() == keepFiles)
    {
        RemoveServer(index, false);
    }

    LoadStyles(Settings.ColorTheme);
}

void MainWindow::on_tabServers_tabBarDoubleClicked(int index)
{
    bool ok;
    QString servername = QInputDialog::getText(this, "Change Server Nickname",
                                               "Server Nickname:", QLineEdit::Normal,
                                               ui->tabServers->tabText(index), &ok);
    if (servername.isEmpty() || !ok)
    {
        return;
    }
    ui->tabServers->setTabText(index, servername);

    ServerWindow *server = (ServerWindow*)ui->tabServers->currentWidget();
    QString ServerFolder = server->ServerFolder;
    if (ServerFolder.isEmpty())
    {
        return;
    }

    IniSettings->setValue(QString("%0/nick").arg(QDir(ServerFolder).dirName()), servername);
    IniSettings->setValue(QString("%0/os").arg(QDir(ServerFolder).dirName()), OS);

    qInfo() << "emitting new nick" << servername;
    emit ServerNickChanged(server, servername);
}


void MainWindow::on_btnAbout_clicked()
{
    AboutDialog *aboutDialog = new AboutDialog(this, AppVersion, AppVersionDate);
    aboutDialog->show();
}


void MainWindow::on_btnReloadServers_clicked()
{
    RefreshServerTab();
}

