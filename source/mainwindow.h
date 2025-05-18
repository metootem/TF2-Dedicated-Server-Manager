#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <QSettings>
#include <QInputDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QSystemTrayIcon>

#include "settingsdialog.h"
#include "serverwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    bool LoadConfig();
    void SettingsChanged( SettingsStruct settings );

    ~MainWindow();

public slots:
    void ServerApplied( QString ServerFolder );
    void ShowSystemNotification( QString, QString, int );

signals:
    void PassSettingsChanged( SettingsStruct Settings );

private slots:
    void LoadStyles(QString colorTheme);

    void on_btnAddServer_clicked();

    void AddServer(QString name, QString serverFolder);
    bool ServerTabExists(QString);
    void ServerActivated();
    void ServerDeactivated();
    void RefreshServerTab();

    void on_btnSettings_clicked();

    void on_tabServers_tabCloseRequested(int index);

    void on_tabServers_tabBarDoubleClicked(int index);

private:
    Ui::MainWindow *ui;

    QString OS;
    SettingsStruct Settings;
    QStringList ServerDirs;
    QSettings *IniSettings;

    QSystemTrayIcon *SystemTrayIcon;

    //QSettings Settings;
};
#endif // MAINWINDOW_H
