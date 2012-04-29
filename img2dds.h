#ifndef IMG2DDS_H
#define IMG2DDS_H

#include <QObject>

class Img2dds : public QObject
{
    Q_OBJECT

    QString basename;

public:
    explicit Img2dds(QObject *parent = 0);
    void convert(QString imgName, QString ddsPath);
    
signals:
    
public slots:
    
};

#endif // IMG2DDS_H
