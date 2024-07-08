#include "../include/clientwork.h"
clientWork::clientWork(QObject *parent) : QObject{ parent } {
    this->socket = nullptr;
    this->isDownloading = false;
    this->t_fileSize = 0;
    this->receiveByteSize = 0;
    this->newFile = nullptr;
}

void clientWork::thread_connectionServer(const QString &username) {
    this->userName = username;
    this->socket = new QTcpSocket(this);
    this->socket->connectToHost(QHostAddress("127.0.0.1"), quint16(7777));
    if (!this->socket->waitForConnected(1 * 1000)) {
        emit this->thread_connectServerState(false);
        return;
    }
    this->thread_sendLoginInfo();
    emit this->thread_connectServerState(true);
    connect(this->socket, &QTcpSocket::readyRead, this, &clientWork::thread_receiveData);
    connect(this->socket, &QTcpSocket::disconnected, this, &clientWork::thread_disconnectedServer);
}

void clientWork::thread_sendFileInfo(const QString &filepath) {
    this->t_filePath = filepath;
    this->localFile = new QFile(this);
    this->localFile->setFileName(this->t_filePath);
    if (this->localFile->isOpen()) {
        this->localFile->close();
    }
    QFileInfo info(this->t_filePath);
    QByteArray out;
    QDataStream stream(&out, QIODevice::WriteOnly);
    this->t_fileName = info.fileName();
    this->t_fileSize = info.size();
    int type = MESSAGETYPE::userFileInfo;
    stream << int(type) << QString(this->t_fileName) << qint64(this->t_fileSize);
    this->socket->write(out);

    this->localFile = new QFile(this->t_filePath);
    if (!this->localFile->open(QFile::ReadOnly)) {
        qDebug() << "open failed";
        return;
    }
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
            QString message = QString::fromUtf8(data);
            emit this->thread_updateClientLoinInfoToQml(message);
        } else if (type == MESSAGETYPE::userMessage) {
            QByteArray data;
            stream >> data;
            QString message = QString::fromUtf8(data);
            emit this->thread_updateClientMessageToQml(message);
        } else if (type == MESSAGETYPE::userNumbers) {
            QByteArray data;
            stream >> data;
            QString clientSize = QString::fromUtf8(data);
            emit this->thread_updateClientNumbers(clientSize);
        } else if (type == MESSAGETYPE::userLogout) {
            QByteArray data;
            stream >> data;
            emit this->thread_updateClientLoinInfoToQml(data);
        } else if (type == MESSAGETYPE::userFileInfo) {
            this->thread_receiveFileInfo();
            this->isDownloading = true;
        } else if (type == MESSAGETYPE::userFileData) {
            this->thread_sendFileData();
        }
    }
}

void clientWork::thread_receiveFileInfo() {
    QDataStream stream(this->socket);
    stream >> this->t_fileName >> this->t_fileSize;

    QByteArray out;
    QDataStream in(&out, QIODevice::WriteOnly);
    int type = MESSAGETYPE::userFileData;
    in << type;
    this->socket->write(out);

    this->newFile = new QFile(this);
    this->newFile->setFileName(this->t_fileName);
    if (!this->newFile->open(QFile::WriteOnly)) {
        qDebug() << "open writeonly failed";
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
        emit this->thread_updateProgress(QString::number(progress));
    }
    if (this->receiveByteSize == this->t_fileSize) {
        progress = (this->receiveByteSize * 1.0) / (this->t_fileSize) * 100;
        emit this->thread_updateProgress(QString::number(progress));
        this->t_fileSize = 0;
        this->receiveByteSize = 0;
        this->newFile->close();
        this->newFile = nullptr;
        this->t_filePath = "";
        this->t_fileName = "";
        this->isDownloading = false;
        return;
    }
}

void clientWork::thread_sendFileData() {
    qint64 loadSIze;
    if ((this->t_fileSize / 1024 / 1024) < 50) {
        loadSIze = 1024 * 1024 * 3;
    }
    if ((this->t_fileSize / 1024 / 1024) < 500 && (this->t_fileSize / 1024) > 50) {
        loadSIze = 1024 * 1024 * 10;
    }
    qint64 alreadyByteSize = 0;
    int progress = 0;
    while (alreadyByteSize != this->t_fileSize) {
        if (alreadyByteSize < this->t_fileSize) {
            progress = (alreadyByteSize * 1.0) / (this->t_fileSize) * 100;
            const QByteArray filedata = this->localFile->read(qMin(this->t_fileSize, loadSIze));
            const qint64 WrittenByteSize = this->socket->write(filedata, filedata.size());
            this->sleep(100);
            if (!this->socket->waitForBytesWritten()) {
                emit this->thread_updateProgress(QString("send file data failed"));
                return;
            }
            alreadyByteSize += WrittenByteSize;
        }
        if (alreadyByteSize == this->t_fileSize) {
            progress = (alreadyByteSize * 1.0) / (this->t_fileSize) * 100;
            this->t_fileSize = 0;
            this->localFile->close();
            this->localFile = nullptr;
            this->t_fileName = "";
            this->t_filePath = "";
            return;
        }
    }
}

void clientWork::thread_ActivedisconnectedServer() {
    QByteArray out;
    QDataStream stream(&out, QIODevice::WriteOnly);
    QByteArray message = QString(this->userName + " 已断开 连接").toUtf8();
    int type = MESSAGETYPE::userLogout;
    stream << type << message;
    this->socket->write(out);
    this->socket->disconnectFromHost();
}

void clientWork::thread_sendQmlMessage(const QString &message) {
    QByteArray out;
    QDataStream stream(&out, QIODevice::WriteOnly);
    int type = MESSAGETYPE::userMessage;
    const QByteArray data = QString(this->userName + " ：" + message).toUtf8();
    stream << type << data;
    this->socket->write(out);
}

void clientWork::sleep(int msec) {
    QTime time = QTime::currentTime();
    QTime nowTime;
    do {
        nowTime = QTime::currentTime();
    } while (time.msecsTo(nowTime) < msec);
}

void clientWork::thread_sendLoginInfo() {
    QByteArray out;
    QDataStream stream(&out, QIODevice::WriteOnly);
    int type = MESSAGETYPE::userLogin;
    QByteArray loginInfo = QString(this->userName).toUtf8();
    stream << int(type) << QByteArray(loginInfo);
    this->socket->write(out);
}

void clientWork::thread_getUserFilePath(const QString &filepath) {
    this->t_filePath = filepath.right(filepath.length() - 8);
    this->thread_sendFileInfo(this->t_filePath);
}
