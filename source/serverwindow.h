#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QWidget>

#include "settingsdialog.h"

#include "serverprops/shared.h"

#include "serverprops_main.h"
#include "serverprops_config.h"

namespace Ui {
class ServerWindow;
}

class ServerWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ServerWindow(QWidget *parent = nullptr);
    ServerWindow(SettingsStruct settings, QWidget *parent = nullptr, QString name = "New Server", QString directory = "");

    QString parentFolder;
    QString ServerFolder;

    bool ServerDirectoryExists();

    ~ServerWindow();

public slots:
    void SettingsChanged(SettingsStruct Settings);
    void UpdateStyles(QString colorTheme, QString fullStyle);

    QString GetServerName();
    QString GetServerIP( bool );

    void StartServer();
    void JoinServer();

signals:
    void PassServerApplied( QString ServerFolder );

    void PassServerActivated();
    void PassServerDeactivated();

    void PassSystemNotification(const QString, const QString, int);

private slots:
    void LoadStyles( QString colorTheme, QString fullStyle="" );
    void LoadServerConfig( QDir directory );
    void LoadServerFirstTimeSetup();
    void SetServerVisualState(VisualState state = ServerDefault);
    void SystemNotification(const QString, const QString, int);

    void AddPropToLayout(QWidget *prop, QString title);
    void HidePropLayout(QWidget *prop);
    void ShowPropLayout(QWidget *prop);

    void ServerApplied( const QString );
    void ServerInstalled();

    void on_listProps_currentRowChanged(int currentRow);

private:
    Ui::ServerWindow *ui;

    QString OS;
    QString PublicIP;

    //QSettings *IniSettings;

    ServerProps_Main *SrvMain;
    ServerProps_Config *SrvConfig;

    void printInfo(const QString);

};

#endif // SERVERWINDOW_H
