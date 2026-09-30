#include "serverprops_config.h"
#include "ui_serverprops_config.h"

ServerProps_Config::ServerProps_Config(QWidget *parent, QString directory)
    : QWidget(parent)
    , ui(new Ui::ServerProps_Config)
{
    ui->setupUi(this);

    OS = QSysInfo::productType();
    if (OS != "windows" && OS != "macos")
    {
        OS = "linux";
    }

    ServerFolder = directory;
}

ServerProps_Config::~ServerProps_Config()
{
    delete ui;
}

void ServerProps_Config::LoadStyles(const QString colorTheme, const QString fullStyle)
{
    if (OS == "windows")
    {
        ui->cmbConfigFile->setStyleSheet("QComboBox {\n	\ncolor: #000000;\n}");
    }
    else if (OS == "linux")
    {
        ui->cmbConfigFile->setStyleSheet(QString("QComboBox {\n	background-color: %0;\ncolor: #ffffff;\n}").arg(colorTheme));
    }
}

void ServerProps_Config::ServerInstalled()
{
    QFile serverCfg(ServerFolder + "/Server/tf/cfg/server.cfg");
    if (!serverCfg.exists())
    {
        if (serverCfg.open(QIODevice::WriteOnly))
        {
            serverCfg.write(ServerCfgExample().toStdString().c_str());
            serverCfg.flush();
            serverCfg.close();
        }
    }

    CheckServerConfigFiles();
    SelectConfigFile("server.cfg");
}

void ServerProps_Config::CheckServerConfigFiles()
{
    QString text = GetSelectedConfigFile();

    ui->cmbConfigFile->clear();
    QFileInfoList fileList = QDir(ServerFolder + "/Server/tf/cfg").entryInfoList(QStringList() << "*.cfg" << "*.txt", QDir::Files);
    for (const QFileInfo &file : std::as_const(fileList))
    {
        ui->cmbConfigFile->addItem(file.fileName());
    }

    SelectConfigFile(text);
}

QString ServerProps_Config::GetSelectedConfigFile()
{
    return ui->cmbConfigFile->currentText();
}

void ServerProps_Config::SelectConfigFile(const QString file)
{
    ui->cmbConfigFile->setCurrentText(file);
}

void ServerProps_Config::LoadServerConfigFileData()
{

}

void ServerProps_Config::on_btnRefreshConfigList_clicked()
{
    CheckServerConfigFiles();
}

void ServerProps_Config::on_btnAddConVar_clicked()
{
    QStringList parentItems;
    for (int i=0; i<ui->treeConfigFileData->topLevelItemCount(); i++)
    {
        QString name = ui->treeConfigFileData->topLevelItem(i)->text(0);
        if (name.first(2) == "//" && name.last(2) == "//" && name.length() > 3)
        {
            parentItems << name;
        }
    }

    auto dialog = new ConfigConVarDialog(parentItems, this);
    int code = dialog->exec();
    int selectedParentItemIndex = dialog->selectedIndex;
    dialog->deleteLater();

    if (!selectedParentItemIndex && code)
        AddConfigTreeItem("NewConVar", "Value", "");
    else if (code)
        AddConfigTreeItem("NewConvar", "Value", "", ui->treeConfigFileData->topLevelItem(selectedParentItemIndex-1));
}

void ServerProps_Config::on_btnDelConVar_clicked()
{
    auto item = ui->treeConfigFileData->currentItem();
    if (item != nullptr)
    {
        if (!item->childCount())
            delete item;
        else
        {
            QMessageBox msgBox(QMessageBox::Icon::Warning, "",
                               tr("Selection has multiple child convars.\nProceed to delete?"), {}, this);
            auto *accept = msgBox.addButton("Accept", QMessageBox::ButtonRole::AcceptRole);
            msgBox.addButton("Cancel", QMessageBox::ButtonRole::RejectRole);
            msgBox.exec();
            if (msgBox.clickedButton() == accept)
                delete item;
        }
    }
}

void ServerProps_Config::on_btnReloadConfig_clicked()
{
    on_cmbConfigFile_currentTextChanged(ui->cmbConfigFile->currentText());
}

void ServerProps_Config::on_btnOpenConfig_clicked()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(ServerFolder + "/Server/tf/cfg/" + ui->cmbConfigFile->currentText()));
}

void ServerProps_Config::on_btnConfigSpecial_clicked()
{
    QString fileName = ui->cmbConfigFile->currentText();
    if (fileName.length() > 8)
    {
        if (fileName.first(8) == "mapcycle")
        {
            //ui->treeConfigFileData->clear();

            QStringList parentItems;
            for (int i=0; i<ui->treeConfigFileData->topLevelItemCount(); i++)
            {
                QString displayItem = ui->treeConfigFileData->topLevelItem(i)->text(0);
                parentItems << displayItem;
            }

            auto mapsDialog = new Cfg_LoadMapsDialog(ServerFolder + "/Server/tf/maps", this);
            if (mapsDialog->exec() == QDialog::Accepted)
            {
                QStringList newMapList = mapsDialog->ReturnMaps();
                for (const QString &map : std::as_const(newMapList))
                {
                    if (!parentItems.contains(map))
                    {
                        AddConfigTreeItem(map, "", "");
                    }
                }
                delete mapsDialog;
            }
            else
                return;

            /*for (QFileInfo fileInfo : QDir(ServerFolder + "/Server/tf/maps").entryInfoList(QStringList() << "*.bsp", QDir::Files))
            {
                qInfo() << fileInfo.fileName().first(fileInfo.fileName().length()-4);
                if (!parentItems.contains(fileInfo.fileName()))
                    AddConfigTreeItem(fileInfo.fileName().first(fileInfo.fileName().length()-4), "", "");
            }*/
        }
    }
}

void ServerProps_Config::on_btnFindConVar_clicked()
{
    if (!ui->treeConfigFileData->topLevelItemCount())
        return;

    ui->treeConfigFileData->clearSelection();
    QString target = QInputDialog::getText(this, "Find ConVar", "ConVar to find:");

    if (target.isEmpty())
        return;

    for (int i=0; i<ui->treeConfigFileData->topLevelItemCount(); i++)
    {
        auto topItem = ui->treeConfigFileData->topLevelItem(i);
        if (topItem->text(0).contains(target))
        {
            ui->treeConfigFileData->scrollToItem(topItem);
            topItem->setSelected(true);
            return;
        }

        for (int j=0;j<topItem->childCount(); j++)
        {
            auto childItem = topItem->child(j);
            if (childItem->text(0).contains(target))
            {
                ui->treeConfigFileData->expandItem(topItem);
                ui->treeConfigFileData->scrollToItem(childItem);
                childItem->setSelected(true);
                return;
            }
        }
    }

    QMessageBox msgBox(QMessageBox::Icon::Critical, "",
                       tr("Couldn't find ConVar '%0'.").arg(target), {}, this);
    msgBox.addButton("Ok", QMessageBox::ButtonRole::AcceptRole);
    msgBox.exec();
}

void ServerProps_Config::on_btnNewConfigFile_clicked()
{
    bool ok;
    QString name = QInputDialog::getText(this, "Create New Config File",
                                         "File Name (without type):", QLineEdit::Normal,
                                         "", &ok);

    if (!ok || name.isEmpty() || name.contains(":") || name.contains(" ") || name.contains("."))
        return;

    QString ext = QInputDialog::getItem(this, "Choose File Type", "File Type:", QStringList() << ".txt" << ".cfg", 0, false, &ok);
    if (!ok || ext.isEmpty())
        return;

    QFile file(ServerFolder + "/Server/tf/cfg/" + name + ext);
    if (file.open(QIODevice::WriteOnly))
    {
        file.close();

        CheckServerConfigFiles();

        ui->cmbConfigFile->setCurrentText(name + ext);
    }
}

void ServerProps_Config::on_btnSaveConfig_clicked()
{
    QString fileName = ui->cmbConfigFile->currentText();
    QFile file (ServerFolder + "/Server/tf/cfg/" + fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        qInfo() << fileName;

        int topLevelCount = ui->treeConfigFileData->topLevelItemCount();
        QString output;
        if (fileName.length() > 4)
        {
            if (fileName.first(4) == "motd")
            {
                qInfo() << "motd";
                output = ui->txtConfigFileData->toPlainText();
                file.write(output.toStdString().c_str());

                file.flush();
                file.close();

                emit SystemNotification("Saved config", fileName, 3000);
                return;
            }
        }

        if (fileName.length() > 8)
        {
            if (fileName.first(8) == "mapcycle")
            {
                qInfo() << "mapcycle";
                int topLevelCount = ui->treeConfigFileData->topLevelItemCount();

                for (int i=0; i<topLevelCount; i++)
                {
                    QTreeWidgetItem *topItem = ui->treeConfigFileData->topLevelItem(i);

                    QString comment;
                    if (!topItem->toolTip(0).isEmpty())
                    {
                        QString string = topItem->toolTip(0);
                        QTextStream in(&string);
                        while (!in.atEnd())
                            comment += "//" + in.readLine() + "\n";
                    }

                    output += (i > 0 ? "\n" : "") + comment + (topItem->text(0) == "//" ? "" : topItem->text(0));
                }
                file.write(output.toStdString().c_str());
                file.flush();
                file.close();
                emit SystemNotification("Saved config", fileName, 3000);
                return;
            }
        }

        for (int i=0; i<topLevelCount; i++)
        {
            QTreeWidgetItem *topItem = ui->treeConfigFileData->topLevelItem(i);

            QString comment;
            if (!topItem->toolTip(0).isEmpty())
            {
                QString string = topItem->toolTip(0);
                QTextStream in(&string);
                while (!in.atEnd())
                    comment += "//" + in.readLine() + "\n";
            }

            output += comment + (topItem->text(0) == "//" ? "" : topItem->text(0)) + (!topItem->text(1).isEmpty() ? " " + topItem->text(1) : "") + "\n";

            for (int j=0; j<topItem->childCount(); j++)
            {
                QTreeWidgetItem *childItem = topItem->child(j);

                comment.clear();
                if (!childItem->toolTip(0).isEmpty())
                {
                    //comment = "\n";
                    QString string = childItem->toolTip(0);
                    QTextStream in(&string);
                    while (!in.atEnd())
                        comment += "//" + in.readLine() + "\n";
                }

                output += comment + (childItem->text(0) == "//" ? "" : childItem->text(0)) + " " + childItem->text(1) + "\n" + (!comment.isEmpty() ? "\n" : "");
            }
        }

        file.write(output.toStdString().c_str());

        file.flush();
        file.close();

        emit SystemNotification("Saved config", fileName, 3000);
    }
}

void ServerProps_Config::on_btnClearConfig_clicked()
{
    ui->treeConfigFileData->clear();
    ui->txtConfigFileData->clear();
}

void ServerProps_Config::on_cmbConfigFile_currentTextChanged(const QString &arg1)
{
    ui->treeConfigFileData->clear();
    ui->txtConfigFileData->clear();

    QFile file(ServerFolder + "/Server/tf/cfg/" + arg1);
    if (file.open(QIODevice::ReadOnly))
    {
        if (arg1.length() < 8)
        {
            ui->treeConfigFileData->show();
            ui->txtConfigFileData->hide();

            ui->treeConfigFileData->setColumnCount(2);
            ui->treeConfigFileData->setHeaderLabels(QStringList() << "ConVar" << "Value");
            ui->treeConfigFileData->setColumnWidth(0, 200);

            ui->btnAddConVar->setEnabled(true);
            ui->btnDelConVar->setEnabled(true);
            ui->btnConfigSpecial->setEnabled(false);
            ui->btnConfigSpecial->hide();
        }
        else if (arg1.first(4) == "motd")
        {
            ui->treeConfigFileData->hide();
            ui->txtConfigFileData->show();

            ui->btnAddConVar->setEnabled(false);
            ui->btnDelConVar->setEnabled(false);
            ui->btnConfigSpecial->setEnabled(false);
            ui->btnConfigSpecial->hide();

            ui->txtConfigFileData->appendPlainText(file.readAll());
            file.close();
            ui->txtConfigFileData->verticalScrollBar()->setSliderPosition(ui->txtConfigFileData->verticalScrollBar()->minimum());
            ui->txtConfigFileData->horizontalScrollBar()->setSliderPosition(ui->txtConfigFileData->horizontalScrollBar()->minimum());
            return;
        }
        else if (arg1.first(8) == "mapcycle")
        {
            ui->treeConfigFileData->show();
            ui->txtConfigFileData->hide();

            ui->treeConfigFileData->setColumnCount(1);
            ui->treeConfigFileData->setHeaderLabel("Map");

            ui->btnAddConVar->setEnabled(true);
            ui->btnDelConVar->setEnabled(true);
            ui->btnConfigSpecial->setEnabled(true);
            ui->btnConfigSpecial->show();

            ui->btnConfigSpecial->setText("Load Maps");
            ui->btnConfigSpecial->setToolTip("Load all .bsp files from the maps folder.");
        }
        else
        {
            ui->treeConfigFileData->show();
            ui->txtConfigFileData->hide();

            ui->treeConfigFileData->setColumnCount(2);
            ui->treeConfigFileData->setHeaderLabels(QStringList() << "ConVar" << "Value");
            ui->treeConfigFileData->setColumnWidth(0, 200);

            ui->btnAddConVar->setEnabled(true);
            ui->btnDelConVar->setEnabled(true);
            ui->btnConfigSpecial->setEnabled(false);
            ui->btnConfigSpecial->hide();
        }

        int parentItemIndex = -1;
        QStringList conVarCommentLines;

        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            if (line.isEmpty())
            {
                if (!conVarCommentLines.isEmpty())
                {
                    QString toolTip;
                    for (const QString &comment : std::as_const(conVarCommentLines))
                    {
                        toolTip += comment;
                    }

                    AddConfigTreeItem("//", "", toolTip, (parentItemIndex >= 0 ? ui->treeConfigFileData->topLevelItem(parentItemIndex) : nullptr));
                }
                conVarCommentLines.clear();
            }
            if (line.length() < 2)
                continue;
            else if (line.first(2) == "//" && line.last(2) == "//" && line.length() > 2)
            {
                QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeConfigFileData);
                item->setText(0, line);

                item->setFlags(item->flags() | Qt::ItemIsEditable);
                ui->treeConfigFileData->addTopLevelItem(item);

                parentItemIndex++;
                ui->treeConfigFileData->setIndentation(20);
            }
            else if (line.first(2) == "//")
            {
                conVarCommentLines << (conVarCommentLines.count() > 0 ? "\n" : "") << line.last(line.length()-2);
            }
            else// if (line.first(2) != "//")
            {
                QString conVar;
                int charCount = 0;
                for (const QChar &c : std::as_const(line))
                {
                    charCount++;
                    if (c == ' ')
                        break;
                    conVar += c;
                }

                QString toolTip;
                for (const QString &comment : std::as_const(conVarCommentLines))
                {
                    toolTip += comment;
                }

                AddConfigTreeItem(conVar, line.last(line.length()-charCount), toolTip, (parentItemIndex >= 0 ? ui->treeConfigFileData->topLevelItem(parentItemIndex) : nullptr));

                conVarCommentLines.clear();
            }
        }
        file.close();

        if (parentItemIndex < 0)
            ui->treeConfigFileData->setIndentation(0);
    }
}

void ServerProps_Config::AddConfigTreeItem(QString ConVar, QString Value, QString Comment, QTreeWidgetItem* parent)
{
    if (parent == nullptr)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeConfigFileData);
        item->setText(0, ConVar);
        item->setText(1, Value);
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        if (!Comment.isEmpty())
        {
            item->setToolTip(0, Comment);
            item->setToolTip(1, Comment);
        }

        ui->treeConfigFileData->addTopLevelItem(item);
    }
    else
    {
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, ConVar);
        item->setText(1, Value);
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        if (!Comment.isEmpty())
        {
            item->setToolTip(0, Comment);
            item->setToolTip(1, Comment);
        }

        parent->addChild(item);
    }
}

QString ServerProps_Config::ServerCfgExample()
{
    return QString("// General Settings //\n"
                   "\n"
                   "// Overrides the max players reported to prospective clients\n"
                   "sv_visiblemaxplayers 32\n"
                   "\n"
                   "// Maximum number of rounds to play before server changes maps\n"
                   "mp_maxrounds 5\n"
                   "\n"
                   "// Set to lock per-frame time elapse\n"
                   "host_framerate 0\n"
                   "\n"
                   "// Set the pause state of the server\n"
                   "setpause 0\n"
                   "\n"
                   "// Control where the client gets content from\n"
                   "// 0 = anywhere, 1 = anywhere listed in white list, 2 = steam official content only\n"
                   "sv_pure 0\n"
                   "\n"
                   "// Is the server pausable\n"
                   "sv_pausable 0\n"
                   "\n"
                   "// Type of server 0=internet 1=lan\n"
                   "sv_lan 0\n"
                   "\n"
                   "// Collect CPU usage stats\n"
                   "sv_stats 1\n"
                   "\n"
                   "\n"
                   "\n"
                   "// Execute Banned Users //\n"
                   "exec banned_user.cfg\n"
                   "exec banned_ip.cfg\n"
                   "writeid\n"
                   "writeip\n"
                   "\n"
                   "\n"
                   "\n"
                   "// Contact & Region //\n"
                   "\n"
                   "// Contact email for server sysop\n"
                   "sv_contact emailaddy@google.com\n"
                   "\n"
                   "// The region of the world to report this server in.\n"
                   "// -1 is the world, 0 is USA east coast, 1 is USA west coast\n"
                   "// 2 south america, 3 europe, 4 asia, 5 australia, 6 middle east, 7 africa\n"
                   "sv_region -1\n"
                   "\n"
                   "\n"
                   "\n"
                   "// Rcon Settings //\n"
                   "\n"
                   "// Password for rcon authentication (Remote CONtrol)\n"
                   "rcon_password yourpw\n"
                   "\n"
                   "// Number of minutes to ban users who fail rcon authentication\n"
                   "sv_rcon_banpenalty 1440\n"
                   "\n"
                   "// Max number of times a user can fail rcon authentication before being banned\n"
                   "sv_rcon_maxfailures 5\n"
                   "\n"
                   "\n"
                   "\n"
                   "// Log Settings //\n"
                   "\n"
                   "// Enables logging to file, console, and udp < on | off >.\n"
                   "log on\n"
                   "\n"
                   "// Log server information to only one file.\n"
                   "sv_log_onefile 0\n"
                   "\n"
                   "// Log server information in the log file.\n"
                   "sv_logfile 1\n"
                   "\n"
                   "// Log server bans in the server logs.\n"
                   "sv_logbans 1\n"
                   "\n"
                   "// Echo log information to the console.\n"
                   "sv_logecho 1\n"
                   "\n"
                   "\n"
                   "\n"
                   "// Rate Settings //\n"
                   "\n"
                   "// Frame rate limiter\n"
                   "fps_max 600\n"
                   "\n"
                   "// Min bandwidth rate allowed on server, 0 == unlimited\n"
                   "sv_minrate 0\n"
                   "\n"
                   "// Max bandwidth rate allowed on server, 0 == unlimited\n"
                   "sv_maxrate 20000\n"
                   "\n"
                   "// Minimum updates per second that the server will allow\n"
                   "sv_minupdaterate 10\n"
                   "\n"
                   "// Maximum updates per second that the server will allow\n"
                   "sv_maxupdaterate 66\n"
                   "\n"
                   "\n"
                   "\n"
                   "// Download Settings //\n"
                   "\n"
                   "// Allow clients to upload customizations files\n"
                   "sv_allowupload 1\n"
                   "\n"
                   "// Allow clients to download files\n"
                   "sv_allowdownload 1\n"
                   "\n"
                   "// Maximum allowed file size for uploading in MB\n"
                   "net_maxfilesize 15\n"
                   "\n"
                   "\n"
                   "\n"
                   "// Team Balancing //\n"
                   "\n"
                   "// Enable team balancing\n"
                   "mp_autoteambalance 1\n"
                   "\n"
                   "// Time after the teams become unbalanced to attempt to switch players.\n"
                   "mp_autoteambalance_delay 60\n"
                   "\n"
                   "// Time after the teams become unbalanced to print a balance warning\n"
                   "mp_autoteambalance_warning_delay 30\n"
                   "\n"
                   "// Teams are unbalanced when one team has this many more players than the other team. (0 disables check)\n"
                   "mp_teams_unbalance_limit 1\n"
                   "\n"
                   "\n"
                   "\n"
                   "// Round and Game Times //\n"
                   "\n"
                   "// Enable timers to wait between rounds. WARNING: Setting this to 0 has been known to cause a bug with setup times lasting 5:20 (5 minutes 20 seconds) on some servers!\n"
                   "mp_enableroundwaittime 1\n"
                   "\n"
                   "// Time after round win until round restarts\n"
                   "mp_bonusroundtime 8\n"
                   "\n"
                   "// If non-zero, the current round will restart in the specified number of seconds\n"
                   "mp_restartround 0\n"
                   "\n"
                   "// Enable sudden death\n"
                   "mp_stalemate_enable 1\n"
                   "\n"
                   "// Timelimit (in seconds) of the stalemate round.\n"
                   "mp_stalemate_timelimit 300\n"
                   "\n"
                   "// Game time per map in minutes\n"
                   "mp_timelimit 35\n"
                   "\n"
                   "\n"
                   "\n"
                   "// Client CVars //\n"
                   "\n"
                   "// Restricts spectator modes for dead players\n"
                   "mp_forcecamera 0\n"
                   "\n"
                   "// Toggles whether the server allows spectator mode or not\n"
                   "mp_allowspectators 1\n"
                   "\n"
                   "// Toggles footstep sounds\n"
                   "mp_footsteps 1\n"
                   "\n"
                   "// Toggles game cheats\n"
                   "sv_cheats 0\n"
                   "\n"
                   "// Time it takes for players to auto-disconnect if your server stops responding.\n"
                   "sv_timeout 60\n"
                   "\n"
                   "// Maximum time a player is allowed to be idle, in minutes.\n"
                   "mp_idlemaxtime 5\n"
                   "\n"
                   "// Deals with idle players 1=send to spectator 2=kick\n"
                   "mp_idledealmethod 2\n"
                   "\n"
                   "// Time (seconds) between decal sprays\n"
                   "decalfrequency 30\n"
                   "\n"
                   "\n"
                   "\n"
                   "// Communications //\n"
                   "\n"
                   "// enable voice communications\n"
                   "sv_voiceenable 1\n"
                   "\n"
                   "// Players can hear all other players, no team restrictions 0=off 1=on\n"
                   "sv_alltalk 0\n"
                   "\n"
                   "// Amount of time players can chat after the game is over\n"
                   "mp_chattime 10\n"
                   "\n"
                   "// Enable party mode\n"
                   "tf_birthday 0\n");
}