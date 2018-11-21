#include "dialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<string>("string");
    Q_INIT_RESOURCE(resources);
    a.setFont(QFont("Roboto", 11, QFont::Normal));
    Dialog w;
    w.show();

    return a.exec();
}
