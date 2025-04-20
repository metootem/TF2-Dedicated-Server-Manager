#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDir>
#include <QFile>
#include <QEventLoop>

class FileDownloader : public QObject
{
    Q_OBJECT
public:
    explicit FileDownloader(QUrl fileUrl, QDir dest, QObject *parent = nullptr);

    void downloadFile(QUrl, QDir);

public slots:
    void errors(QNetworkReply::NetworkError);

signals:
    void finished();
    void ErrorFound(QString error);

private slots:
    void fileDownloaded(QNetworkReply* reply);

private:
    QNetworkAccessManager *NetAccess;

    QFile TargetFile;
};

#endif // FILEDOWNLOADER_H
