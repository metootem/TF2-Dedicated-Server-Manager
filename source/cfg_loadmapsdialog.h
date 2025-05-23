#ifndef CFG_LOADMAPSDIALOG_H
#define CFG_LOADMAPSDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>

#include "selectmapdialog.h"

namespace Ui {
class Cfg_LoadMapsDialog;
}

class Cfg_LoadMapsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit Cfg_LoadMapsDialog(QString mapDir, QWidget *parent = nullptr);
    ~Cfg_LoadMapsDialog();

    QStringList ReturnMaps();

private slots:
    void LoadMaps( QStringList prefixFilter );
    void AddMapToList( QString );
    QStringList GetPrefixFilter();

    void on_btnAddSrv_clicked();

    void on_btnRmvSrv_clicked();

    void on_btnRfshPrefix_clicked();

    void on_btnAddPrefix_clicked();

    void on_btnRmvPrefix_clicked();

    void on_btnClearMapList_clicked();

private:
    Ui::Cfg_LoadMapsDialog *ui;

    QString MapDir;
};

#endif // CFG_LOADMAPSDIALOG_H
