#include <QtCore/QCoreApplication>
#include <QDebug>
#include <QStringList>
#include "tab2csv.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    if (argc < 3) {
        qWarning() << "wrong count of arguments";
        return 1;
    }

    QString action = a.arguments().at(1);
    QString source = a.arguments().at(2);
    QString target = a.arguments().at(3);

    if (action == "tab2csv") {
        Tab2csv t2c;
        t2c.convert(source, target);
    }

//    return a.exec();
    return 0;
}
