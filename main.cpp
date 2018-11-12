#include "dialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<string>("string");
    Dialog w;
    w.show();

    return a.exec();
}
