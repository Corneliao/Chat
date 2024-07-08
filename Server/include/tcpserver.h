#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
class tcpServer : public QTcpServer {
    Q_OBJECT
    public:
    explicit tcpServer(QObject *parent = nullptr);

    protected:
    void incomingConnection(const qintptr handle) override;
    signals:
    void newClientConnection(const qintptr handle);
};

#endif // TCPSERVER_H
