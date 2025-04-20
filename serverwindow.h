#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QWidget>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QTextBrowser>
#include <QThread>
#include <QMessageBox>
#include <QInputDialog>
#include <QDesktopServices>
#include <QTimer>
#include <QClipboard>
#include <QSystemTrayIcon>
#include <QIcon>

#include <QHostAddress>
#include <QNetworkInterface>

#include "settingsdialog.h"
#include "serverconsoledialog.h"
#include "filedownloader.h"
#include "steamcmddialog.h"
#include "additionalparametersdialog.h"
#include "selectmapdialog.h"

enum VisualState
{
    ServerDefault = 0,
    ServerStarted,
    ServerStopped,
    ServerInstalling,
    ServerFinishedInstalling,
};

namespace Ui {
class ServerWindow;
}

class ServerWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ServerWindow(QWidget *parent = nullptr);
    ServerWindow(QWidget *parent = nullptr, QString name = "New Server", QString directory = "");

    QString ServerFolder;
    bool ServerInstalling = false;

    ~ServerWindow();

public slots:
    void InstallServerFinished();

signals:
    void ServerApplied( QString ServerFolder );

    void ServerActivated();
    void ServerDeactivated();

private slots:
    void LoadServerConfig( QDir directory );
    void LoadServerFirstTimeSetup();
    QString GetPublicIP();

    bool SteamCMDExists();
    bool SteamCMDZipExists();
    bool SRCDSExists();
    void DownloadSteamCMD();
    void InstallSteamCMD();
    void InstallServer();
    void KillSteamCMDProcess();

    void on_listProps_currentRowChanged(int currentRow);

    void on_btnApply_clicked();

    void on_btnInstallServer_clicked();

    void on_btnStartServer_clicked();

    void on_btnStopServer_clicked();

    void SetServerVisualState(VisualState state = ServerDefault);

    void on_btnShowConsole_clicked();

    void on_btnConnectToServer_clicked();

    void on_btnCopyIp_clicked();

    void on_btnSteamCMDConsole_clicked();

    void on_btnParameters_clicked();

    void on_btnSelectMap_clicked();

private:
    Ui::ServerWindow *ui;

    QString OS;
    QString PublicIP;

    QSettings *IniSettings;
    QProcess *SteamCMDProcess;
    QProcess *ServerProcess;
    ServerConsoleDialog *ServerConsole;
    SteamCMDDialog *SteamCMDWindow;
    AdditionalParametersDialog *AdditionalParametersWindow;

    QTimer *ConsoleRefreshRate;
    int RefreshCount;
};

#endif // SERVERWINDOW_H
