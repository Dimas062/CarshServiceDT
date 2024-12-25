#include "xlspatterns.h"
#include <QFile>
#include <QStandardPaths>

QVector<QString> tempFiles;

QString GetTempFNameSchet()
{
    QString strTempFname(QStandardPaths::writableLocation(QStandardPaths::TempLocation));
    strTempFname.append("tmp.xls");

    QFile::remove(strTempFname);

    if(QFile::copy(":/templates/schet.xls", strTempFname))
        return strTempFname;

    return QString();
}

void WriteULsSchetInfo(QString strFname , QUuid uuidZakaz , QUuid uuidIspoln)
{

}

void DeleteTempFiles()
{
    foreach(QString strFile , tempFiles)
        QFile::remove(strFile);
}
