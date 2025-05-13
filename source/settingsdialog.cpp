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

QString SettingsDialog::GetPublicIP()
{
    QProcess GetIP;
    GetIP.start("curl", QStringList() << "https://api.ipify.org");
    GetIP.waitForFinished(5000);
    QString IP = GetIP.readAllStandardOutput();
    GetIP.terminate();
    if (IP.isEmpty())
        qInfo() << "Couldn't get Public IP.";
    else
        qInfo() << "Got Public IP:" << IP;
    return IP;
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

    Settings.PublicIP = GetPublicIP();

    colorTheme = IniSettings.value(OS + "/color_theme").toString();

    ui->btnColor->setStyleSheet(QString("border: 2px solid #232323; border-radius: 0px; background-color: %0; color: #ffffff;").arg(colorTheme));
    ui->lineSrvDir->setText(IniSettings.value(tr("%0/server_directory").arg(OS)).toString());

    return Settings;
}

void SettingsDialog::on_btnApply_clicked()
{
    bool apply = true;
    ui->lblSrvDirError->hide();
    ui->lblApplySuccess->hide();
    ui->lblTip->hide();

    if (ui->lineSrvDir->text() == "")
    {
        if (Settings.ServerDirectory.absolutePath().isEmpty())
        {
            ui->lblSrvDirError->setText("Directory is empty!");
            ui->lblSrvDirError->show();
            ui->lblTip->show();
            apply = false;
        }
        else
            ui->lineSrvDir->setText(Settings.ServerDirectory.absolutePath());
    }
    else if (ui->lineSrvDir->text().contains(" "))
    {
        ui->lblSrvDirError->setText("Directory can't have spaces!");
        ui->lblSrvDirError->show();
        ui->lblTip->show();
        apply = false;
    }

    if (apply)
    {
        if (!QDir(ui->lineSrvDir->text()).exists())
        {
            QDir dir;
            if (!dir.mkdir(ui->lineSrvDir->text()))
            {
                qInfo() << "There was an error creating server directory!";
                QMessageBox msgBox(QMessageBox::Icon::Critical, "Couldn't create server directory",
                                   tr("Couldn't create server directory.\nSelect a different one."), {}, this);
                msgBox.addButton("Ok", QMessageBox::ButtonRole::RejectRole);
                msgBox.exec();
                return;
            }
        }

        Settings.valid = true;
        Settings.ServerDirectory = ui->lineSrvDir->text();
        Settings.ColorTheme = colorTheme;

        IniSettings.setValue(QString("%0/server_directory").arg(OS), Settings.ServerDirectory.absolutePath());
        IniSettings.setValue(QString("%0/color_theme").arg(OS), Settings.ColorTheme);
        IniSettings.setValue(QString("%0/portForwardTip").arg(OS), IniSettings.value(QString("%0/portForwardTip"), false).toBool());

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

