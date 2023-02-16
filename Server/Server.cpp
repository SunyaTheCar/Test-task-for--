#include "Server.h"

Server::Server (QString address, qint16 port) {
    server = new QTcpServer(this);
    server->setMaxPendingConnections(1);
    server->waitForNewConnection(300000);
    server->listen(QHostAddress(address), port);

    connect(server, SIGNAL(newConnection()), this, SLOT(setUpSocket()));
    connect(this, SIGNAL(processingData(QString, quint16)), this, SLOT(setUpSocket(QString, quint16)));
}

void Server::send_answer(QString message)
{
    QDataStream stream(tcpSocket);

    QByteArray buff = message.toUtf8();

    stream.setVersion(QDataStream::Qt_5_14);

    stream << buff;
}

void Server::setUpSocket() {
    tcpSocket = server->nextPendingConnection();
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readSocket()));
}

void Server::setUpSocket(QString ip, quint16 port){
    udpSocket = new QUdpSocket (this);
    udpSocket->bind(QHostAddress(ip), port);
    qDebug () << "setup UDP Socket";
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(readSocket()));
}

void Server::readSocket(){
    QByteArray buffer;
    if (sender() == udpSocket){
        buffer.resize(udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        udpSocket->readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);

        qDebug() << buffer;
    }
    else if (sender() == tcpSocket) {
        QDataStream streamData(tcpSocket);
        streamData.setVersion(QDataStream::Qt_5_14);

        streamData.startTransaction();
        streamData >> buffer;
        if (!buffer.isNull()){
            streamData.abortTransaction();

            QString message = QString::fromStdString(buffer.toStdString());
            QString strPort = message.left(message.indexOf(' '));
            QString fileName = message.remove(message.left(message.indexOf(' ') + 1));
            quint16 port = strPort.toUShort();
            buffer.clear();

            send_answer("Succsefull get data");
            emit(processingData(tcpSocket->localAddress().toString(), port));
        }
    }
}


