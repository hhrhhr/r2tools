#include "tab2csv.h"
#include <QFile>
#include <QDateTime>
#include <QStringList>
#include <QFileInfo>
#include <QDebug>

Tab2csv::Tab2csv(QObject *parent) :
    QObject(parent)
{
}

void Tab2csv::convert(QString tabName, QString csvPath)
{
    QFile tab(tabName);
    if (!tab.exists()) {
        qDebug() << "file not found";
        return;
    }
    if (!tab.open(QIODevice::ReadOnly)) {
        qDebug() << "file open error";
        return;
    }
    QDataStream in(&tab);
    in.setByteOrder(QDataStream::LittleEndian);

    quint32 magic;
    in >> magic;
    if (magic != 826425684UL) {
        qDebug() << "magic not match";
        return;
    }
    qDebug() << "magic  : TAB1";

    quint16 version;
    in >> version;
    if (version != 2) {
        qDebug() << "version not match";
        return;
    }
    qDebug() << "version: 2";

    quint16 format;
    in >> format;
    if (format != 0 && format != 1) {
        qDebug() << "format unknown:" << format;
        return;
    }
    qDebug() << "format :" << ((format > 0) ? "Unicode" : "ANSI");

    quint64 dateTime;
    in >> dateTime;
    QDateTime dt = QDateTime::fromTime_t(dateTime * 0.0000001 + 0x00295E9648864000ULL);
    qDebug() << "time   :" << dt.toString("dd.MM.yyyy HH:mm:ss");

    quint32 columns;
    in >> columns;
    qDebug() << "columns:" << columns;

    QList<QStringList> cols;
    QStringList col;

    for (uint i = 0; i < columns; ++i) {
        quint8 attached;
        in >> attached;
//        qDebug() << "\nattached" << attached;

        if (attached) {
            quint16 unknown;
            in >> unknown;
//            qDebug() << unknown;

            quint16 nameLenght;
            in >> nameLenght;
//            qDebug() << nameLenght;

            QChar nameString[nameLenght];
            quint16 k = 0;
            while (k < nameLenght)
                in >> nameString[k++];
            col << QString(nameString, nameLenght);

            quint32 rows;
            in >> rows;
//            qDebug() << "rows" << rows;

            for (uint j = 0; j < rows; ++j) {
                quint16 textLenght;
                in >> textLenght;

                QChar textString[textLenght];
                k = 0;
                while (k < textLenght)
                    in >> textString[k++];
                col << QString(textString, textLenght);
            }

            cols << col;
            col.clear();
        }
    }
    tab.close();

    QFileInfo fi(tab);
    QFile csv(csvPath + "/" + fi.baseName() + ".csv");
    qDebug() << csv.fileName();
    if (!csv.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qDebug() << "output file open error";
        return;
    }
    QTextStream out(&csv);
    QString tmp;
    for (int i = 0; i < cols.at(0).size(); ++i) {
        for (int j = 0; j < cols.size(); ++j) {
            tmp.append(cols.at(j).at(i)).append(";");
        }
        out << tmp << endl;
        tmp.clear();
    }

    csv.close();
}
