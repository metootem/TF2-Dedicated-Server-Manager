#ifndef ADDITIONALPARAMETERSDIALOG_H
#define ADDITIONALPARAMETERSDIALOG_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class AdditionalParametersDialog;
}

class AdditionalParametersDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AdditionalParametersDialog(QWidget *parent = nullptr, QSettings *IniSettings = nullptr);

    void LoadParameters( QSettings* );
    void FirstTimeSetup();
    QStringList GetParameters();

    ~AdditionalParametersDialog();

private slots:
    void on_btnAddParameter_clicked();
    void AddParameter(QString param, QString val, Qt::CheckState state);

    void on_btnRemoveParameter_clicked();

private:
    Ui::AdditionalParametersDialog *ui;
};

#endif // ADDITIONALPARAMETERSDIALOG_H
