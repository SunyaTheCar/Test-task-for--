#ifndef CLIENT_H
#define CLIENT_H
#include <QTcpSocket>
#include <QHostAddress>
#include <QObject>
#include <QDataStream>
#include <QUdpSocket>


class Client : public QObject{
    Q_OBJECT
public:
    Client(QString fileName, QString port);
    void connect(QString address, qint16 port = 0);
private:
    QTcpSocket *socket;
    QUdpSocket *udp_socket;
    QString portUdp;
    QString fileName;
private slots:
    void succsesfull_connect();
    void readData();
};

#endif // CLIENT_H
