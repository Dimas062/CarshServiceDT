#ifndef BDPATTERNS_H
#define BDPATTERNS_H

#define NUMBER_BY_TASK  "(select Госномер  from \"Расширение задачи ШС\" where id=Задачи.Расширение union select Госномер from \"Расширение задачи Возврат в зону\" where id=Задачи.Расширение union select Госномер from \"Расширение задачи Номера\" where id=Задачи.Расширение union select Госномер from \"Расширение задачи Парковка\" where id=Задачи.Расширение)"
#define PAY_BY_TASK "(select SUM(\"Платежи сотрудников\".Сумма) from \"Платежи сотрудников\" where \"Платежи сотрудников\".id in (select \"Расширение задачи ШС\".\"Оплата парковки\" from \"Расширение задачи ШС\" where \"Расширение задачи ШС\".id = Задачи.Расширение))"
#endif // BDPATTERNS_H
