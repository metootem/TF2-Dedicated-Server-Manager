#include "filedownloader.h"

FileDownloader::FileDownloader(QObject *parent)
    : QObject{parent}
{
}

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
    loop.exec();
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
        qInfo() << "downloaded file";
        loop.exit();
        emit finished();
    }
    else
    {
        QString error = reply->errorString();
        qInfo() << "Couldn't download file. " << reply->errorString();
        loop.exit();
        emit ErrorFound(error);
    }
    reply->deleteLater();
    this->deleteLater();
}
