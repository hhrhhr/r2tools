#ifndef TAB2CSV_H
#define TAB2CSV_H

#include <QObject>

class Tab2csv : public QObject
{
    Q_OBJECT
public:
    explicit Tab2csv(QObject *parent = 0);
    void convert(QString tabName, QString csvPath);
signals:
    
public slots:
    
};

#endif // TAB2CSV_H
