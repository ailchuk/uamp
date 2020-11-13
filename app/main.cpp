#include "uamp.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName("Player Example");
    QCoreApplication::setOrganizationName("UNIT Study project");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);



    UAmp w;
    w.show();
    return a.exec();
}
