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

#include "settingsdialog.h"

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
    void ServerApplied( QString ServerFolder );

    ~MainWindow();

signals:

private slots:
    void on_btnAddServer_clicked();

    bool ServerTabExists(QString);
    void RefreshServerTab();

    void on_btnSettings_clicked();

    void on_tabServers_tabCloseRequested(int index);

    void on_tabServers_tabBarDoubleClicked(int index);

private:
    Ui::MainWindow *ui;

    SettingsStruct Settings;
    QDir ServerDir;

    //QSettings Settings;
};
#endif // MAINWINDOW_H
