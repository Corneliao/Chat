#include "../include/tcpserver.h"

tcpServer::tcpServer(QObject *parent) : QTcpServer(parent) {
}

void tcpServer::incomingConnection(const qintptr handle) {
    emit this->newClientConnection(handle);
}
