#ifndef CONFIGCONVARDIALOG_H
#define CONFIGCONVARDIALOG_H

#include <QDialog>

namespace Ui {
class ConfigConVarDialog;
}

class ConfigConVarDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigConVarDialog(QStringList parentItems, QWidget *parent = nullptr);
    ~ConfigConVarDialog();

    QString selectedParentItem;
    int selectedIndex;

private slots:
    void on_btnSelect_clicked();

    void on_btnCancel_clicked();

private:
    Ui::ConfigConVarDialog *ui;
};

#endif // CONFIGCONVARDIALOG_H
