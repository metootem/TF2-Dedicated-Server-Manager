#ifndef SELECTMAPDIALOG_H
#define SELECTMAPDIALOG_H

#include <QDialog>
#include <QDir>

namespace Ui {
class SelectMapDialog;
}

class SelectMapDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectMapDialog(QWidget *parent = nullptr, QString ServerFolder = "");

    void LoadAvailableMaps(QString path);
    QString SelectMap();

    ~SelectMapDialog();

private slots:

private:
    Ui::SelectMapDialog *ui;
};

#endif // SELECTMAPDIALOG_H
