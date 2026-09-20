#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent, QString appVersion, QString appDate)
    : QDialog(parent)
    , ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    QString title = ui->lblTitle->text();
    QString newTitle = title.replace("[VERSION]", appVersion);
    newTitle = newTitle.replace("[DATE]", appDate);
    ui->lblTitle->setText(newTitle);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
