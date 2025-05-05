#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
    , IniSettings("tf2-dsm_config.ini", QSettings::Format::IniFormat)
{
    ui->setupUi(this);
    ui->lblSrvDirError->hide();
    ui->lblApplySuccess->hide();
    ui->lblTip->hide();
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
    Settings.valid = true;
    Settings.OS = OS;

    if (!IniSettings.contains(tr("%0/color_theme").arg(OS)))
        IniSettings.setValue(tr("%0/color_theme").arg(OS), "#cf6a32");

    if (!IniSettings.contains(tr("%0/server_directory").arg(OS)))
    {
        Settings.valid = false;
        IniSettings.setValue(tr("%0/server_directory").arg(OS), "Input server directory.");
        ui->lblTip->show();
    }
    else if (!QDir(IniSettings.value(tr("%0/server_directory").arg(OS)).toString()).exists())
    {
        Settings.valid = false;
        ui->lblSrvDirError->show();
        ui->lblTip->show();
    }
    else
    {
        Settings.ColorTheme = IniSettings.value(tr("%0/color_theme").arg(OS)).toString();
        Settings.ServerDirectory = IniSettings.value(tr("%0/server_directory").arg(OS)).toString();
    }

    colorTheme = IniSettings.value(OS + "/color_theme").toString();

    ui->btnColor->setStyleSheet(QString("border: 2px solid #232323;\nborder-radius: 0px;\nbackground-color: %0;").arg(colorTheme));
    ui->lineSrvDir->setText(IniSettings.value(tr("%0/server_directory").arg(OS)).toString());

    return Settings;
}

void SettingsDialog::on_btnApply_clicked()
{
    bool apply = true;
    ui->lblSrvDirError->hide();
    ui->lblApplySuccess->hide();
    ui->lblTip->hide();

    if (!QDir(ui->lineSrvDir->text()).exists() || ui->lineSrvDir->text() == "")
    {
        if (Settings.ServerDirectory.absolutePath().isEmpty())
        {
            ui->lblSrvDirError->show();
            ui->lblTip->show();
            apply = false;
        }
        else
            ui->lineSrvDir->setText(Settings.ServerDirectory.absolutePath());
    }

    if (apply)
    {
        Settings.valid = true;
        Settings.ServerDirectory = ui->lineSrvDir->text();
        Settings.ColorTheme = colorTheme;

        IniSettings.setValue(tr("%0/server_directory").arg(OS), Settings.ServerDirectory.absolutePath());
        IniSettings.setValue(tr("%0/color_theme").arg(OS), Settings.ColorTheme);

        emit SettingsChanged(Settings);
        ui->lblApplySuccess->show();
    }
}


void SettingsDialog::on_btnSrvDir_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Open Directory", QDir::currentPath(), QFileDialog::ShowDirsOnly);
    if (!dir.isEmpty())
        ui->lineSrvDir->setText(dir);
}


void SettingsDialog::on_btnColor_clicked()
{
    QColor hex = QColorDialog::getColor(QColor(colorTheme), this, tr("Select Color"));

    if (!hex.isValid())
        return;
    ui->btnColor->setStyleSheet(QString("border: 2px solid #232323;\nborder-radius: 0px;\nbackground-color: %0;").arg(hex.name()));
    colorTheme = hex.name();
}


void SettingsDialog::on_btnColorDefault_clicked()
{
    ui->btnColor->setStyleSheet(QString("border: 2px solid #232323;\nborder-radius: 0px;\nbackground-color: #cf6a32;"));
    colorTheme = "#cf6a32";
}

