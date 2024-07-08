#include "../include/clientwork.h"
#include <QThread>
clientWork::clientWork(const qintptr handle, QObject *parent) : QObject{ parent } {

    this->t_fileSize = 0;
    this->localFile = nullptr;
    this->isDownloading = false;
    this->receiveByteSize = 0;
    this->localFile = nullptr;
    this->newFile = nullptr;
    this->socket = new QTcpSocket(this);
    this->socket->setSocketDescriptor(handle);
    connect(this->socket, &QTcpSocket::readyRead, this, &clientWork::thread_receiveData, Qt::DirectConnection);
    connect(this->socket, &QTcpSocket::disconnected, this, &clientWork::disconnected, Qt::DirectConnection);
}

void clientWork::thread_receiveData() {
    if (this->isDownloading) {
        this->thread_receiveFileData();
        return;
    } else {
        QDataStream stream(this->socket);
        int type;
        stream >> type;
        if (type == MESSAGETYPE::userLogin) {
            QByteArray data;
            stream >> data;
            this->userName = QString::fromUtf8(data);
            QByteArray message = QString(this->userName + " " + "已连接服务器").toUtf8();
            emit this->thread_receiveClientMessage(MESSAGETYPE::userLogin, message);
            emit this->thread_newClientLoginMessage(this->userName);
        } else if (type == MESSAGETYPE::userMessage) {
            QByteArray data;
            stream >> data;
            emit this->thread_receiveClientMessage(MESSAGETYPE::userMessage, data);
        } else if (type == MESSAGETYPE::userLogout) {
            QByteArray data;
            stream >> data;
            emit this->thread_receiveClientMessage(MESSAGETYPE::userLogout, data);
        } else if (type == MESSAGETYPE::userFileInfo) {
            this->thread_receiveFileInfo();
            this->isDownloading = true;
        } else if (type == MESSAGETYPE::userFileData) {
            this->thread_sendFileData();
        }
    }
}

void clientWork::thread_setfilepath(const QString &filepath) {
    this->t_filePath = filepath;
    emit this->thread_updateProgress(this->t_filePath);
}

void clientWork::thread_receiveFileInfo() {
    QDataStream stream(this->socket);
    stream >> this->t_fileName >> this->t_fileSize;
    QString info = (this->t_fileName) + QString::number(this->t_fileSize);
    emit this->thread_updateProgress(info);
    QByteArray out;
    QDataStream in(&out, QIODevice::WriteOnly);
    int type = MESSAGETYPE::userFileData;
    in << type;
    this->socket->write(out);

    this->newFile = new QFile(this);
    QString existPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/" + this->t_fileName;
    this->t_filePath = existPath;
    this->newFile->setFileName(existPath);
    if (!this->newFile->open(QFile::WriteOnly)) {
        qDebug() << "open failed";
        return;
    }
}

void clientWork::thread_sendFileInfo() {
    this->localFile = new QFile(this);
    this->localFile->setFileName(this->t_filePath);
    if (this->localFile->isOpen()) {
        this->localFile->close();
    }

    QFileInfo info(this->t_filePath);
    this->t_fileName = info.fileName();
    this->t_fileSize = info.size();
    QByteArray out;
    QDataStream stream(&out, QIODevice::WriteOnly);
    int type = MESSAGETYPE::userFileInfo;
    stream << int(type) << QString(this->t_fileName) << qint64(this->t_fileSize);
    this->socket->write(out);

    if (!this->localFile->open(QFile::ReadOnly)) {
        qDebug() << "open failed";
        return;
    }
}

void clientWork::thread_receiveFileData() {
    qint64 socketSize = this->socket->bytesAvailable();
    if (socketSize <= 0) {
        return;
    }
    int progress = 0;
    if (this->receiveByteSize < this->t_fileSize) {
        progress = (this->receiveByteSize * 1.0) / (this->t_fileSize) * 100;
        const QByteArray filedata = this->socket->read(socketSize);
        const qint64 WrittenByteSize = this->newFile->write(filedata, filedata.size());
        this->receiveByteSize += WrittenByteSize;
        QString pro = QString("当前已接受字节：%1 进度：%2").arg(this->receiveByteSize).arg(progress);
        emit this->thread_updateProgress(pro);
    }
    if (this->receiveByteSize == this->t_fileSize) {
        progress = (this->receiveByteSize * 1.0) / (this->t_fileSize) * 100;
        QString pro = QString("当前已接受字节：%1 进度：%2").arg(this->receiveByteSize).arg(progress);
        emit this->thread_updateProgress(pro);
        this->t_fileSize = 0;
        this->receiveByteSize = 0;
        this->newFile->close();
        this->newFile = nullptr;
        emit this->thread_filepath(this->t_filePath);
        this->isDownloading = false;
        return;
    }
}

void clientWork::thread_sendFileData() {
    qint64 loadSize;
    if ((this->t_fileSize / 1024 / 1024) < 30) {
        loadSize = 1024 * 1024 * 1;
    }
    if ((this->t_fileSize / 1024 / 1024) < 50) {
        loadSize = 1024 * 1024 * 3;
    }
    if ((this->t_fileSize / 1024 / 1024) < 500 && (this->t_fileSize / 1024) > 50) {
        loadSize = 1024 * 1024 * 10;
    }

    qint64 alreadyByteSize = 0;
    int progress = 0;
    while (alreadyByteSize != this->t_fileSize) {
        if (alreadyByteSize < this->t_fileSize) {
            progress = (alreadyByteSize * 1.0) / (this->t_fileSize) * 100;
            const QByteArray filedata = this->localFile->read(qMin(this->t_fileSize, loadSize));
            const qint64 WrittenByteSize = this->socket->write(filedata, filedata.size());
            if (!this->socket->waitForBytesWritten(5 * 1000)) {
                emit this->thread_updateProgress(QString("send file data failed") + this->socket->errorString());
                return;
            }
            alreadyByteSize += WrittenByteSize;
            this->sleep(300);
            QString pro = QString("当前已发送字节：%1 进度：%2").arg(alreadyByteSize).arg(progress);
            emit this->thread_updateProgress(pro);
        }
        if (alreadyByteSize == this->t_fileSize) {
            progress = (alreadyByteSize * 1.0) / (this->t_fileSize) * 100;
            QString pro = QString("当前已发送字节：%1 进度：%2").arg(alreadyByteSize).arg(progress);
            emit this->thread_updateProgress(pro);
            this->t_fileSize = 0;
            this->localFile->close();
            this->localFile = nullptr;
            this->t_fileName = "";
            this->isDownloading = false;
            this->deleteTempFile();
            return;
        }
    }
}

void clientWork::thread_sendClientMessage(const int type, const QByteArray &message) {
    if (type == MESSAGETYPE::userLogin) {
        QByteArray out;
        QDataStream stream(&out, QIODevice::WriteOnly);
        stream << int(type) << QByteArray(message);
        this->socket->write(out);
    }
    if (type == MESSAGETYPE::userMessage) {
        QByteArray out;
        QDataStream stream(&out, QIODevice::WriteOnly);
        stream << int(type) << QByteArray(message);
        this->socket->write(out);
    }
    if (type == MESSAGETYPE::userNumbers) {
        QByteArray out;
        QDataStream stream(&out, QIODevice::WriteOnly);
        stream << int(type) << QByteArray(message);
        this->socket->write(out);
    }
    if (type == MESSAGETYPE::userLogout) {
        QByteArray out;
        QDataStream stream(&out, QIODevice::WriteOnly);
        stream << int(type) << QByteArray(message);
        this->socket->write(out);
    }
}

void clientWork::sleep(int msec) {
    QTime time = QTime::currentTime();
    QTime nowTime;
    do {
        nowTime = QTime::currentTime();
    } while (time.msecsTo(nowTime) < msec);
}

const QString clientWork::getClientUserName() {
    return this->userName;
}

void clientWork::disconnected() {
    emit this->thread_clientdisconnectedServer(this->userName);
    emit this->thread_updateClientNumbers(userNumbers);
}

void clientWork::deleteTempFile() {
    QFileInfo info(this->t_filePath);
    if (info.isFile()) {
        QFile::remove(this->t_filePath);
    }
    this->t_filePath = "";
}

void clientWork::updateClientNumbers() {
    emit this->thread_updateClientNumbers(userNumbers);
}
