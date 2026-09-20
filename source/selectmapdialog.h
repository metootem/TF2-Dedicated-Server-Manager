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
    QString GetMapPrefix(QString mapName);
    QString SelectMap();

    ~SelectMapDialog();

private slots:

    void on_cmbFilter_currentIndexChanged();

    void on_lineFilter_textChanged();

private:
    Ui::SelectMapDialog *ui;

    QString DirPath;
    QStringList MapList;
    QStringList NoPrefixMapList;
};

#endif // SELECTMAPDIALOG_H
