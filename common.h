#ifndef COMMON_H
#define COMMON_H
#include <QImage>
#include <QString>

enum class PhotoTypes {DriverCard,    //Права
                        Passport,      //Паспорт
                        FLSert         //Свидетельство о ИП или самозанятости
};

enum     UserTypes {    Emploee,         //Сотрудник
                 CarshService,    //Карш-сервис
                 Carsh,           //Карш-служба
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

QString PictureFileToBase64(QString strPath);

QImage Base64ToImage(QString & base64Str);

QString ImageToBase64(const QImage & img);

#endif // COMMON_H
