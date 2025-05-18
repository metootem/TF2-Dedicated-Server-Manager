#ifndef STEAMCMDDIALOG_H
#define STEAMCMDDIALOG_H

#include <QDialog>
#include <QProcess>
#include <QScrollBar>

#define Error_NoDiskSpace (1 << 0)

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

public slots:
    void ReadOutput();
    void InstallFinished(int exitCode, QProcess::ExitStatus exitStatus);

private slots:

private:
    Ui::SteamCMDDialog *ui;

    QProcess *Process;
    int errorCode;
};

#endif // STEAMCMDDIALOG_H
