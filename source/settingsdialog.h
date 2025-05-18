#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QSettings>
#include <QColorDialog>
#include <QMessageBox>
#include <QProcess>

struct SettingsStruct{
    bool valid;

    QString OS;
    QStringList ServerDirectories;

    QString PublicIP;
    QString ColorTheme;
};

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);

    SettingsStruct ParseSettings();
    QString GetPublicIP();

    ~SettingsDialog();

signals:
    void SettingsChanged( SettingsStruct settings );

private slots:
    void on_btnApply_clicked();

    void on_btnAddSrvDir_clicked();

    void on_btnColor_clicked();

    void on_btnColorDefault_clicked();

    void on_btnRmvSrvDir_clicked();

private:
    Ui::SettingsDialog *ui;

    SettingsStruct Settings;
    QSettings IniSettings;
    QString OS;

    QString colorTheme;
};

#endif // SETTINGSDIALOG_H
