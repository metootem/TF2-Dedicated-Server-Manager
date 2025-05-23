#include "cfg_loadmapsdialog.h"
#include "ui_cfg_loadmapsdialog.h"

Cfg_LoadMapsDialog::Cfg_LoadMapsDialog(QString mapDir, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Cfg_LoadMapsDialog)
{
    ui->setupUi(this);

    MapDir = mapDir;

    LoadMaps(QStringList());

    connect(ui->btnConfirm, SIGNAL(clicked()), this, SLOT(accept()));
}

Cfg_LoadMapsDialog::~Cfg_LoadMapsDialog()
{
    delete ui;
}

QStringList Cfg_LoadMapsDialog::ReturnMaps()
{
    QStringList mapList;
    for (int i=0; i<ui->listMaps->count(); i++)
    {
        auto item = ui->listMaps->item(i);
        mapList << item->text();
    }
    return mapList;
}

void Cfg_LoadMapsDialog::LoadMaps(QStringList prefixFilter)
{
    if (prefixFilter.isEmpty())
        prefixFilter << "*.bsp";

    QStringList mapList;
    for (int i=0; i<ui->listMaps->count(); i++)
        mapList << ui->listMaps->item(i)->text();

    for (QFileInfo fileInfo : QDir(MapDir).entryInfoList(prefixFilter, QDir::Files))
    {
        QString file = fileInfo.fileName().first(fileInfo.fileName().length()-4);
        if (!mapList.contains(file))
            AddMapToList(file);
    }
}

QStringList Cfg_LoadMapsDialog::GetPrefixFilter()
{
    QStringList filterList;
    for (int i=0; i<ui->listPrefix->count(); i++)
        filterList << ui->listPrefix->item(i)->text() + "_*.bsp";
    return filterList;
}

void Cfg_LoadMapsDialog::AddMapToList(QString map)
{
    if (map.isEmpty())
        return;

    auto item = new QListWidgetItem(ui->listMaps);
    item->setText(map);
    ui->listMaps->addItem(item);
}

void Cfg_LoadMapsDialog::on_btnAddSrv_clicked()
{
    QDir srvFolder(MapDir);
    for (int i=0; i<3; i++)
        srvFolder.cdUp();

    auto selectMapDialog = new SelectMapDialog(this, srvFolder.path());
    if (selectMapDialog->exec() == QDialog::Accepted)
        AddMapToList(selectMapDialog->SelectMap());
}


void Cfg_LoadMapsDialog::on_btnRmvSrv_clicked()
{
    for (auto item : ui->listMaps->selectedItems())
        delete item;
}


void Cfg_LoadMapsDialog::on_btnRfshPrefix_clicked()
{
    ui->listMaps->clear();
    LoadMaps(GetPrefixFilter());
}


void Cfg_LoadMapsDialog::on_btnClearMapList_clicked()
{
    ui->listMaps->clear();
}


void Cfg_LoadMapsDialog::on_btnAddPrefix_clicked()
{
    bool ok;
    QString prefix = QInputDialog::getText(this, "Add Prefix Filter", "Map prefix (without underscore):", QLineEdit::Normal, "", &ok);

    if (!ok || prefix.isEmpty())
        return;

    auto item = new QListWidgetItem(ui->listPrefix);
    item->setText(prefix);
    ui->listPrefix->addItem(item);
    LoadMaps(QStringList() << prefix+"_*.bsp");
}


void Cfg_LoadMapsDialog::on_btnRmvPrefix_clicked()
{
    bool remove = false;
    QMessageBox msgBox(QMessageBox::Icon::Warning, "",
                       tr("Remove associated maps from the list?"), {}, this);
    auto accept = msgBox.addButton("Yes", QMessageBox::ButtonRole::AcceptRole);
    msgBox.addButton("No", QMessageBox::ButtonRole::RejectRole);
    msgBox.exec();
    if (msgBox.clickedButton() == accept)
        remove = true;

    for (auto item : ui->listPrefix->selectedItems())
    {
        if (remove)
        {
            QString prefix = item->text() + "_";
            for (int i=0; i<ui->listMaps->count(); i++)
            {
                auto mapItem = ui->listMaps->item(i);
                if (mapItem->text().length() >= prefix.length())
                {
                    if (mapItem->text().first(prefix.length()) == prefix)
                    {
                        delete mapItem;
                        i--;
                    }
                }
            }
        }

        delete item;
    }
}

