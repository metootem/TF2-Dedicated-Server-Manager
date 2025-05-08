#include "selectmapdialog.h"
#include "ui_selectmapdialog.h"

SelectMapDialog::SelectMapDialog(QWidget *parent, QString path)
    : QDialog(parent)
    , ui(new Ui::SelectMapDialog)
{
    ui->setupUi(this);
    LoadAvailableMaps(path + "/Server/tf/maps");
    NoMaps = true;

    connect(ui->btnSelect, SIGNAL(clicked()), this, SLOT(accept()));
}

SelectMapDialog::~SelectMapDialog()
{
    delete ui;
}


void SelectMapDialog::LoadAvailableMaps(QString path)
{
    for (QFileInfo fileInfo : QDir(path).entryInfoList(QStringList() << "*.bsp", QDir::Files))
    {
        ui->listMaps->addItem(fileInfo.baseName());
        NoMaps = false;
    }
    if (!ui->listMaps->count())
        ui->listMaps->addItem("No maps available.");
}


QString SelectMapDialog::SelectMap()
{
    if (ui->listMaps->item(ui->listMaps->currentRow())->text().length() < 3)
        return "";
    return (ui->listMaps->item(ui->listMaps->currentRow())->text().first(3) == "No " ? "" : ui->listMaps->item(ui->listMaps->currentRow())->text());
}

