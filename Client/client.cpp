#include "client.h"
Client::Client(QString fileName, QString port, int wait){ //Объявляем сокеты и соединяем сигналы и слоты
    socket = new QTcpSocket(this);
    udp_socket = new QUdpSocket(this);
    portUdp = port;
    this->fileName = fileName;
    waitAnswer = wait;
    QObject::connect(socket, SIGNAL(connected()), this, SLOT(succsesfull_connect()));
    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(readData()));
    th = new Thread;
    QObject::connect(this, &Client::goToThread, th, &Thread::sendFileUdp);
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
        const qint64 datagram_size = 1024;

        char raw_data[datagram_size];

        while ((raw_size = file.read(raw_data, datagram_size)) > 0) {

           QByteArray buffer = QByteArray::fromRawData(raw_data, raw_size);

           datagramaFile.insert(id, buffer);
           id++;
        }
        endKey = id;
        th->start();
        emit goToThread(datagramaFile, udp_socket, socket->localAddress(), portUdp.toUShort());
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

void Client::resendFile(QList<int> id) //Отправка файла через UDP
{
    QByteArray datagram;
    QDataStream streamFile(&datagram, QIODevice::WriteOnly);
    foreach (int temp, id){
        udp_socket->abort();
        streamFile << id;
        streamFile << datagramaFile.value(temp);
        udp_socket->writeDatagram(datagram, socket->localAddress(), portUdp.toUShort());
    }
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
    stream.abortTransaction();
    if (message[0] == "1"){
        if (datagramaFile.value(endKey) == datagramaFile.value(QString(message.back()).toInt())){
            if (datagramaFile.empty()){
                qDebug () << "File is sended";
                udp_socket->close();
                socket->close();
                udp_socket->deleteLater();
                socket->deleteLater();
            }
            else resendFile(datagramaFile.keys());
        }
        else datagramaFile.remove(QString(message.back()).toInt());
    }
}

