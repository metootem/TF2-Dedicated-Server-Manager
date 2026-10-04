#ifndef STEAMCMDDIALOG_H
#define STEAMCMDDIALOG_H

#include <QDialog>
#include <QProcess>
#include <QScrollBar>

#define Error_NoDiskSpace (1 << 0)
#define Error_Unknown (1 << 1)
#define Error_SteamDown (1 << 2)
#define Error_Interrupted (1 << 3)
#define Error_NoPermission (1 << 4)
#define Error_NoConnection (1 << 5)

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
