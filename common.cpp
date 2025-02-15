#include "common.h"
#include <QBuffer>
#include <QDebug>
#include <QSqlQuery>
#include <QDate>


QString bool_to_str(bool b)
{
    if(b) return "true";
    else return "false";
}

QString PictureFileToBase64(QString strPath)
{
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    QImage qp(strPath);

    qp.save(&buffer, "JPG");

    return buffer.data().toBase64();
}

QString ImageToBase64(const QImage & img)
{
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);

    img.save(&buffer, "JPG");

    return buffer.data().toBase64();
}

QImage Base64ToImage(QString & base64Str)
{
    //TODO:  дописать

    QByteArray arr = QByteArray::fromBase64(base64Str.toLatin1());

    return QImage::fromData(arr, "JPG");

}

QString GenNextShcetActNumber()
{
    QString strUuidMonth("f149f490-dae9-4171-a967-68644e952c12");
    QString strUuidNumber("e408ffca-c786-4009-b1a1-a270c9a0d925");
    QSqlQuery query;
    int iMonth = 0;
    int iNumber = 0;

    query.exec(QString("select Значение from Настройки where id='%1'").arg(strUuidMonth));
    while(query.next()) iMonth = query.value(0).toInt();

    query.exec(QString("select Значение from Настройки where id='%1'").arg(strUuidNumber));
    while(query.next()) iNumber = query.value(0).toInt();

    int iCurMonth = QDate::currentDate().month();
    if(iMonth == iCurMonth) iNumber++;
    else
    {
        iMonth = iCurMonth;
        iNumber = 1;
    }

    query.exec(QString("update Настройки set Значение ='%1' where id='%2'").arg(iNumber).arg(strUuidNumber));
    query.exec(QString("update Настройки set Значение ='%1' where id='%2'").arg(iMonth).arg(strUuidMonth));

    return QString("%1").arg(iNumber);
}

//----
/*Перевод числа в строку-слова*/
//----
std::vector<QString> rubles = { QString(" рубль"), QString(" рубля"), QString(" рублей") };
std::vector<QString> thousands = { " тысяча", " тысячи", " тысяч" };
std::vector<QString> millions = { " миллион", " миллиона", " миллионов" };


std::vector<QString> one = { "", " одна", " две", " три", " четыре", " пять", " шесть", " семь", " восемь", " девять" };
std::vector<QString> ones = { "", " один", " два", " три", " четыре", " пять", " шесть", " семь", " восемь", " девять" };
std::vector<QString> teens = { " десять", " одиннадцать", " двенадцать", " тринадцать", " четырнадцать", " пятнадцать", " шестнадцать", " семнадцать", " восемнадцать", " девятнадцать" };
std::vector<QString> tens = { "", "", " двадцать", " тридцать", " сорок", " пятьдесят", " шестьдесят", " семьдесят", " восемьдесят", " девяносто" };
std::vector<QString> hundredth = { ""," сто", " двести", " триста", " четыреста", " пятьсот", " шестьсот", " семьсот", " восемьсот", " девятьсот",};

QString Ending(long long number , std::vector<QString> ends) {
    int lastDigit = number % 10;
    int penultimateDigit = (number / 10) % 10;

    if (penultimateDigit == 1) {  // Для чисел 10-19 всегда "тысяч"
        return ends[2];
    }
    else if (lastDigit == 1) {  // Для чисел, оканчивающихся на 1 (кроме 11), "тысяча"
        return ends[0];
    }
    else if (lastDigit >= 2 && lastDigit <= 4) {  // Для чисел, оканчивающихся на 2-4 (кроме 12-14), "тысячи"
        return ends[1];
    }
    else {  // Во всех остальных случаях "тысяч"
        return ends[2];
    }
}

QString nameForNumber(long long number, bool flag=false) {
    //единицы
    if (number < 10) {
        return flag ? one[number] : ones[number];
    }
    //числительные
    else if (number < 20) {
        return teens[number - 10];
    }
    //десятки
    else if (number < 100) {
        return tens[number / 10] + nameForNumber(number % 10);
    }
    //сотни
    else if (number < 1000) {
        return hundredth[number / 100] +  nameForNumber(number % 100);
    }
    //тысячи
    else if (number < 1000000) {
        return nameForNumber(number / 1000, true) + Ending(number / 1000 , thousands) + nameForNumber(number % 1000);//Тут ошибка от 10к до 100к
    }
    //миллионы
    else if (number < 1000000000) {
        return nameForNumber(number / 1000000) + Ending(number / 1000000 , millions) + nameForNumber(number % 1000000);
    }
    //миллиарды
    else if (number < 1000000000000) {
        return nameForNumber(number / 1000000000) + " миллиардов" + nameForNumber(number % 1000000000);//еще не доделалано
    }
    return "error";
}

QString rublesForNumber(long long number)
{
    QString rVal = nameForNumber(number) + Ending(number % 100 , rubles);
    rVal.removeFirst();
    rVal.front() = rVal.front().toUpper();

    return rVal;
}
//----
/*Перевод числа в строку-слова*/
//----


