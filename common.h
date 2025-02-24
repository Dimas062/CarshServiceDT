#ifndef COMMON_H
#define COMMON_H
#include <QImage>
#include <QString>
#include <QSqlQuery>

QString bool_to_str(bool b);


enum class PhotoTypes {DriverCard,    //Права
                        Passport,      //Паспорт
                        FLSert         //Свидетельство о ИП или самозанятости
};

enum     UserTypes {    Emploee,         //Сотрудник
                 CarshService,    //Карш-сервис
                 Carsh,           //Карш-служба (сами машины)
                 PartnerWasher,   //Партнёр мойка
                 PartnerPlate,    //Партнер номера
                 PartnerStick,    //Партнер оклейка
                 UndefinedUserType
};

enum PayTypes {
    Card,
    Business,
    Undefined = -1
};

bool executeQueryWithReconnect(QSqlQuery &query, const QString &queryString);

QString GenNextShcetActNumber();

QString PictureFileToBase64(QString strPath);

QImage Base64ToImage(QString & base64Str);

QString ImageToBase64(const QImage & img);

QString rublesForNumber(long long number);

#endif // COMMON_H
