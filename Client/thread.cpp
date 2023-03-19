#include "thread.h"


Thread::Thread(QObject *parent) : QThread(parent)
{

}

void Thread::sendFileUdp(QMap<int, QByteArray> datagram, QUdpSocket *socket, QHostAddress adress, ushort port)
{
    QByteArray data;
    QDataStream streamFile(&data, QIODevice::WriteOnly);
    for (int i = 0; i < datagram.size(); i++){
        streamFile << i;
        streamFile << datagram.value(i);
        socket->writeDatagram(data, adress, port);
    }

}
