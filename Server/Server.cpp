#include "Server.h"

Server::Server (QString address, qint16 port, QString fileName) { // Запуск TCP сервера
    server = new QTcpServer(this);
    server->setMaxPendingConnections(1);
    server->waitForNewConnection(300000);
    server->listen(QHostAddress(address), port);

    connect(server, SIGNAL(newConnection()), this, SLOT(setUpSocket()));
    connect(this, SIGNAL(processingData(quint16)), this, SLOT(setUpSocket(quint16)));

    file = new QFile("D:/Work/Smth/c++/Server/" + fileName);
}

Server::~Server()
{
    server->close();
    server->deleteLater();
    tcpSocket->close();
    tcpSocket->deleteLater();
    udpSocket->close();
    udpSocket->deleteLater();
}

void Server::send_answer(QString message) // Функция для ответа
{
    QDataStream stream(tcpSocket);

    QByteArray buff = message.toUtf8();

    stream.setVersion(QDataStream::Qt_5_14);

    stream << buff;
}

void Server::setUpSocket() { //Слот установки TCP сокета
    tcpSocket = server->nextPendingConnection();
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readSocket()));
}

void Server::setUpSocket(quint16 port){ //Cлот установки UDP сокета
    udpSocket = new QUdpSocket (this);
    udpSocket->bind(tcpSocket->localAddress(), port);
    qDebug () << "setup UDP Socket";
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(readSocket()));
}

void Server::readSocket(){ // Считывает данные из сокетов в зависимости от отправителя
    QByteArray buffer;
    if (sender() == udpSocket){
        buffer.resize(udpSocket->pendingDatagramSize());

        udpSocket->readDatagram(buffer.data(), buffer.size());

        QDataStream stream(buffer);

        QString id;
        stream >> id;
        qDebug () << id;
        QByteArray data;
        stream >> data;

        if(file->open(QFile::WriteOnly)){
            file->write(data.data(), data.size());
            file->flush();
        }
        send_answer("1Succsesfull get " + id);
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

            send_answer("UDP is open. File is " + fileName);
            emit(processingData(port));
        }
    }
}


