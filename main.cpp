#include "dialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<string>("string");
    Q_INIT_RESOURCE(resources);
    Dialog w;
    w.show();

    return a.exec();
}
