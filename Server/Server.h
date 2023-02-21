#ifndef SERVER_H
#define SERVER_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QDataStream>
#include <QUdpSocket>
#include <QFile>
#include <QCryptographicHash>

class Server : public QObject {
    Q_OBJECT
public:
    Server (QString address, qint16 port, QString fileName);
    ~Server();
private:
    QTcpServer *server;
    QTcpSocket *tcpSocket;
    QUdpSocket *udpSocket;
    QFile *file;
    void send_answer(QString message);
private slots:
    void setUpSocket();
    void readSocket();
    void setUpSocket(quint16 port);
signals:
    void processingData(quint16 port);
};

#endif // SERVER_H
