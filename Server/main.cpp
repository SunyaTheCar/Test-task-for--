#include <QCoreApplication>
#include "Server.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Server *server = new Server("127.0.0.1", 5555); //ip и порт для tcp socket-а

    return a.exec();
}
