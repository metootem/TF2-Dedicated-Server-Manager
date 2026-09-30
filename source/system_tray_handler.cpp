#include "system_tray_handler.h"

SystemTrayHandler::SystemTrayHandler(QObject *parent)
    : QObject{parent}
{
    if (!QSystemTrayIcon::isSystemTrayAvailable())
    {
        return;
    }

    SysTrayIcon = new QSystemTrayIcon(this);
    SysTrayIcon->setIcon(QIcon(":/tf2dsm.ico"));
    SysTrayIcon->setVisible(true);

    QMenu *menu = new QMenu();

    menu->addSeparator();

    QAction *quitAct = new QAction();
    quitAct->setText("Close");

    connect(quitAct, &QAction::triggered, this, &SystemTrayHandler::slotClose);
    menu->addAction(quitAct);
    SysTrayIcon->setContextMenu(menu);

    connect(SysTrayIcon, &QSystemTrayIcon::activated, this, &SystemTrayHandler::slotClicked);
    connect(SysTrayIcon, &QSystemTrayIcon::destroyed, this, &SystemTrayHandler::onDestroyed);
}

SystemTrayHandler::~SystemTrayHandler()
{
    if (Exists())
    {
        SysTrayIcon->hide();
        SysTrayIcon->deleteLater();
    }
}

bool SystemTrayHandler::Exists()
{
    return QSystemTrayIcon::isSystemTrayAvailable() && SysTrayIcon != nullptr;
}

QSystemTrayIcon* SystemTrayHandler::GetSystemTrayIcon()
{
    return SysTrayIcon;
}

void SystemTrayHandler::onDestroyed()
{
    SysTrayIcon = nullptr;
}

int SystemTrayHandler::getServerIndex(ServerWindow *server)
{
    int index = 0;
    for (ServerWindow *target : std::as_const(ServerList))
    {
        if (target == server)
        {
            return index;
        }
        index++;
    }
    return -1;
}

ServerWindow* SystemTrayHandler::getServerFromIndex(const int &index)
{
    if (index >= ServerList.count() || index < 0)
    {
        return nullptr;
    }
    return ServerList.takeAt(index);
}

ServerWindow* SystemTrayHandler::getServerFromSysTray(QAction *action)
{
    QList<QAction*> actionList = SysTrayIcon->contextMenu()->actions();
    int index = 0;
    for (QAction *target : std::as_const(actionList))
    {
        if (target == action)
        {
            return ServerList.takeAt(index);
        }
        index++;
    }
    return nullptr;
}

QAction* SystemTrayHandler::findActionParent(QAction *action)
{
    QList<QAction*> mainActionList = SysTrayIcon->contextMenu()->actions();
    for (QAction *mainAction : std::as_const(mainActionList))
    {
        QMenu *mainMenu = mainAction->menu();
        auto actionList = mainMenu->actions();
        for (QAction *target : std::as_const(actionList))
        {
            if (target == action)
            {
                return mainAction;
            }
        }
    }
    return nullptr;
}

void SystemTrayHandler::ShowMessage(const QString &title, const QString &message, const int &length)
{
    SysTrayIcon->showMessage(title, message, QIcon(":/tf2dms.ico"), length);
}

void SystemTrayHandler::AddServerToSysTray(ServerWindow *server)
{
    QString serverNick = emit RequestServerNick(server);

    QMenu *serverMenu = new QMenu();
    serverMenu->setTitle(serverNick);

    /*QAction *focusAct = new QAction();
    focusAct->setText("Focus");
    connect(focusAct, &QAction::triggered, [&focusAct]() {
        QString serverName = ((QMenu*)focusAct->parent())->title();

    });*/

    QAction *startAct = new QAction();
    startAct ->setText("Start");
    connect(startAct, &QAction::triggered, server, &ServerWindow::StartServer);

    QAction *copyAct = new QAction();
    copyAct->setText(QString("IP: %0").arg(server->GetServerIP(false)));
    connect(copyAct, &QAction::triggered, this, &SystemTrayHandler::slotCopyIP);

    QAction *joinAct = new QAction();
    joinAct->setText("Join");
    connect(joinAct, &QAction::triggered, server, &ServerWindow::JoinServer);

    //serverMenu->addAction(focusAct);
    serverMenu->addAction(startAct);
    serverMenu->addAction(joinAct);
    serverMenu->addSeparator();
    serverMenu->addAction(copyAct);

    int lastIndex = SysTrayIcon->contextMenu()->actions().count()-2; // The separator
    QAction *lastAction = SysTrayIcon->contextMenu()->actions().at(lastIndex);

    SysTrayIcon->contextMenu()->insertMenu(lastAction, serverMenu);

    ServerList << server;
}

void SystemTrayHandler::RemoveServerFromSysTray(ServerWindow *server)
{
    QString serverNick = emit RequestServerNick(server);

    QList<QAction *> actionList = SysTrayIcon->contextMenu()->actions();
    for (QAction *action : std::as_const(actionList))
    {
        if (action->text() == serverNick)
        {
            SysTrayIcon->contextMenu()->removeAction(action);
            return;
        }
    }

    int index = getServerIndex(server);
    if (index != -1)
    {
        ServerList.removeAt(index);
        ServerList.squeeze();
    }
}

void SystemTrayHandler::slotClicked()
{
    emit Clicked();
}

void SystemTrayHandler::slotClose()
{
    emit Close();
}

void SystemTrayHandler::slotStartServer()
{
    //emit StartServer();
    QAction *action = (QAction*)sender();
    QAction *mainAction = findActionParent(action);
    if (mainAction != nullptr)
    {
        ServerWindow* server = getServerFromSysTray(mainAction);
        if (server != nullptr)
        {
            printInfo(QString("Attempting to start %0 server.").arg(mainAction->text()));
            server->StartServer();
        }
        else
        {
            printInfo("Server not found.");
        }
    }
    else
    {
        printInfo("mainAction not found.");
    }
}

void SystemTrayHandler::slotCopyIP()
{
    //emit CopyIP();
    QAction *action = (QAction*)sender();
    QAction *mainAction = findActionParent(action);
    if (mainAction != nullptr)
    {
        ServerWindow* server = getServerFromSysTray(mainAction);
        if (server != nullptr)
        {
            printInfo(server->GetServerIP(true));
        }
        else
        {
            printInfo("Server not found.");
        }
    }
    else
    {
        printInfo("mainAction not found.");
    }
}

void SystemTrayHandler::ServerNickChanged(ServerWindow *server, QString newNick)
{
    int serverIndex = getServerIndex(server);
    if (serverIndex == -1)
    {
        qInfo() << "Server tab exists but not in system tray?";
        return;
    }

    QList<QAction*> actionList = SysTrayIcon->contextMenu()->actions();
    if (actionList.count() <= 2)
    {
        printInfo("Server tab exists but no servers in tray?");
    }
    QAction *action = actionList.takeAt(serverIndex);

    printInfo(QString("new nick %0 -> %1").arg(action->text(), newNick));
    action->setText(newNick);

}

void SystemTrayHandler::printInfo(const QString &message)
{
    qInfo() << "SystemTrayHandler:" << message;
}