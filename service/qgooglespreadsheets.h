#ifndef QGOOGLESPREADSHEETS_H
#define QGOOGLESPREADSHEETS_H

#include <QObject>


class QGoogleSpreadSheets : public QObject
{
    Q_OBJECT
public:
    explicit QGoogleSpreadSheets(QObject *parent = nullptr);

signals:
};

#endif // QGOOGLESPREADSHEETS_H
