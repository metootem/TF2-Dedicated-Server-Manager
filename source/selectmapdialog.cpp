#include "selectmapdialog.h"
#include "ui_selectmapdialog.h"

SelectMapDialog::SelectMapDialog(QWidget *parent, QString path)
    : QDialog(parent)
    , ui(new Ui::SelectMapDialog)
{
    ui->setupUi(this);

    DirPath = path + "/Server/tf/maps";
    LoadAvailableMaps(DirPath);
    if (!MapsList.empty())
    {
        SortMapList();
    }
    else
    {
        ui->listMaps->addItem("No maps available.");
    }

    connect(ui->btnSelect, SIGNAL(clicked()), this, SLOT(accept()));
}

SelectMapDialog::~SelectMapDialog()
{
    delete ui;
}


void SelectMapDialog::LoadAvailableMaps(QString path)
{
    ui->listMaps->clear();
    ClearPrefixFilter();

    QFileInfoList fileList = QDir(path).entryInfoList(QStringList() << "*.bsp", QDir::Files);
    MapsList.clear();
    PrefixList.clear();
    NoPrefixMapList.clear();

    if (fileList.empty())
    {
        return;
    }

    QStringList mapList;
    QString mapName;
    QString currentPrefix;
    for (const QFileInfo &fileInfo : std::as_const(fileList))
    {
        mapName = fileInfo.baseName();
        FullMapList << mapName;

        QString prefix = GetMapPrefix(mapName);
        prefix = prefix.left(prefix.length()-1);

        if (prefix.isEmpty())
        {
            NoPrefixMapList << mapName;
            continue;
        }

        if (prefix != currentPrefix)
        {
            PrefixList << prefix;

            if (!currentPrefix.isEmpty())
            {
                MapsList << mapList;
            }

            mapList = QStringList();
            currentPrefix = prefix;
        }

        mapName = mapName.right(mapName.length()-prefix.length()-1);

        mapList << mapName;
    }

    MapsList << mapList; // append last set of maps.

    for (const QString &prefixFilter : std::as_const(PrefixList))
    {
        ui->cmbFilter->addItem(prefixFilter);
    }

    return;
}

void SelectMapDialog::SortMapList()
{
    ui->listMaps->clear();

    int indexFilter = ui->cmbFilter->currentIndex();

    QString textFilter = ui->lineFilter->text();

    if (!indexFilter) // Set to "All"
    {
        for (int i=0; i < PrefixList.count(); i++)
        {
            QString prefix = PrefixList.at(i);
            QListWidgetItem *item = new QListWidgetItem();
            item->setText(QString("%0 -----------").arg(prefix));
            item->setFlags(Qt::NoItemFlags);
            ui->listMaps->addItem(item);

            QStringList mapList = MapsList.at(i);
            for (const QString &mapName : std::as_const(mapList))
            {
                if (mapName.contains(textFilter))
                {
                    ui->listMaps->addItem(mapName);
                }
            }
        }

        if (NoPrefixMapList.isEmpty())
        {
            return;
        }

        QListWidgetItem *item = new QListWidgetItem();
        item->setText(tr("Unregistered -----------"));
        item->setFlags(Qt::NoItemFlags);
        ui->listMaps->addItem(item);

        for (const QString &mapName : std::as_const(NoPrefixMapList))
        {
            if (mapName.contains(textFilter))
            {
                ui->listMaps->addItem(mapName);
            }
        }

        return;
    }

    // Filter set
    indexFilter--;
    if (indexFilter < 0)
    {
        printInfo("indexFilter set to less than 0?");
        LoadAvailableMaps(DirPath);
        return;
    }

    QString prefix = PrefixList.at(indexFilter);
    QListWidgetItem *item = new QListWidgetItem();
    item->setText(QString("%0 -----------").arg(prefix));
    item->setFlags(Qt::NoItemFlags);
    ui->listMaps->addItem(item);

    QStringList mapList = MapsList.at(indexFilter);
    for (const QString &mapName : std::as_const(mapList))
    {
        if (mapName.contains(textFilter))
        {
            ui->listMaps->addItem(mapName);
        }
    }
}

QString SelectMapDialog::SelectMap()
{
    if (ui->listMaps->item(ui->listMaps->currentRow())->text().length() < 3)
    {
        return "";
    }
    if (ui->listMaps->item(ui->listMaps->currentRow())->text().first(3) == "No ")
    {
        return "";
    }

    QString mapName = ui->listMaps->currentItem()->text();
    for (const QString &fullMapName : std::as_const(FullMapList))
    {
        if (fullMapName.contains(mapName))
        {
            return fullMapName;
        }
    }

    return tr("(prefix)_%0").arg(mapName);
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

void SelectMapDialog::ClearPrefixFilter()
{
    QComboBox *cmbFilter = ui->cmbFilter;

    if (cmbFilter->currentIndex() <= -1)
    {
        if (!cmbFilter->count())
        {
            cmbFilter->addItem("All");
        }
        cmbFilter->setCurrentIndex(0);
    }

    if (cmbFilter->count() == 1)
    {
        return;
    }

    for (int i = cmbFilter->count()-1; i > 0; i--)
    {
        cmbFilter->removeItem(i);
    }
}


void SelectMapDialog::on_cmbFilter_currentIndexChanged()
{
    SortMapList();
}


void SelectMapDialog::on_lineFilter_textChanged()
{
    SortMapList();
}


void SelectMapDialog::on_btnRefresh_clicked()
{
    LoadAvailableMaps(DirPath);
    if (!MapsList.isEmpty())
    {
        SortMapList();
    }
    else
    {
        ui->listMaps->addItem("No available maps.");
    }
}

void SelectMapDialog::printInfo(const QString &message)
{
    qInfo() << "SelectMapDialog:" << message;
}
