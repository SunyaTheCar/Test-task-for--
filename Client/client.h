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
    void generate_datagram();
    int endKey;
    int waitAnswer;
private slots:
    void succsesfull_connect();
    void readData();
    void sendFileUDP(QString id = "0");
    void readAnswer();
signals:
    void startSendFile(QString id = "0");
};

#endif // CLIENT_H
