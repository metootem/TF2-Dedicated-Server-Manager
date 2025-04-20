#ifndef STEAMCMDDIALOG_H
#define STEAMCMDDIALOG_H

#include <QDialog>
#include <QProcess>

namespace Ui {
class SteamCMDDialog;
}

class SteamCMDDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SteamCMDDialog(QWidget *parent = nullptr, QProcess *process = nullptr, QString name = "");

    void NewProcess(QProcess *process);

    ~SteamCMDDialog();

signals:
    void KillSteamCMDProcess();

public slots:
    void ReadOutput();
    void InstallFinished();

private slots:
    void on_btnCancel_clicked();

private:
    Ui::SteamCMDDialog *ui;

    QProcess *Process;
};

#endif // STEAMCMDDIALOG_H
