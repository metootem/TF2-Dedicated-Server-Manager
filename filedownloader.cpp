#include "filedownloader.h"

FileDownloader::FileDownloader(QUrl fileUrl, QDir dest, QObject *parent)
    : QObject{parent}
{
    downloadFile(fileUrl, dest);
}

void FileDownloader::downloadFile(QUrl fileUrl, QDir dest)
{
    qInfo() << "Trying to download " + fileUrl.toString();
    qInfo() << dest.absolutePath() + "/" + fileUrl.fileName();
    TargetFile.setFileName(dest.absolutePath() + "/" + fileUrl.fileName());

    NetAccess = new QNetworkAccessManager(this);
    connect(NetAccess, SIGNAL(finished(QNetworkReply*)), SLOT(fileDownloaded(QNetworkReply*)));

    NetAccess->get(QNetworkRequest(fileUrl));
}

void FileDownloader::errors(QNetworkReply::NetworkError error)
{
    qInfo() << "errors";
    qInfo() << error;
}

void FileDownloader::fileDownloaded(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        TargetFile.open(QIODevice::WriteOnly);
        TargetFile.write(reply->readAll());
        TargetFile.flush();
        TargetFile.close();
        reply->deleteLater();
        emit finished();
        this->deleteLater();
    }
    else
    {
        QString error = reply->errorString();
        qInfo() << "Couldn't download file. " << reply->errorString();
        emit ErrorFound(error);
    }
}
