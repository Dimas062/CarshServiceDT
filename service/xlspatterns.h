#ifndef XLSPATTERNS_H
#define XLSPATTERNS_H

#include <QString>
#include <QVector>
#include <QUuid>



QString GetTempFNameSchet();

void WriteULsSchetInfo(QString strFname , QUuid uuidZakaz , QUuid uuidIspoln);

void DeleteTempFiles();

#endif // XLSPATTERNS_H
