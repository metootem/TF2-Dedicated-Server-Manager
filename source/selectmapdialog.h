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
    void SortMapList();
    QString GetMapPrefix(QString mapName);
    QString SelectMap();

    ~SelectMapDialog();

private slots:

    void on_cmbFilter_currentIndexChanged();

    void on_lineFilter_textChanged();

    void on_btnRefresh_clicked();

private:
    Ui::SelectMapDialog *ui;

    void ClearPrefixFilter();

    QString DirPath;
    QStringList FullMapList;
    QStringList NoPrefixMapList;

    QStringList PrefixList;
    QList<QStringList> MapsList;

    void printInfo(const QString &message);

};

#endif // SELECTMAPDIALOG_H
