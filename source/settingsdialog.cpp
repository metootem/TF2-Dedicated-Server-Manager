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
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

SettingsStruct SettingsDialog::ParseSettings()
{
    Settings.valid = true;
    Settings.OS = OS;

    if (!IniSettings.contains(QString("%0/color_theme").arg(OS)))
        IniSettings.setValue(QString("%0/color_theme").arg(OS), "#cf6a32");

    if (!IniSettings.contains(tr("%0/server_directories").arg(OS)))
    {
        Settings.valid = false;
        ui->lblTip->show();
    }
    else
    {
        QStringList dirList = IniSettings.value(QString("%0/server_directories").arg(OS)).toStringList();
        if (dirList.isEmpty())
        {
            Settings.valid = false;
            ui->lblSrvDirError->setText("At least one directory is required!");
            ui->lblSrvDirError->show();
        }
        else
        {
            for (QString strDir : dirList)
            {
                /*if (strDir.contains(" "))
                {
                    Settings.valid = false;
                    ui->lblSrvDirError->setText("Directory can't have spaces!");
                    ui->lblSrvDirError->show();
                }
                else
                {*/
                    auto item = new QTreeWidgetItem(ui->treeSrvDirs);
                    item->setText(0, strDir);
                    item->setFlags(item->flags() | Qt::ItemIsEditable);
                    ui->treeSrvDirs->addTopLevelItem(item);
               //}
            }
        }
    }

    Settings.ColorTheme = IniSettings.value(QString("%0/color_theme").arg(OS)).toString();
    Settings.ServerDirectories = IniSettings.value(QString("%0/server_directories").arg(OS)).toStringList();
    //Settings.PublicIP = GetPublicIP();

    colorTheme = Settings.ColorTheme;

    ui->btnColor->setStyleSheet(QString("border: 2px solid #232323; border-radius: 0px; background-color: %0; color: #ffffff;").arg(colorTheme));

    return Settings;
}

void SettingsDialog::on_btnApply_clicked()
{
    bool apply = true;
    ui->lblSrvDirError->hide();
    ui->lblApplySuccess->hide();
    ui->lblTip->hide();

    QStringList dirList;
    for (int i=0; i<ui->treeSrvDirs->topLevelItemCount(); i++)
    {
        auto item = ui->treeSrvDirs->topLevelItem(i);
        QString dir = item->text(0);
        if (!dir.isEmpty())
            dirList << dir;
        else
        {
            qInfo() << dir;
            ui->lblSrvDirError->setText("Directory is invalid!");
            ui->lblSrvDirError->show();
            ui->lblTip->show();
            apply = false;
        }
    }

    if (dirList.isEmpty())
    {
        ui->lblSrvDirError->setText("No directory specified!");
        ui->lblSrvDirError->show();
        ui->lblTip->show();
        apply = false;
    }

    if (apply)
    {
        for (QString strDir : dirList)
        {
            if (!QDir(strDir).exists())
            {
                QMessageBox msgBox(QMessageBox::Icon::Question, "Directory doesn't exist",
                                   tr("Directory '%0' doesn't exist.\n"
                                      "Do you want to create it?").arg(strDir), {}, this);
                auto *accept = msgBox.addButton("Accept", QMessageBox::ButtonRole::AcceptRole);
                msgBox.addButton("Ignore", QMessageBox::ButtonRole::RejectRole);
                msgBox.exec();
                if (msgBox.clickedButton() == accept)
                {
                    QDir dir;
                    if (!dir.mkdir(strDir))
                    {
                        qInfo() << "There was an error creating server directory!";
                        QMessageBox msgBox(QMessageBox::Icon::Critical, "Couldn't create server directory",
                                           tr("Couldn't create server directory.\nSelect a different one."), {}, this);
                        msgBox.addButton("Ok", QMessageBox::ButtonRole::RejectRole);
                        msgBox.exec();
                        return;
                    }
                }
            }
        }

        Settings.valid = true;
        Settings.ColorTheme = colorTheme;
        Settings.ServerDirectories = dirList;

        IniSettings.setValue(QString("%0/server_directories").arg(OS), Settings.ServerDirectories);
        IniSettings.setValue(QString("%0/color_theme").arg(OS), Settings.ColorTheme);
        IniSettings.setValue(QString("%0/portForwardTip").arg(OS), IniSettings.value(QString("%0/portForwardTip"), false).toBool());

        emit SettingsChanged(Settings);
        ui->lblApplySuccess->show();
    }
}


void SettingsDialog::on_btnAddSrvDir_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Open Directory", QDir::currentPath(), QFileDialog::ShowDirsOnly);
    if (!dir.isEmpty())
    {
        auto item = new QTreeWidgetItem(ui->treeSrvDirs);
        item->setText(0, dir);
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->treeSrvDirs->addTopLevelItem(item);
    }
}


void SettingsDialog::on_btnRmvSrvDir_clicked()
{
    delete ui->treeSrvDirs->currentItem();
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

