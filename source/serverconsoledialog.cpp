#include "serverconsoledialog.h"
#include "ui_serverconsoledialog.h"

ServerConsoleDialog::ServerConsoleDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ServerConsoleDialog)
{
    ui->setupUi(this);
}

ServerConsoleDialog::ServerConsoleDialog(QWidget *parent, QProcess *process, QString Server)
    : QDialog(parent)
    , ui(new Ui::ServerConsoleDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(Server);
    Process = process;
    connect(process, SIGNAL(readyRead()), this, SLOT(ReadOutput()));
    //connect(process, SIGNAL(readyReadStandardError()), this, SLOT(ReadError()));
    //ConsoleRefreshRate = new QTimer(this);
   // connect(ConsoleRefreshRate, SIGNAL(timeout()), this, SLOT(ReadOutput()));
    //ConsoleRefreshRate->start(1000);
    //RefreshCount = 0;
}

ServerConsoleDialog::~ServerConsoleDialog()
{
    delete ui;
}

void ServerConsoleDialog::ReadOutput()
{
    QByteArray output = Process->readAllStandardOutput();
    ui->txtMainConsole->append(output);
    qInfo() << output;
}

void ServerConsoleDialog::ReadError()
{
    ui->txtErrorConsole->append(Process->readAllStandardError());
}

void ServerConsoleDialog::on_lineInput_returnPressed()
{
    ui->txtMainConsole->append(ui->lineInput->text());
    QByteArray command(ui->lineInput->text().toLocal8Bit() + "\n");
    Process->write("say hello");
    ui->lineInput->clear();
}

