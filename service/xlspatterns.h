#ifndef XLSPATTERNS_H
#define XLSPATTERNS_H

#include <QString>
#include <QVector>
#include <QUuid>

struct SSchetItem
{
    QString strName;
    QString strUnitMeasure;
    double dblCount;
    double dblItemPrice;
};


QString GetTempFNameSchet();
QString GetTempFNameAct();


void WriteULsSchetInfo(QString strFname , QUuid uuidZakaz , QUuid uuidIspoln, QVector<SSchetItem> vItems, QString strPNum);
void WriteULsActInfo(QString strFname , QUuid uuidZakaz , QUuid uuidIspoln, QVector<SSchetItem> vItems, QString strPNum);

void DeleteTempFiles();

#endif // XLSPATTERNS_H
