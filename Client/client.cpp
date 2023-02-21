#include "client.h"

Client::Client(QString fileName, QString port){ //Объявляем сокеты и соединяем сигналы и слоты
    socket = new QTcpSocket(this);
    udp_socket = new QUdpSocket(this);
    portUdp = port;
    this->fileName = fileName;

    QObject::connect(this, SIGNAL(startSendFile(QString)), this, SLOT(sendFileUDP(QString)));
    QObject::connect(socket, SIGNAL(connected()), this, SLOT(succsesfull_connect()));
    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(readData()));
}

void Client::connect(QString address, qint16 port){ // Подключение к серверу
    socket->connectToHost(address, port);
}

void Client::generate_datagram() // Генерация датаграммы
{
    QFile file ("D:/Work/Smth/c++/Client/" + fileName);
    if(file.open(QFile::ReadOnly)){
        qint64 raw_size = 0;
        int  id = 0;
        const qint64 datagram_size = 128;

        char raw_data[datagram_size];

        while ((raw_size = file.read(raw_data, datagram_size)) > 0) {

           QByteArray buffer = QByteArray::fromRawData(raw_data, raw_size);

           datagramaFile.insert(id, buffer);

           id++;
        }
        endKey = id;
        emit(startSendFile());
    }
}

void Client::succsesfull_connect() //Слот срабатывающий при emit-е сигнала connected()
{
    qDebug() << "Succsesfull connected to server";
    QString str = portUdp + ' ' + fileName;
    QDataStream stream(socket);
    QByteArray byteArray = str.toUtf8();

    stream.setVersion(QDataStream::Qt_5_14);
    stream << byteArray;
}

void Client::readData(){ //Считывание данных о получении порта и имени файла UDP сокета
    QByteArray buff;
    QDataStream stream (socket);
    stream.setVersion(QDataStream::Qt_5_14);

    stream.startTransaction();
    stream >> buff;
    stream.abortTransaction();
    qDebug () << buff;
    QObject::disconnect(socket, SIGNAL(readyRead()), this, SLOT(readData()));
    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(readAnswer()));
    generate_datagram();
}

void Client::sendFileUDP(QString id) //Отправка файла через UDP
{
    udp_socket->abort();
    QByteArray datagram;

    QDataStream streamFile(&datagram, QIODevice::WriteOnly);
    streamFile << id;
    streamFile << datagramaFile.value(id.toInt());
    udp_socket->writeDatagram(datagram, socket->localAddress(), portUdp.toUShort());
}

void Client::readAnswer() //Считывание ответа о принятии
{
    QByteArray buffer;
    QDataStream stream (socket);
    stream.setVersion(QDataStream::Qt_5_14);
    stream.startTransaction();
    stream >> buffer;

    qDebug () << buffer;
    QString message = QString::fromStdString(buffer.toStdString());
    if (message[0] == "1"){
        stream.abortTransaction();
        if (datagramaFile.value(endKey) == datagramaFile.value(QString(message.back()).toInt())){
            qDebug () << "File is sended";
            udp_socket->close();
            socket->close();
            udp_socket->deleteLater();
            socket->deleteLater();
        }
        else emit(startSendFile(QString::number(QString(message.back()).toInt() + 1)));
    }
}

