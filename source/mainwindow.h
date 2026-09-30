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
#include <QMenu>
#include <QCloseEvent>

#include "settingsdialog.h"
#include "serverwindow.h"
#include "aboutdialog.h"
#include "system_tray_handler.h"

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
    void RefreshSysTray();
    QString PublicIP;
    QString AppVersion;
    QString AppVersionDate;
    QString GetServerNick( ServerWindow* server );

    ~MainWindow();

public slots:
    void FocusWindow();
    void CloseApp();
    void FocusServer( ServerWindow *server );
    void ServerApplied( QString ServerFolder );
    void ShowSystemNotification( const QString, const QString, int );
    QString GetPublicIP();

protected:
    void closeEvent( QCloseEvent *event ) override;

signals:
    void PassSettingsChanged( SettingsStruct Settings );
    void ServerNickChanged( ServerWindow *server, QString newNick );

private slots:
    void LoadStyles(QString colorTheme);

    void AddServer( QString name, QString serverFolder );
    void AddServerToSysTray( ServerWindow *server );
    void RemoveServer( int index, bool removeFiles );
    void RemoveServerFromSysTray( ServerWindow *server );
    bool ServerTabExists( QString );
    void ServerActivated();
    void ServerDeactivated();
    void RefreshServerTab();

    void on_btnAddServer_clicked();
    void on_btnSettings_clicked();
    void on_btnAbout_clicked();
    void on_tabServers_tabCloseRequested(int index);
    void on_tabServers_tabBarDoubleClicked(int index);

    QString sysTrayRequestServerNick( ServerWindow* );

private:
    Ui::MainWindow *ui;

    QString OS;
    SettingsStruct Settings;
    QStringList ServerDirs;
    QSettings *IniSettings;

    //QSystemTrayIcon *SystemTrayIcon;
    SystemTrayHandler *SysTrayHandler;

    //QSettings Settings;
};
#endif // MAINWINDOW_H
