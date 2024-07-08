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
    explicit clientWork(QObject *parent = nullptr);
    void thread_connectionServer(const QString &username);
    void thread_sendFileInfo(const QString &filepath);
    void thread_receiveData();
    void thread_receiveFileInfo();
    void thread_receiveFileData();
    void thread_sendFileData();
    void sleep(int msec);
    void thread_sendLoginInfo();

    public slots:
    void thread_ActivedisconnectedServer();
    void thread_sendQmlMessage(const QString &message);
    void thread_getUserFilePath(const QString &filepath);
    signals:
    void thread_updateProgress(const QString &progress);
    void thread_disconnectedServer();
    void thread_updateClientMessageToQml(const QString &message);
    void thread_connectServerState(const bool flag);
    void thread_updateClientNumbers(const QString &clientSize);
    void thread_updateClientLoinInfoToQml(const QString &mesage);

    private:
    QString userName;
    QTcpSocket *socket;
    QString t_filePath;
    QString t_fileName;
    qint64 t_fileSize;
    qint64 receiveByteSize;
    QFile *newFile;
    bool isDownloading;
    QFile *localFile;
};

#endif // CLIENTWORK_H
