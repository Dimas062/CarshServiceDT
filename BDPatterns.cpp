#include "BDPatterns.h"
#include <QSqlQuery>


QUuid CreatePayRecord(double dblSum , PayTypes iPayType, qint64 iDate)
{
    QUuid uuidPay = QUuid::createUuid();

    QString uuidPayType;

    if(iPayType == Card) uuidPayType=QString("1368c945-6d5b-4a32-af29-007896fc5aba");
    if(iPayType == Business) uuidPayType=QString("c1a98e3d-c6a9-4991-9c2c-2e8771ccf37f");
    if(iPayType == Undefined) uuidPayType=QString("1be45f61-dd36-4209-9866-7b8c2987da34");

    QString strExec = QString("insert into \"Платежи сотрудников\" (id,\"Сумма\",\"Тип оплаты\",\"ДатаВремя\") values ('%1','%2','%3','%4')").arg(uuidPay.toString()).arg(dblSum).arg(uuidPayType).arg(iDate);

    QSqlQuery query;
    query.exec(strExec);

    return uuidPay;
}

void UpdatePayRecord(QUuid uuidPay , double dblSum , PayTypes iPayType, qint64 iDate)
{
    QString uuidPayType;

    if(iPayType == Card) uuidPayType=QString("1368c945-6d5b-4a32-af29-007896fc5aba");
    if(iPayType == Business) uuidPayType=QString("c1a98e3d-c6a9-4991-9c2c-2e8771ccf37f");
    if(iPayType == Undefined) uuidPayType=QString("1be45f61-dd36-4209-9866-7b8c2987da34");

    QString strExec = QString("update \"Платежи сотрудников\" set \"Сумма\" = '%1' ,\"Тип оплаты\"='%2',\"ДатаВремя\"='%3'  where id='%4'").arg(dblSum).arg(uuidPayType).arg(iDate).arg(uuidPay.toString());

    QSqlQuery query;
    query.exec(strExec);
}


QUuid CreatePayDocRecord(QUuid uuidPay , QString strImg)
{
    QUuid uuidDoc = QUuid::createUuid();
    QUuid uuidPayDoc = QUuid::createUuid();

    QString strExec = QString("insert into \"Документы\" (id,\"Тип\",\"Изображение\") values ('%1','07763912-00d4-49e0-8888-a83dec0f18b4','%2')").arg(uuidDoc.toString()).arg(strImg);
    QSqlQuery query;
    query.exec(strExec);

    strExec = QString("insert into \"Платеж сотрудника - Документы\" (id,\"Платеж\",\"Документ\") values ('%1','%2','%3')").arg(uuidPayDoc.toString()).arg(uuidPay.toString()).arg(uuidDoc.toString());
    query.exec(strExec);

    return uuidPayDoc;
}

//Возвращает гуид созданного документа (не связки документ-задача)
QUuid CreateTaskDocRecord(QUuid uuidTask , QString strImg , QUuid uuidType)
{
    QUuid uuidDoc = QUuid::createUuid();
    QUuid uuidTaskDoc = QUuid::createUuid();

    QString strExec = QString("insert into \"Документы\" (id,\"Тип\",\"Изображение\") values ('%1','%2','%3')").arg(uuidDoc.toString()).arg(uuidType.toString()).arg(strImg);
    execMainBDQueryUpdate(strExec);

    strExec = QString("insert into \"Задача-Документы задач\" (id,\"Задача\",\"Документ\") values ('%1','%2','%3')").arg(uuidTaskDoc.toString()).arg(uuidTask.toString()).arg(uuidDoc.toString());
    execMainBDQueryUpdate(strExec);

    return uuidDoc;
}

QUuid CopyDocToTask(QUuid uuidTask , QUuid uuidSrcDoc)
{
    QUuid uuidDoc = QUuid::createUuid();
    QUuid uuidTaskDoc = QUuid::createUuid();

    QString strExec = QString("INSERT INTO \"Документы\" (id, \"Тип\", \"Изображение\") SELECT '%1', \"Тип\", \"Изображение\" FROM \"Документы\" WHERE id = '%2'").arg(uuidDoc.toString()).arg(uuidSrcDoc.toString());
    execMainBDQueryUpdate(strExec);

    strExec = QString("insert into \"Задача-Документы задач\" (id,\"Задача\",\"Документ\") values ('%1','%2','%3')").arg(uuidTaskDoc.toString()).arg(uuidTask.toString()).arg(uuidDoc.toString());
    execMainBDQueryUpdate(strExec);

    return uuidDoc;
}

void RemoveTaskDocs(QUuid uuidTask)
{
    QString strExec = QString("delete from \"Документы\" where Документы.id in (select \"Задача-Документы задач\".Документ from \"Задача-Документы задач\" where \"Задача-Документы задач\".Задача ='%1')").arg(uuidTask.toString());
    QSqlQuery query;
    query.exec(strExec);

    strExec = QString("delete from \"Задача-Документы задач\" where Задача='%1'").arg(uuidTask.toString());
    query.exec(strExec);
}

void RemovePayDocs(QUuid uuidPay)
{
    QString strExec = QString("delete from \"Документы\" where Документы.id in (select \"Платеж сотрудника - Документы\".Документ from \"Платеж сотрудника - Документы\" where \"Платеж сотрудника - Документы\".Платеж ='%1')").arg(uuidPay.toString());
    QSqlQuery query;
    query.exec(strExec);

    strExec = QString("delete from \"Платеж сотрудника - Документы\" where Платеж='%1'").arg(uuidPay.toString());
    query.exec(strExec);
}

QString StrFromUuid(QString strUuid , QString strTableName, QString strColName)
{
    QString strExec = QString("select %1 from %2 where id='%3'").arg(strColName).arg(strTableName).arg(strUuid);
    QSqlQuery query;
    query.exec(strExec);
    if(query.next())
        return query.value(0).toString();
    return QString("");
}
