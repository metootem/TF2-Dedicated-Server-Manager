#include "additionalparametersdialog.h"
#include "ui_additionalparametersdialog.h"

AdditionalParametersDialog::AdditionalParametersDialog(QWidget *parent, QSettings *IniSettings)
    : QDialog(parent)
    , ui(new Ui::AdditionalParametersDialog)
{
    ui->setupUi(this);
    if (IniSettings != nullptr)
    {
        this->setWindowTitle(IniSettings->value("server_name").toString() + " Additional Parameters");
        LoadParameters(IniSettings);
    }
    else
        this->setWindowTitle("New Additional Parameters Window");
    ui->treeParameters->setColumnWidth(0, 200);
    ui->treeParameters->setColumnWidth(1, 200);
}

AdditionalParametersDialog::~AdditionalParametersDialog()
{
    delete ui;
}

void AdditionalParametersDialog::FirstTimeSetup()
{
    AddParameter("+sv_pure", "0", Qt::Checked);
    AddParameter("-strictportbind", "", Qt::Unchecked);
}

void AdditionalParametersDialog::LoadParameters(QSettings *IniSettings)
{
    QStringList parameters = IniSettings->value("parameters").toStringList();
    if (parameters.count() % 3)
        return;

    for (int i = 0; i < parameters.count(); i+=3)
    {
        AddParameter(parameters[i], parameters[i+1], (parameters[i+2] == "True" ? Qt::Checked : Qt::Unchecked));
    }
}

QStringList AdditionalParametersDialog::GetParameters()
{
    QStringList list;
    for (int i = 0; i < ui->treeParameters->topLevelItemCount(); i++)
    {
        list << ui->treeParameters->topLevelItem(i)->text(0);
        list << ui->treeParameters->topLevelItem(i)->text(1);
        list << (ui->treeParameters->topLevelItem(i)->checkState(2) == Qt::Checked ? "True" : "False");
    }
    return list;
}

void AdditionalParametersDialog::AddParameter(QString param, QString val, Qt::CheckState state)
{
    QTreeWidgetItem *parameter = new QTreeWidgetItem(ui->treeParameters);
    parameter->setText(0, param);
    parameter->setText(1, val);
    parameter->setCheckState(2, state);
    parameter->setFlags(parameter->flags() | Qt::ItemIsEditable);
    ui->treeParameters->addTopLevelItem(parameter);
    qInfo() << "Adding parameter:" << param;
}

void AdditionalParametersDialog::on_btnAddParameter_clicked()
{
    AddParameter("+parameter", "", Qt::Checked);
}


void AdditionalParametersDialog::on_btnRemoveParameter_clicked()
{
    QTreeWidgetItem *item = ui->treeParameters->currentItem();
    delete item;
}

