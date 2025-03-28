#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
    , Settings("tf2-dsm_config.ini", QSettings::Format::IniFormat)
{
    ui->setupUi(this);
    ui->lblSrvDirError->hide();
    ui->lblApplySuccess->hide();
    OS = QSysInfo::productType();
    if (OS != "windows" && OS != "macos")
        OS = "linux";

    //connect(this, )
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

SettingsStruct SettingsDialog::ParseSettings()
{
    SettingsStruct settings;
    settings.valid = true;
    settings.OS = OS;

    if (!Settings.contains(tr("%0/server_directory").arg(OS)))
    {
        settings.valid = false;
        Settings.setValue(tr("%0/server_directory").arg(OS), "Input server directory.");
    }
    else if (!QDir(Settings.value(tr("%0/server_directory").arg(OS)).toString()).exists())
    {
        settings.valid = false;
        ui->lblSrvDirError->show();
    }
    else
        settings.ServerDirectory = Settings.value(tr("%0/server_directory").arg(OS)).toString();

    ui->lineSrvDir->setText(Settings.value(tr("%0/server_directory").arg(OS)).toString());

    return settings;
}

void SettingsDialog::on_btnApply_clicked()
{
    bool apply = true;
    ui->lblSrvDirError->hide();
    ui->lblApplySuccess->hide();

    if (!QDir(ui->lineSrvDir->text()).exists() || ui->lineSrvDir->text() == "")
    {
        ui->lblSrvDirError->show();
        apply = false;
    }

    if (apply)
    {
        Settings.setValue(tr("%0/server_directory").arg(OS), ui->lineSrvDir->text());
        SettingsStruct settingsstruct;
        settingsstruct.valid = true;
        settingsstruct.ServerDirectory = Settings.value(tr("%0/server_directory").arg(OS)).toString();
        emit SettingsChanged(settingsstruct);
        ui->lblApplySuccess->show();
    }
}


void SettingsDialog::on_btnSrvDir_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Open Directory", QDir::currentPath(), QFileDialog::ShowDirsOnly);
    ui->lineSrvDir->setText(dir);
}

