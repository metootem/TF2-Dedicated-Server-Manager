#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QSettings>

struct SettingsStruct{
    bool valid;

    QString OS;
    QDir ServerDirectory;
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

private:
    Ui::SettingsDialog *ui;
    QSettings Settings;
    QString OS;
};

#endif // SETTINGSDIALOG_H
