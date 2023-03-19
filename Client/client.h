#ifndef CLIENT_H
#define CLIENT_H
#include <QTcpSocket>
#include <QHostAddress>
#include <QObject>
#include <QDataStream>
#include <QUdpSocket>
#include <QFileDevice>
#include <QFile>
#include <QCryptographicHash>
#include <QList>
#include "thread.h"

class Client : public QObject{
    Q_OBJECT
public:
    Client(QString fileName, QString port, int wait = 50000);
    void connect(QString address, qint16 port = 0);
private:
    QTcpSocket *socket;
    QUdpSocket *udp_socket;
    QString portUdp;
    QString fileName;
    QMap <int, QByteArray> datagramaFile;
    Thread *th;
    void generate_datagram();
    int endKey;
    int waitAnswer;
    void sendFileUDP();
    void resendFile(QList<int> id);
private slots:
    void succsesfull_connect();
    void readData();
    void readAnswer();
signals:
    void goToThread(QMap<int, QByteArray> datagram, QUdpSocket *socket, QHostAddress adress, ushort port);
};

#endif // CLIENT_H
