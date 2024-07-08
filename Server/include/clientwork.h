#ifndef CLIENTWORK_H
#define CLIENTWORK_H

#include <QFile>
#include <QFileInfo>
#include <QObject>
#include <QStandardPaths>
#include <QTcpSocket>
#include <QTime>
class clientWork : public QObject {
    Q_OBJECT
    enum MESSAGETYPE { userLogin = 0, userMessage, userNumbers, userLogout, userFileInfo, userFileData };

    public:
    explicit clientWork(const qintptr handle, QObject *parent = nullptr);
    void thread_receiveData();

    void thread_setfilepath(const QString &filepath);
    void thread_receiveFileInfo();
    void thread_sendFileInfo();
    void thread_receiveFileData();
    void thread_sendFileData();
    void thread_sendClientMessage(const int message_type, const QByteArray &message);
    void sleep(int msec);
    const QString getClientUserName();
    void disconnected();
    void deleteTempFile();
    void updateClientNumbers();
    signals:
    void thread_receiveClientMessage(const int message_type, const QByteArray &message);
    void thread_filepath(const QString &filepath);
    void thread_updateProgress(const QString &progress);
    void thread_clientdisconnectedServer(const QString &userName);
    void thread_newClientLoginMessage(const QString &username);
    void thread_updateClientNumbers(const int type);

    private:
    QTcpSocket *socket;
    QString userName;
    // file  info
    QString t_filePath;
    QString t_fileName;
    qint64 t_fileSize;
    QFile *localFile;
    bool isDownloading;
    QFile *newFile;
    qint64 receiveByteSize;
};

#endif // CLIENTWORK_H
