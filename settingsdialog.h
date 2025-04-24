#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QSettings>
#include <QColorDialog>

struct SettingsStruct{
    bool valid;

    QString OS;
    QDir ServerDirectory;

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

    ~SettingsDialog();

signals:
    void SettingsChanged( SettingsStruct settings );

private slots:
    void on_btnApply_clicked();

    void on_btnSrvDir_clicked();

    void on_btnColor_clicked();

    void on_btnColorDefault_clicked();

private:
    Ui::SettingsDialog *ui;

    SettingsStruct Settings;
    QSettings IniSettings;
    QString OS;

    QString colorTheme;
};

#endif // SETTINGSDIALOG_H
