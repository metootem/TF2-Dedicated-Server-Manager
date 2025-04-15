#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::LoadConfig()
{
    SettingsDialog* settingsDialog = new SettingsDialog(this);
    Settings = settingsDialog->ParseSettings();
    if (!Settings.valid)
    {
        settingsDialog->show();

        connect(settingsDialog, &SettingsDialog::SettingsChanged, this, &MainWindow::SettingsChanged);
        return false;
    }
    ServerDir = QDir(Settings.ServerDirectory);
    RefreshServerTab();
    return true;
}

void MainWindow::SettingsChanged( SettingsStruct settings )
{
    ServerDir = settings.ServerDirectory;
    RefreshServerTab();
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
    if (!ServerDir.exists() || ServerDir.path() == ".")
    {
        if (!LoadConfig())
            return;
    }

    bool ok;
    QString servername = QInputDialog::getText(this, "Give Server Nickname",
                                               "Server Nickname:", QLineEdit::Normal,
                                               "New Server", &ok);
    if (!ok)
        return;
    else if (servername.isEmpty())
        servername = "New Server";

    AddServer(servername, "");
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
            dir.removeRecursively();

            QSettings IniSettings("tf2-dsm_config.ini", QSettings::Format::IniFormat);
            IniSettings.remove(dir.dirName());

            ui->tabServers->removeTab(index);
        }
    }
    else if (msgBox.clickedButton() == keepFiles)
    {
        QFile serverFile(tr("%0/server.ini").arg(((ServerWindow*)ui->tabServers->currentWidget())->ServerFolder));
        serverFile.remove();

        QString folder = QDir(((ServerWindow*)ui->tabServers->currentWidget())->ServerFolder).dirName();
        QSettings IniSettings("tf2-dsm_config.ini", QSettings::Format::IniFormat);
        IniSettings.remove(folder);

        ui->tabServers->removeTab(index);
    }
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
    QSettings IniSettings("tf2-dsm_config.ini", QSettings::Format::IniFormat);
    IniSettings.setValue(tr("%0/nick").arg(QDir(ServerFolder).dirName()), servername);
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

    auto newServerWindow = new ServerWindow(this, name, serverFolder);
    int index = ui->tabServers->addTab(newServerWindow, name);

    connect(newServerWindow, SIGNAL(ServerApplied(QString)), this, SLOT(ServerApplied(QString)));
    connect(newServerWindow, SIGNAL(ServerActivated()), this, SLOT(ServerActivated()));
    connect(newServerWindow, SIGNAL(ServerDeactivated()), this, SLOT(ServerDeactivated()));

    if (serverFolder.isEmpty())
        ui->tabServers->setTabIcon(index, QIcon::fromTheme(QIcon::ThemeIcon::FolderNew));
    else
        ui->tabServers->setTabIcon(index, QIcon::fromTheme(QIcon::ThemeIcon::ProcessStop));
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
    qInfo() << ServerFolder;
    QSettings IniSettings("tf2-dsm_config.ini", QSettings::Format::IniFormat);
    QString folder = QDir(ServerFolder).dirName();
    int index = ui->tabServers->currentIndex();

    ui->tabServers->setTabIcon(index, QIcon::fromTheme(QIcon::ThemeIcon::ProcessStop));

    IniSettings.setValue(tr("%0/nick").arg(folder), ui->tabServers->tabText(index));
}

void MainWindow::ServerActivated()
{
    int index = ui->tabServers->currentIndex();
    ui->tabServers->setTabIcon(index, QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaybackStart));
}

void MainWindow::ServerDeactivated()
{
    int index = ui->tabServers->currentIndex();
    ui->tabServers->setTabIcon(index, QIcon::fromTheme(QIcon::ThemeIcon::ProcessStop));
}

void MainWindow::RefreshServerTab()
{
    QSettings IniSettings("tf2-dsm_config.ini", QSettings::Format::IniFormat);
    QFileInfoList fileList = ServerDir.entryInfoList(QDir::Filter::Dirs);
    for (QFileInfo file : fileList)
    {
        if (QFile(tr("%0/server.ini").arg(file.filePath())).exists())
        {
            if (ServerTabExists(IniSettings.value(tr("%0/nick").arg(file.fileName())).toString()))
                continue;
            AddServer(IniSettings.value(tr("%0/nick").arg(file.fileName())).toString(), file.filePath());
            //ServerWindow *newServerWindow = new ServerWindow(this, "", file.filePath());
            //ui->tabServers->addTab(newServerWindow, IniSettings.value(tr("%0/nick").arg(file.fileName())).toString());
            ui->tabServers->setCurrentIndex(ui->tabServers->count()-1);

            //connect(newServerWindow, &ServerWindow::ServerApplied, this, &MainWindow::ServerApplied);
        }
    }
}

