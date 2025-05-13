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
#include <QTreeWidgetItem>
#include <QScrollBar>

#include <QHostAddress>
#include <QNetworkInterface>

#include "settingsdialog.h"
#include "serverconsoledialog.h"
#include "filedownloader.h"
#include "steamcmddialog.h"
#include "additionalparametersdialog.h"
#include "selectmapdialog.h"
#include "configconvardialog.h"

enum VisualState
{
    ServerDefault = 0,
    ServerStarted,
    ServerStopped,
    ServerDownloading,
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
    ServerWindow(SettingsStruct settings, QWidget *parent = nullptr, QString name = "New Server", QString directory = "");

    QString ServerFolder;
    bool ServerInstalling = false;

    ~ServerWindow();

public slots:
    void SettingsChanged(SettingsStruct Settings);
    void InstallServerFinished();

signals:
    void ServerApplied( QString ServerFolder );

    void ServerActivated();
    void ServerDeactivated();

private slots:
    void LoadStyles( QString colorTheme );
    void LoadServerConfig( QDir directory );
    void LoadServerFirstTimeSetup();

    bool SteamCMDExists();
    bool SteamCMDZipExists();
    bool SRCDSExists();
    void DownloadSteamCMD();
    void InstallSteamCMD();
    void InstallServer();
    void KillSteamCMDProcess();

    void on_listProps_currentRowChanged(int currentRow);

    void on_btnApply_clicked();

    void on_btnShowConsole_clicked();
    void on_btnSteamCMDConsole_clicked();

    void on_btnInstallServer_clicked();
    void on_btnStartServer_clicked();
    //void on_btnStopServer_clicked();
    void on_btnConnectToServer_clicked();

    void on_btnCopyIp_clicked();
    void on_btnParameters_clicked();
    void on_btnSelectMap_clicked();

    void SetServerVisualState(VisualState state = ServerDefault);

    void CheckServerConfigFiles();
    void LoadServerConfigFileData();
    void AddConfigTreeItem(QString ConVar, QString Value, QString Comment, QTreeWidgetItem* parent = nullptr);
    QString ServerCfgExample();

    void on_cmbConfigFile_currentTextChanged(const QString &arg1);

    void on_btnAddConVar_clicked();

    void on_btnDelConVar_clicked();

    void on_btnSaveConfig_clicked();

    void on_btnReloadConfig_clicked();

    void on_btnOpenConfig_clicked();

    void on_btnConfigSpecial_clicked();

    void on_btnFindConVar_clicked();

    void on_btnGotoServerFolder_clicked();

    void on_btnRefreshConfigList_clicked();

    void on_btnNewConfigFile_clicked();

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

};

#endif // SERVERWINDOW_H
