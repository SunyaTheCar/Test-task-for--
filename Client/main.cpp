#include <QCoreApplication>
#include "client.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Client *obj_client = new Client("test", "6000"); //имя файла и порт для udp
    obj_client->connect("127.0.0.1", 5555); //ip и порт для tcp сокета

    return a.exec();
}
