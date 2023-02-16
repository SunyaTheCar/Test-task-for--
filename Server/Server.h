#ifndef SERVER_H
#define SERVER_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QDataStream>
#include <QUdpSocket>

class Server : public QObject {
    Q_OBJECT
public:
    Server (QString address, qint16 port);
private:
    QTcpServer *server;
    QTcpSocket *tcpSocket;
    QUdpSocket *udpSocket;
    void send_answer(QString message);
private slots:
    void setUpSocket();
    void readSocket();
    void setUpSocket(QString ip, quint16 port);
signals:
    void processingData(QString ip, quint16 port);
};

#endif // SERVER_H
