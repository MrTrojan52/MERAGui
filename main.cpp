#include "dialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<string>("string");
    Q_INIT_RESOURCE(resources);
    QFile styleFile(":/style/assets/ControlStyles.qss");
    styleFile.open(QFile::ReadOnly);
    a.setFont(QFont("Roboto", 11, QFont::Normal));    
    a.setStyleSheet(QLatin1String(styleFile.readAll()));
    styleFile.close();
    Dialog w;
    w.show();

    return a.exec();
}
