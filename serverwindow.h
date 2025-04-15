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

#include <QHostAddress>
#include <QNetworkInterface>

#include "settingsdialog.h"
#include "serverconsoledialog.h"

enum VisualState
{
    ServerStarted = 0,
    ServerFinished,
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

    ~ServerWindow();

signals:
    void ServerApplied( QString ServerFolder );

    void ServerActivated();
    void ServerDeactivated();

private slots:
    void LoadServerConfig( QDir directory );
    QString GetPublicIP();

    void on_listProps_currentRowChanged(int currentRow);

    void on_btnApply_clicked();

    void on_btnInstallServer_clicked();

    void on_btnStartServer_clicked();

    void on_btnStopServer_clicked();

    void ServerStateChanged( QProcess::ProcessState state );
    void SetServerVisualState(VisualState state);
    void ReadOutput();

    void on_btnShowConsole_clicked();

    void on_btnConnectToServer_clicked();

    void on_btnCopyIp_clicked();

private:
    Ui::ServerWindow *ui;

    QString OS;
    QString PublicIP;

    QProcess *ServerProcess;
    ServerConsoleDialog *ServerConsole;

    QTimer *ConsoleRefreshRate;
    int RefreshCount;
};

#endif // SERVERWINDOW_H
