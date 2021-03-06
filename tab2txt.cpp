#include "tab2txt.h"
#include <QFile>
#include <QDateTime>
#include <QFileInfo>
#include <QDebug>

Tab2txt::Tab2txt(QObject *parent) :
    QObject(parent)
{
}

void Tab2txt::convert(QString tabName, QString txtPath, QString outType)
{
// input
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
    QFileInfo fi(tab);
    basename = fi.baseName();

// read
    quint32 magic;
    in >> magic;
    if (magic != 826425684UL) {
        qDebug() << "magic not match:" << magic;
        return;
    }
    qDebug() << "magic  : TAB1";

    quint16 version;
    in >> version;
    if (version != 2) {
        qDebug() << "version not match:" << version;
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

    QStringList col;

    quint32 rowsTmp = 0;
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

            // //check for bad numbers
            if (i > 0 && rows != rowsTmp) {
                qDebug() << "number of rows is different";
                return;
            }
            rowsTmp = rows;
            // \\check for bad numbers

            for (uint j = 0; j < rows; ++j) {
                quint16 textLenght;
                in >> textLenght;

                if (format == 0) {
                    // TODO: check this
                    char textString[textLenght];
                    char *t = textString;
                    k = 0;
                    while (k < textLenght) {
                        in >> t;
                        k++;
                    }
                    col << QString(textString);
                } else if (format == 1) {
                    QChar textString[textLenght];
                    k = 0;
                    while (k < textLenght)
                        in >> textString[k++];
                    col << QString(textString, textLenght);
                }
            }
            cols << col;
            col.clear();
        } else {
            qDebug() << "not attached!!!";
        }
    }
    tab.close();
    qDebug() << "tab readed";

    m_txtPath = txtPath;
    if (outType == "xml")
        save2xml();
    else if (outType == "csv")
        save2cvs();
}

void Tab2txt::save2cvs()
{
    QFile csv(m_txtPath + "/" + basename + ".csv");
//    qDebug() << csv.fileName();
    if (!csv.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qDebug() << "output file open error";
        return;
    }
    QTextStream out(&csv);
    QString line;
    for (int i = 0; i < cols.at(0).size(); ++i) {
        for (int j = 0; j < cols.size(); ++j) {
            line.append(cols.at(j).at(i)).append(";");
        }
        out << line << endl;
        line.clear();
    }

    csv.close();
    qDebug() << "csv is written successfully";
}

void Tab2txt::save2xml()
{
    QFile xml(m_txtPath + "/" + basename + ".xml");
//    qDebug() << xml.fileName();
    if (!xml.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qDebug() << "output file open error";
        return;
    }
    QTextStream out(&xml);
    QString tag;
    out << "<" << basename << ">" << endl;
    for (int i = 1; i < cols.at(0).size(); ++i) {
        out << "  <string id=\"" << cols.at(0).at(i) << "\">" << endl;
        for (int j = 1; j < cols.size(); ++j) {
//            setLangTag(cols.at(j).at(0), tag);
            tag = cols.at(j).at(0);
            out << "    <lang id=\"" << tag << "\">";
            out << cols.at(j).at(i);
            out << "</" << tag << ">" << endl;
        }
        out << "  </string>" << endl << endl;
    }
    out << "</" << basename << ">" << endl;

    xml.close();
    qDebug() << "xml is written successfully";
}

void Tab2txt::setLangTag(const QString lng, QString &tag)
{
    if (lng == "German_Text")
        tag = "DE";
    else if (lng == "English_Text")
        tag = "EN";
    else if (lng == "German_Text")
        tag = "DE";
    else if (lng == "French_Text")
        tag = "FR";
    else if (lng == "Italian_Text")
        tag = "IT";
    else if (lng == "Spanish_Text" || lng == "Spanish_text")
        tag = "SP";
    else if (lng == "Russian_Text")
        tag = "RU";
    else if (lng == "Polish_Text")
        tag = "PL";
    else if (lng == "Czech_Text")
        tag = "CZ";
    else
        tag = lng;
}
