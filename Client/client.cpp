#include "client.h"

Client::Client(QString fileName, QString port){
    socket = new QTcpSocket(this);
    udp_socket = new QUdpSocket(this);
    portUdp = port;
    this->fileName = fileName;
    QObject::connect(socket, SIGNAL(connected()), this, SLOT(succsesfull_connect()));
    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(readData()));
}

void Client::connect(QString address, qint16 port){
    socket->connectToHost(address, port);
}

void Client::succsesfull_connect()
{
    qDebug() << "Succsesfull connected to server";
    QString str = portUdp + ' ' + fileName;
    QDataStream stream(socket);
    stream.setVersion(QDataStream::Qt_5_14);
    QByteArray byteArray = str.toUtf8();

    stream << byteArray;
}

void Client::readData(){
    QByteArray buff;
    QDataStream stream (socket);
    stream.setVersion(QDataStream::Qt_5_14);

    stream.startTransaction();
    stream >> buff;

    qDebug () << buff;

    QByteArray Data = "Message from UDP";
    udp_socket->writeDatagram(Data, socket->localAddress(), portUdp.toUShort());
}

