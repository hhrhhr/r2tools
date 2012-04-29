#include "img2dds.h"
#include <QFile>
#include <QFileInfo>
#include <QDateTime>

#include <QDebug>

Img2dds::Img2dds(QObject *parent) :
    QObject(parent)
{
}

void Img2dds::convert(QString imgName, QString ddsPath)
{
    QFile img(imgName);
    if (!img.exists()) {
        qDebug() << "file not found";
        return;
    }
    if (!img.open(QIODevice::ReadOnly)) {
        qDebug() << "file open error";
        return;
    }
    QDataStream in(&img);
    in.setByteOrder(QDataStream::LittleEndian);
    QFileInfo fi(img);
    basename = fi.baseName();

// read
    quint32 magic;
    in >> magic;
    if (magic != 861028679UL) {
        qDebug() << "magic not match:" << magic;
        return;
    }
    qDebug() << "magic     : GAR3";

    in >> magic;
    if (magic != 0) {
        qDebug() << "magic2 not match:" << magic;
        return;
    }

    quint32 resMagic;
    in >> resMagic;
    if (resMagic != 825250375UL) {
        qDebug() << "resMagic not match:" << resMagic;
        return;
    }
    qDebug() << "resMagic  : GR01";

    quint32 resClass;
    in >> resClass;
    if (resClass != 875580745UL) {
        qDebug() << "resClass not match:" << resClass;
        return;
    }
    qDebug() << "resMagic  : IM04";

    quint32 propOffset;
    in >> propOffset;

    quint32 propLenght;
    in >> propLenght;

    quint32 dataOffset;
    in >> dataOffset;

    quint32 dataLenght;
    in >> dataLenght;

//    qDebug() << propOffset << propLenght << dataOffset << dataLenght;

    quint64 dateTime;
    in >> dateTime;
    QDateTime dt = QDateTime::fromTime_t(dateTime * 0.0000001 + 0x00295E9648864000ULL);
    qDebug() << "time      :" << dt.toString("dd.MM.yyyy HH:mm:ss");

    char rawFileExt[8];
    in.readRawData(rawFileExt, 8);
    qDebug() << "rawFileExt:" << rawFileExt;

// props
    if (in.device()->pos() != propOffset + 8) {
        qDebug() << "pos() != propOffset";
        return;
    }

// write props
    QFile prop(ddsPath + "/" + basename + ".prop");
    if (!prop.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qDebug() << "output file open error";
        return;
    }
    QDataStream outProp(&prop);
    QByteArray ba;
    ba.resize(propLenght);
    in.readRawData(ba.data(), ba.size());
    outProp.writeRawData(ba.data(), ba.size());
    prop.close();
    qDebug() << "prop is written successfully";

// data
    in >> magic;
    if (magic != 861028679UL) {
        qDebug() << "data magic not match:" << magic;
        return;
    }

    in >> magic;
    if (magic != 0) {
        qDebug() << "data magic2 not match:" << magic;
        return;
    }

// write dds
    QFile dds(ddsPath + "/" + basename + ".dds");
    if (!dds.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qDebug() << "output file open error";
        return;
    }
    QDataStream ddsOut(&dds);
    ba.resize(dataLenght - 8);
    in.readRawData(ba.data(), ba.size());
    ddsOut.writeRawData(ba.data(), ba.size());
    dds.close();
    qDebug() << "dds is written successfully";


    img.close();
}
