#ifndef THREAD_H
#define THREAD_H
#include <QThread>
#include <QMap>
#include "client.h"
#include <QByteArray>
#include <QDataStream>
class Thread : public QThread
{
public:
    Thread(QObject* parent = nullptr);
public slots:
    void sendFileUdp(QMap<int, QByteArray> datagram, QUdpSocket *socket, QHostAddress adress, ushort port);
private:
    void run() override;
};

#endif // THREAD_H
