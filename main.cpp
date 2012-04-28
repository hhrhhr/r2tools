#include <QtCore/QCoreApplication>
#include <QDebug>
#include <QStringList>
#include "tab2txt.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    if (argc < 3) {
        qWarning() << "see README";
        return 1;
    }

    QString action = a.arguments().at(1);
    QString source = a.arguments().at(2);
    QString target = a.arguments().at(3);

    if (action == "tab2csv") {
        Tab2txt t2c;
        t2c.convert(source, target, "csv");
    } else if (action == "tab2xml") {
        Tab2txt t2x;
        t2x.convert(source, target, "xml");
    }

//    return a.exec();
    return 0;
}
