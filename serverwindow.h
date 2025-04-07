#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QWidget>
#include <QDir>
#include <QFile>
#include <QProcess>

#include "settingsdialog.h"

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

private slots:
    void LoadServerConfig( QDir directory );

    void on_listProps_currentRowChanged(int currentRow);

    void on_btnApply_clicked();

    void on_btnInstallServer_clicked();

    void on_btnStartServer_clicked();

private:
    Ui::ServerWindow *ui;

    QString OS;
};

#endif // SERVERWINDOW_H
