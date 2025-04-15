#ifndef SERVERCONSOLEDIALOG_H
#define SERVERCONSOLEDIALOG_H

#include <QDialog>
#include <QProcess>
#include <QTimer>

namespace Ui {
class ServerConsoleDialog;
}

class ServerConsoleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ServerConsoleDialog(QWidget *parent = nullptr);
    ServerConsoleDialog(QWidget *parent = nullptr, QProcess *process = nullptr, QString Server = "Server Dialog");


    ~ServerConsoleDialog();

public slots:
    void ReadOutput();
    void ReadError();

private slots:
    void on_lineInput_returnPressed();

private:
    Ui::ServerConsoleDialog *ui;

    QProcess *Process;
    QTimer *ConsoleRefreshRate;
    int RefreshCount;
};

#endif // SERVERCONSOLEDIALOG_H
