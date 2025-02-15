#ifndef BDPATTERNS_H
#define BDPATTERNS_H

#include <QUuid>
#include "common.h"
#include <QImage>

#define NUMBER_BY_TASK  "(select Госномер  from \"Расширение задачи ШС\" where id=Задачи.Расширение union select Госномер from \"Расширение задачи Возврат в зону\" where id=Задачи.Расширение union select Госномер from \"Расширение задачи Номера\" where id=Задачи.Расширение union select Госномер from \"Расширение задачи Парковка\" where id=Задачи.Расширение)"

//Затраты сотрудников по задаче (оплата парковки, штрафстоянки, изготовления документов)
#define PAY_BY_TASK "(select SUM(\"Платежи сотрудников\".Сумма) from \"Платежи сотрудников\" where \
 \"Платежи сотрудников\".id in (select \"Расширение задачи ШС\".\"Оплата парковки\" from \"Расширение задачи ШС\" where \"Расширение задачи ШС\".id = Задачи.Расширение) or \
 \"Платежи сотрудников\".id in (select \"Расширение задачи Документы\".Оплата from \"Расширение задачи Документы\" where \"Расширение задачи Документы\".id = Задачи.Расширение) or\
 \"Платежи сотрудников\".id in (select \"Расширение задачи Парковка\".\"Оплата парковки\" from \"Расширение задачи Парковка\" where \"Расширение задачи Парковка\".id = Задачи.Расширение))"



QUuid CreatePayRecord(double dblSum , PayTypes iPayType);

void UpdatePayRecord(QUuid uuidPay , double dblSum , PayTypes iPayType);

QUuid CreatePayDocRecord(QUuid uuidPay , QString strImg);

void RemovePayDocs(QUuid uuidPay);

QUuid CreateTaskDocRecord(QUuid uuidTask , QString strImg , QUuid uuidType=QUuid("94ec977e-0be0-4dd7-b1fc-682af124e0ac"));

void RemoveTaskDocs(QUuid uuidTask);


QString StrFromUuid(QString strUuid , QString strTableName , QString strColName="Название");

#endif // BDPATTERNS_H



