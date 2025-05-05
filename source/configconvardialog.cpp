#include "configconvardialog.h"
#include "ui_configconvardialog.h"

ConfigConVarDialog::ConfigConVarDialog(QStringList parentItems, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ConfigConVarDialog)
{
    ui->setupUi(this);

    new QListWidgetItem("New ConVar", ui->listParentConVars);
    for(QString name : parentItems)
    {
        new QListWidgetItem(name, ui->listParentConVars);
    }
}

ConfigConVarDialog::~ConfigConVarDialog()
{
    delete ui;
}

void ConfigConVarDialog::on_btnSelect_clicked()
{
    selectedParentItem = ui->listParentConVars->currentItem()->text();
    selectedIndex = ui->listParentConVars->currentIndex().row();
    this->accept();
}


void ConfigConVarDialog::on_btnCancel_clicked()
{
    this->reject();
}

