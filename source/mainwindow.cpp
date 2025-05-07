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

    LoadStyles(Settings.ColorTheme);

    ServerDir = QDir(Settings.ServerDirectory);
    RefreshServerTab();
    return true;
}

void MainWindow::SettingsChanged( SettingsStruct settings )
{
    Settings = settings;

    LoadStyles(settings.ColorTheme);

    ServerDir = settings.ServerDirectory;
    RefreshServerTab();

    emit PassSettingsChanged(settings);
}

void MainWindow::LoadStyles(QString colorTheme)
{

    this->setStyleSheet(QString("QTabBar::tab {\n	"
                                "border: 0px solid;\n	"
                                "background-color: #2b2b2b;\n"
                                "padding: 5px;\n"
                                "border-top-left-radius: 3px;\n"
                                "border-top-right-radius: 3px;\n"
                                "color: #ffffff;\n}\n\n"
                                "QTabBar::tab:selected {\n"
                                "background-color: %0;\n}\n\n"
                                "QPushButton {\n	"
                                "border: none;\n"
                                "border-bottom: 3px solid %0;\n"
                                "background-color: #2b2b2b;\n"
                                "font: 13pt \"Noto Sans\";\n"
                                "color: #ffffff\n}\n\n"
                                "QPushButton::hover {\n"
                                "background-color: %1;\n}\n"
                                "QPushButton::pressed {\n"
                                "background-color: %2;\n}\n"
                                "QPushButton:disabled {\n"
                                "color: #3b3b3b;"
                                "\n}\n").arg(colorTheme, QColor(colorTheme).lighter(130).name(), QColor(colorTheme).darker(130).name()));

    if (!ui->tabServers->count())
        ui->tabServers->setStyleSheet("QTabWidget::pane {\n	border: none;\n	background-color: #2b2b2b;\n}");
    else
        ui->tabServers->setStyleSheet(QString("QTabWidget::pane {\n	border: 3px solid %0;\n	background-color: #2b2b2b;\n}").arg(colorTheme));
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

    qInfo() << "Adding Server Tab: " + name;
    auto newServerWindow = new ServerWindow(this, name, serverFolder);
    int index = ui->tabServers->addTab(newServerWindow, name);

    connect(this, SIGNAL(PassSettingsChanged(SettingsStruct)), newServerWindow, SLOT(SettingsChanged(SettingsStruct)));

    connect(newServerWindow, SIGNAL(ServerApplied(QString)), this, SLOT(ServerApplied(QString)));
    connect(newServerWindow, SIGNAL(ServerActivated()), this, SLOT(ServerActivated()));
    connect(newServerWindow, SIGNAL(ServerDeactivated()), this, SLOT(ServerDeactivated()));

    if (serverFolder.isEmpty())
        ui->tabServers->setTabIcon(index, QIcon::fromTheme(QIcon::ThemeIcon::FolderNew));
    else
        ui->tabServers->setTabIcon(index, QIcon::fromTheme(QIcon::ThemeIcon::ProcessStop));

    ui->lblAddServer->hide();
    if (!ui->tabServers->count())
    {
        ui->lblAddServer->show();
        ui->tabServers->setStyleSheet("QTabWidget::pane {\n	border: none;\n	background-color: #2b2b2b;\n}");
    }
    else
        ui->tabServers->setStyleSheet(QString("QTabWidget::pane {\n	border: 3px solid %0;\n	background-color: #2b2b2b;\n}").arg(Settings.ColorTheme));
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

    ui->tabServers->setTabIcon(index, QIcon::fromTheme(QIcon::ThemeIcon::ProcessStop));

    IniSettings->setValue(QString("%0/nick").arg(folder), ui->tabServers->tabText(index));
    IniSettings->setValue(QString("%0/os").arg(folder), OS);
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
    QFileInfoList fileList = ServerDir.entryInfoList(QDir::Filter::Dirs);
    for (QFileInfo file : fileList)
    {
        if (QFile(QString("%0/server.ini").arg(file.absoluteFilePath())).exists())
        {
            QSettings fileIniSettings(QString("%0/server.ini").arg(file.absoluteFilePath()), QSettings::IniFormat);
            QString serverNick = IniSettings->value(QString("%0/nick").arg(file.fileName())).toString();
            if (ServerTabExists(serverNick) || fileIniSettings.value("os").toString() != OS)
                continue;

            if (serverNick.isEmpty())
                serverNick = file.fileName();
            AddServer(serverNick, file.filePath());
            ui->tabServers->setCurrentIndex(ui->tabServers->count()-1);
            ServerApplied(file.absoluteFilePath());
        }
    }
    for (QString server : IniSettings->childGroups())
    {
        if (server == OS)
            continue;
        if (!QDir(QString("%0/%1").arg(ServerDir.absolutePath(), server)).exists())
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

    if (!ui->tabServers->count())
        ui->tabServers->setStyleSheet("QTabWidget::pane {\n	border: none;\n	background-color: #2b2b2b;\n}");
    else
        ui->tabServers->setStyleSheet("QTabWidget::pane {\n	border: 3px solid #cf6a32;\n	background-color: #2b2b2b;\n}");
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

