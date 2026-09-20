#include "selectmapdialog.h"
#include "ui_selectmapdialog.h"

SelectMapDialog::SelectMapDialog(QWidget *parent, QString path)
    : QDialog(parent)
    , ui(new Ui::SelectMapDialog)
{
    ui->setupUi(this);

    DirPath = path + "/Server/tf/maps";
    LoadAvailableMaps(DirPath);

    connect(ui->btnSelect, SIGNAL(clicked()), this, SLOT(accept()));
}

SelectMapDialog::~SelectMapDialog()
{
    delete ui;
}


void SelectMapDialog::LoadAvailableMaps(QString path)
{
    ui->listMaps->clear();
    QFileInfoList fileList = QDir(path).entryInfoList(QStringList() << "*.bsp", QDir::Files);

    if (fileList.empty())
    {
        ui->listMaps->addItem("No maps available.");
        return;
    }

    bool mapFilterEmpty = ui->cmbFilter->count() == 1;
    bool mapListEmpty = MapList.empty();

    QString curPrefix = "";
    QString filter = ui->lineFilter->text();
    for (const QFileInfo &fileInfo : std::as_const(fileList))
    {
        QString mapName = fileInfo.baseName();
        if (mapListEmpty)
        {
            MapList << mapName;
            qInfo() << "appending" << mapName;
        }
        QString prefix = GetMapPrefix(mapName);
        prefix = prefix.left(prefix.length()-1);

        if (prefix.isEmpty())
        {
            NoPrefixMapList << mapName;
            continue;
        }

        if (curPrefix != prefix)
        {
            if (mapFilterEmpty)
            {
                ui->cmbFilter->addItem(prefix);
            }
            else if (ui->cmbFilter->currentText() != prefix && ui->cmbFilter->currentText() != "All")
            {
                continue;
            }

            curPrefix = prefix;

            QListWidgetItem *item = new QListWidgetItem();
            item->setText(tr("%0 -----------").arg(prefix));
            item->setFlags(Qt::NoItemFlags);
            ui->listMaps->addItem(item);
        }

        QString displayName = mapName.right(mapName.length()-prefix.length()-1);

        if (displayName.contains(filter))
        {
            ui->listMaps->addItem(displayName);
        }
    }

    if (ui->cmbFilter->currentIndex())
    {
        NoPrefixMapList.clear();
        return;
    }

    if (!NoPrefixMapList.isEmpty())
    {
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(tr("Unregistered -----------"));
        item->setFlags(Qt::NoItemFlags);
        ui->listMaps->addItem(item);
    }
    for (const QString &displayName : std::as_const(NoPrefixMapList))
    {
        ui->listMaps->addItem(displayName);
    }
}

QString SelectMapDialog::GetMapPrefix(QString mapName)
{
    int index = 1;
    for (const QChar &ch : std::as_const(mapName))
    {
        if (ch == '_')
        {
            return mapName.left(index);
            break;
        }
        index++;
    }
    return "";
}

QString SelectMapDialog::SelectMap()
{
    if (ui->listMaps->item(ui->listMaps->currentRow())->text().length() < 3)
        return "";
    if (ui->listMaps->item(ui->listMaps->currentRow())->text().first(3) == "No ")
        return "";

    QString mapName = ui->listMaps->currentItem()->text();
    for (const QString &fullMapName : std::as_const(MapList))
    {
        if (fullMapName.contains(mapName))
        {
            return fullMapName;
        }
    }

    return tr("(prefix)_%0").arg(mapName);
}


void SelectMapDialog::on_cmbFilter_currentIndexChanged()
{
    LoadAvailableMaps(DirPath);
}


void SelectMapDialog::on_lineFilter_textChanged()
{
    LoadAvailableMaps(DirPath);
}

