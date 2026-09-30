#ifndef SERVERPROPS_MAIN_H
#define SERVERPROPS_MAIN_H

#include <QWidget>
#include <QFile>
#include <QDialog>
#include <QMessageBox>
#include <QProcess>
#include <QClipboard>
#include <QNetworkInterface>
#include <QInputDialog>

#include "filedownloader.h"
#include "serverconsoledialog.h"
#include "steamcmddialog.h"
#include "additionalparametersdialog.h"
#include "settingsdialog.h"
#include "selectmapdialog.h"

#include "serverprops/shared.h"

namespace Ui {
class ServerProps_Main;
}

class ServerProps_Main : public QWidget
{
    Q_OBJECT

public:
    explicit ServerProps_Main(QWidget *parent, QString name, QString directory);

    void LoadStyles( const QString, const QString="" );
    void FirstTimeSetup();
    void LoadServerConfig( const QDir );

    QString GetServerDirectory();
    QString GetServerFolder();
    QString GetName();
    QString GetIP(bool copyToClipboard);

    void StartServer();
    void JoinServer();

    void SetName( const QString );
    void SetIP( const QString );

    bool SteamCMDExists();
    bool SteamCMDZipExists();
    bool SRCDSExists();

    ~ServerProps_Main();

signals:
    void SystemNotification(const QString, const QString, int);
    void ServerActivated();
    void ServerDeactivated();
    void ServerApplied( const QString );
    void ServerInstalled();

public slots:
    void SetServerVisualState(VisualState state = ServerDefault);

private slots:
    void DownloadSteamCMD();
    void InstallSteamCMD();
    void InstallServer();
    void InstallServerFinished();

    void on_btnApply_clicked();

    void on_btnShowConsole_clicked();
    void on_btnSteamCMDConsole_clicked();

    void on_btnInstallServer_clicked();
    void on_btnStartServer_clicked();
    //void on_btnStopServer_clicked();
    void on_btnConnectToServer_clicked();
    void on_btnGotoServerFolder_clicked();
    void on_btnAdvancedDropDown_clicked();

    void on_btnCopyIp_clicked();
    void on_btnParameters_clicked();
    void on_btnSelectMap_clicked();

private:
    Ui::ServerProps_Main *ui;

    QString OS;
    QString ServerFolder;
    QString PublicIP;

    QString FormatDirectory( const QString );

    bool ServerInstalling = false;

    QProcess *SteamCMDProcess;
    QProcess *ServerProcess;

    ServerConsoleDialog *ServerConsole;
    SteamCMDDialog *SteamCMDWindow;
    AdditionalParametersDialog *AdditionalParametersWindow;
};

#endif // SERVERPROPS_MAIN_H
