#ifndef TAB2XML_H
#define TAB2XML_H

#include <QObject>
#include <QStringList>

class Tab2txt : public QObject
{
    Q_OBJECT

    QString m_txtPath;
    QString basename;
    QList<QStringList> cols;

    void save2cvs();
    void save2xml();
    void setLangTag(const QString lng, QString &tag);

public:
    explicit Tab2txt(QObject *parent = 0);
    void convert(QString tabName, QString txtPath, QString outType);

signals:
    
public slots:
    
};

#endif // TAB2XML_H
