#ifndef SERVERPROPS_CONFIG_H
#define SERVERPROPS_CONFIG_H

#include <QWidget>
#include <QTreeWidgetItem>
#include <QFile>
#include <QScrollBar>

#include "configconvardialog.h"
#include "cfg_loadmapsdialog.h"
#include "serverprops/shared.h"

namespace Ui {
class ServerProps_Config;
}

class ServerProps_Config : public QWidget
{
    Q_OBJECT

public:
    explicit ServerProps_Config(QWidget *parent, QString directory);

    void LoadStyles(const QString, const QString="" );

    void CheckServerConfigFiles();

    QString GetSelectedConfigFile();
    void SelectConfigFile( const QString );

    ~ServerProps_Config();

public slots:
    void ServerInstalled();

signals:
    void SystemNotification(const QString, const QString, int);

private slots:
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
    void on_btnRefreshConfigList_clicked();
    void on_btnNewConfigFile_clicked();
    void on_btnClearConfig_clicked();

private:
    Ui::ServerProps_Config *ui;

    QString OS;

    QString ServerFolder;
};

#endif // SERVERPROPS_CONFIG_H
