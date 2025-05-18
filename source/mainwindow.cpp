#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    OS = QSysInfo::productType();
    if (OS != "windows" && OS != "macos")
        OS = "linux";
    IniSettings = new QSettings("tf2-dsm_config.ini", QSettings::Format::IniFormat);

    if (QSystemTrayIcon::isSystemTrayAvailable())
    {
        SystemTrayIcon = new QSystemTrayIcon(this);
        SystemTrayIcon->setIcon(QIcon(":/tf2dsm.ico"));
        SystemTrayIcon->setVisible(true);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ShowSystemNotification(QString title, QString message, int length)
{
    if (QSystemTrayIcon::isSystemTrayAvailable())
        SystemTrayIcon->showMessage(title, message, QIcon(":/tf2dms.ico"), length);
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

    ServerDirs = Settings.ServerDirectories;
    RefreshServerTab();

    LoadStyles(Settings.ColorTheme);

    return true;
}

void MainWindow::SettingsChanged( SettingsStruct settings )
{
    Settings = settings;
    ServerDirs = settings.ServerDirectories;
    RefreshServerTab();
    LoadStyles(settings.ColorTheme);

    ShowSystemNotification("Settings changed", "", 3000);

    emit PassSettingsChanged(settings);
}

void MainWindow::LoadStyles(QString colorTheme)
{

    this->setStyleSheet(QString("QInputDialog { background-color: #2b2b2b; }"
                                "QMessageBox { background-color: #2b2b2b; }"
                                "QLabel { color: #ffffff; }"
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

    ui->lblAddServer->hide();
    if (!ui->tabServers->count())
    {
        ui->lblAddServer->show();
        ui->tabServers->setStyleSheet("QTabWidget::pane { border: none; background-color: #2b2b2b; }");
    }
    else
        ui->tabServers->setStyleSheet(QString("QTabWidget::pane { border-bottom: 0px solid #5a5a5a;"
                                              "border-top: 2px solid %0;"
                                              "background-color: #2b2b2b; }").arg(colorTheme));
}


void MainWindow::AddServer(QString servername, QString serverFolder)
{
    QString name = servername;
    int found_count = 0;
    do
    {
        found_count++;
        if (found_count > 1)
            name = tr("%0 %1").arg(servername).arg(found_count, 1);
    } while (ServerTabExists(name));

    qInfo() << " ";
    qInfo() << "Adding Server Tab: " + name;
    auto newServerWindow = new ServerWindow(Settings, this, name, serverFolder);
    int index = ui->tabServers->addTab(newServerWindow, name);

    connect(this, SIGNAL(PassSettingsChanged(SettingsStruct)), newServerWindow, SLOT(SettingsChanged(SettingsStruct)));

    connect(newServerWindow, SIGNAL(ServerApplied(QString)), this, SLOT(ServerApplied(QString)));
    connect(newServerWindow, SIGNAL(ServerActivated()), this, SLOT(ServerActivated()));
    connect(newServerWindow, SIGNAL(ServerDeactivated()), this, SLOT(ServerDeactivated()));
    connect(newServerWindow, SIGNAL(SystemNotification(QString, QString, int)), SLOT(ShowSystemNotification(QString, QString, int)));

    if (serverFolder.isEmpty())
        ui->tabServers->setTabIcon(index, QIcon(":/icons/resources/Icons/Add.svg"));
    else
        ui->tabServers->setTabIcon(index, QIcon(":/icons/resources/Icons/ServerInactive.svg"));

    LoadStyles(Settings.ColorTheme);
}

bool MainWindow::ServerTabExists(QString name)
{
    for (int i=0; i<ui->tabServers->count(); i++)
    {
        if (ui->tabServers->tabText(i) == name)
            return true;
    }
    return false;
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
        auto srvWindow = ((ServerWindow*)ui->tabServers->widget(i));
        if (ServerDirs.contains(srvWindow->ServerFolder))
            continue;
        QDir dir(srvWindow->ServerFolder);
        dir.cdUp();
        if (ServerDirs.contains(dir.path()))
            continue;
        qInfo() << "Removed server tab" << ui->tabServers->tabText(i);
        ui->tabServers->removeTab(i--);
    }

    for (QString srvDir : ServerDirs) // Find new servers
    {
        qInfo() << "Searching in" << srvDir;
        QDir ServerDir(srvDir);
        QFileInfoList fileList = ServerDir.entryInfoList(QDir::Filter::Dirs);
        for (QFileInfo file : fileList)
        {
            if (QFile(QString("%0/server.ini").arg(file.absoluteFilePath())).exists())
            {
                QSettings fileIniSettings(QString("%0/server.ini").arg(file.absoluteFilePath()), QSettings::IniFormat);
                if (!IniSettings->contains(file.fileName()) && fileIniSettings.value("os").toString() == OS)
                {
                    IniSettings->setValue(QString("%0/nick").arg(file.fileName()), file.fileName());
                    IniSettings->setValue(QString("%0/os").arg(file.fileName()), OS);
                }
                QString serverNick = IniSettings->value(QString("%0/nick").arg(file.fileName())).toString();
                if (ServerTabExists(serverNick) || fileIniSettings.value("os").toString() != OS)
                    continue;
                if (serverNick.isEmpty())
                    serverNick = file.fileName();
                AddServer(serverNick, file.filePath());
                ui->tabServers->setCurrentIndex(ui->tabServers->count()-1);
            }
        }
    }

    for (QString server : IniSettings->childGroups()) // Remove unused servers from config
    {
        if (server == OS)
            continue;
        bool remove = true;
        for (QString parentDir : ServerDirs)
        {
            if (QDir(QString("%0/%1").arg(parentDir, server)).exists() && server != ".")
                remove = false;
        }
        if (remove)
            IniSettings->remove(server);
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
    bool ok;
    QString strDir = QInputDialog::getItem(this, "Select servers directory", "Servers directory:", ServerDirs, 0, false, &ok);

    if (!QDir(strDir).exists() && !strDir.isEmpty())
    {
        QMessageBox msgBox(QMessageBox::Icon::Question, "Directory doesn't exist",
                           tr("Directory '%0' doesn't exist.\n"
                              "Do you want to create it?").arg(strDir), {}, this);
        auto *accept = msgBox.addButton("Accept", QMessageBox::ButtonRole::AcceptRole);
        msgBox.addButton("Cancel", QMessageBox::ButtonRole::RejectRole);
        msgBox.exec();
        if (msgBox.clickedButton() != accept)
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
            QDir dir(((ServerWindow*)ui->tabServers->currentWidget())->ServerFolder);
            if (dir.dirName() != ".")
                dir.removeRecursively();

            IniSettings->remove(dir.dirName());

            ui->tabServers->removeTab(index);
        }
    }
    else if (msgBox.clickedButton() == keepFiles)
    {
        QFile serverFile(QString("%0/server.ini").arg(((ServerWindow*)ui->tabServers->currentWidget())->ServerFolder));
        serverFile.remove();

        QString folder = QDir(((ServerWindow*)ui->tabServers->currentWidget())->ServerFolder).dirName();
        IniSettings->remove(folder);

        ui->tabServers->removeTab(index);
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
        return;
    ui->tabServers->setTabText(index, servername);
    QString ServerFolder = ((ServerWindow*)ui->tabServers->currentWidget())->ServerFolder;
    if (ServerFolder.isEmpty())
        return;
    IniSettings->setValue(QString("%0/nick").arg(QDir(ServerFolder).dirName()), servername);
    IniSettings->setValue(QString("%0/os").arg(QDir(ServerFolder).dirName()), OS);
}

