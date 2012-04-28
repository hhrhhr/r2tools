#ifndef TAB2XML_H
#define TAB2XML_H

#include <QObject>

class Tab2xml : public QObject
{
    Q_OBJECT
public:
    explicit Tab2xml(QObject *parent = 0);
    void convert(QString tabName, QString xmlPath);
    void setLangTag(const QString lng, QString &tag);
signals:
    
public slots:
    
};

#endif // TAB2XML_H
