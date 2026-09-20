#ifndef SYSTEM_TRAY_HANDLER_H
#define SYSTEM_TRAY_HANDLER_H

#include <QSystemTrayIcon>
#include <QObject>
#include <QMenu>

#include "serverwindow.h"

class SystemTrayHandler : public QObject
{
    Q_OBJECT
public:
    SystemTrayHandler(QObject *parent);

    bool Exists();
    void AddServerToSysTray( ServerWindow* );
    void RemoveServerFromSysTray( ServerWindow* );

    QSystemTrayIcon* GetSystemTrayIcon();
    void ShowMessage(const QString &Title, const QString &Message, const int &length);

    ~SystemTrayHandler();

public slots:
    void ServerNickChanged( ServerWindow*, QString );

signals:
    void Clicked();
    void Close();
    void StartServer( ServerWindow* );
    void CopyIP( ServerWindow* );
    QString RequestServerNick ( ServerWindow* );

private slots:
    void slotClicked();
    void slotClose();
    void slotStartServer();
    void slotCopyIP();
    void onDestroyed();

private:
    int getServerIndex( ServerWindow* );
    ServerWindow* getServerFromIndex( const int& );
    ServerWindow* getServerFromSysTray( QAction* );
    QAction* findActionParent( QAction* );

    QSystemTrayIcon *SysTrayIcon;
    QList<ServerWindow*> ServerList;
    QStringList ServerNickList;
    //MainWindow *MWParent;
    void printInfo(const QString &Message);

};

#endif // SYSTEM_TRAY_HANDLER_H
