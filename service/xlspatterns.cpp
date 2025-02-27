#include "xlspatterns.h"
#include <QFile>
#include <QStandardPaths>
#include <QSqlQuery>
#include <QDate>
#include "common.h"

 #include "xlsxdocument.h"
// #include "xlsxchartsheet.h"
// #include "xlsxcellrange.h"
// #include "xlsxchart.h"
// #include "xlsxrichstring.h"
#include "xlsxworkbook.h"
using namespace QXlsx;

QVector<QString> tempFiles;

QString months(int num)
{
    switch (num) {
    case 1: return "января";
    case 2: return "февраля";
    case 3: return "марта";
    case 4: return "апреля";
    case 5: return "мая";
    case 6: return "июня";
    case 7: return "июля";
    case 8: return "августа";
    case 9: return "сентября";
    case 10: return "октября";
    case 11: return "ноября";
    case 12: return "декабря";
    default: return "месяца";
    }
}

QString GetTempFNameSchet()
{
    QString strTempFname(QStandardPaths::writableLocation(QStandardPaths::TempLocation));
    strTempFname.append("/tmp.xlsx");

    qDebug()<<"GetTempFNameSchet strTempFname="<<strTempFname;

    QFile::remove(strTempFname);

    QFile sourceFile(":/templates/schet.xlsx");

    if(sourceFile.copy(strTempFname))
    {
        QFile::setPermissions(strTempFname, QFileDevice::ReadOwner|QFileDevice::WriteOwner);
        return strTempFname;
    }
    qDebug()<<"no copy :/templates/schet.xlsx "<<sourceFile.errorString();

    return QString();
}


QString GetTempFNameAct()
{
    QString strTempFname(QStandardPaths::writableLocation(QStandardPaths::TempLocation));
    strTempFname.append("/tmpAct.xlsx");

    QFile::remove(strTempFname);

    if(QFile::copy(":/templates/act.xlsx", strTempFname))
    {
        QFile::setPermissions(strTempFname, QFileDevice::ReadOwner|QFileDevice::WriteOwner);
        return strTempFname;
    }

    return QString();
}

void WriteULsActInfo(QString strFname , QUuid uuidZakaz , QUuid uuidIspoln, QVector<SSchetItem> vItems , QString strPNum)
{
    int iHeaderSize = 12;
    QXlsx::Format bottomLineFormat;
    bottomLineFormat.setBottomBorderStyle(Format::BorderThin);
    QXlsx::Format rigthAlignFormat;
    rigthAlignFormat.setHorizontalAlignment(Format::AlignRight);
    QXlsx::Format centerAlignFormat;
    rigthAlignFormat.setHorizontalAlignment(Format::AlignHCenter);
    QXlsx::Format topCenterTextFormat;
    topCenterTextFormat.setHorizontalAlignment(Format::AlignHCenter);
    topCenterTextFormat.setVerticalAlignment(Format::AlignTop);
    topCenterTextFormat.setFontSize(8);

    int iItemCount = vItems.size();

    QXlsx::Document xlsx(strFname);
    if(xlsx.load())
    {
        QString strNumber = QString("АКТ №%1-%2-%3K от %4 %5 %6 г.").arg(QDate::currentDate().month()).arg(QDate::currentDate().year() - 2000).arg(strPNum).arg(QDate::currentDate().day()).arg(months(QDate::currentDate().month())).arg(QDate::currentDate().year());
        xlsx.write(3 , 2, QVariant(strNumber));

        QSqlQuery query;
        QString strIspolnQuery = QString("select Форма, Название, Адрес, ИНН, Банк, Счет, КПП, БИК, КоррСчет, ФамилияДиректора, ИмяДиректора ,ОтчествоДиректора from ЮЛ where id='%1'").arg(uuidIspoln.toString());
        QString strIspolnDirector;
        QString strIspolnUL;
        query.exec(strIspolnQuery);
        while(query.next())
        {
            QString strLetA = "";
            if(query.value(10).toString().length()>0) strLetA=query.value(10).toString().first(1);
            QString strLetB = "";
            if(query.value(11).toString().length()>0) strLetB=query.value(11).toString().first(1);
            strIspolnDirector = QString("%1 %2.%3.").arg(query.value(9).toString()).arg(strLetA).arg(strLetB);
            strIspolnUL = QString("%1 \"%2\"").arg(query.value(0).toString()).arg(query.value(1).toString());
            QString strIstpoln = query.value(0).toString();
            strIstpoln.append(" \"");
            strIstpoln.append(query.value(1).toString());
            strIstpoln.append(" \", ИНН ");
            strIstpoln.append(query.value(3).toString());
            strIstpoln.append(" , КПП ");
            strIstpoln.append(query.value(6).toString());
            strIstpoln.append(" , ");
            strIstpoln.append(query.value(2).toString());
            strIstpoln.append(" , р/с ");
            strIstpoln.append(query.value(5).toString());
            strIstpoln.append(" в банке ");
            strIstpoln.append(query.value(4).toString());
            strIstpoln.append(" БИК ");
            strIstpoln.append(query.value(7).toString());
            strIstpoln.append(" к/с ");
            strIstpoln.append(query.value(8).toString());
            xlsx.write(5 , 6, QVariant(strIstpoln));
        }

        QString strZakazQuery = QString("select Форма, Название, Адрес, ИНН, Банк, Счет, КПП, БИК, КоррСчет, ФамилияДиректора, ИмяДиректора ,ОтчествоДиректора from ЮЛ where id='%1'").arg(uuidZakaz.toString());
        QString strZakazDirector;
        QString strZakazUL;
        query.exec(strZakazQuery);
        while(query.next())
        {
            QString strLetA = "";
            if(query.value(10).toString().length()>0) strLetA=query.value(10).toString().first(1);
            QString strLetB = "";
            if(query.value(11).toString().length()>0) strLetB=query.value(11).toString().first(1);
            strZakazDirector = QString("%1 %2.%3.").arg(query.value(9).toString()).arg(strLetA).arg(strLetB);
            strZakazUL = QString("%1 \"%2\"").arg(query.value(0).toString()).arg(query.value(1).toString());
            QString strZakaz = query.value(0).toString();
            strZakaz.append(" \"");
            strZakaz.append(query.value(1).toString());
            strZakaz.append(" \", ИНН ");
            strZakaz.append(query.value(3).toString());
            strZakaz.append(" , КПП ");
            strZakaz.append(query.value(6).toString());
            strZakaz.append(" , ");
            strZakaz.append(query.value(2).toString());
            strZakaz.append(" , р/с ");
            strZakaz.append(query.value(5).toString());
            strZakaz.append(" в банке ");
            strZakaz.append(query.value(4).toString());
            strZakaz.append(" БИК ");
            strZakaz.append(query.value(7).toString());
            strZakaz.append(" к/с ");
            strZakaz.append(query.value(8).toString());
            xlsx.write(7 , 6, QVariant(strZakaz));
        }

        //Элементы акта
        int iItemCounter = 0;
        double dblSummCost = 0;//Общая стоимость
        double dblItemSummCount = 0;//Общее количество всех услуг
        foreach(SSchetItem item , vItems)
        {
            if(item.dblItemPrice == 0) continue;

            iItemCounter++;

            QXlsx::Format itemFormat;
            itemFormat.setHorizontalAlignment(Format::AlignHCenter);
            if(iItemCount == iItemCounter)
                itemFormat.setBottomBorderStyle(Format::BorderMedium);
            else itemFormat.setBottomBorderStyle(Format::BorderThin);
            itemFormat.setTopBorderStyle(Format::BorderThin);
            itemFormat.setRightBorderStyle(Format::BorderThin);
            itemFormat.setLeftBorderStyle(Format::BorderMedium);
            xlsx.mergeCells(CellRange(iHeaderSize + iItemCounter , 2 , iHeaderSize + iItemCounter, 3), itemFormat);
            xlsx.write( iHeaderSize + iItemCounter , 2, QVariant(iItemCounter) , itemFormat);

            itemFormat.setHorizontalAlignment(Format::AlignLeft);
            itemFormat.setLeftBorderStyle(Format::BorderThin);
            xlsx.mergeCells(CellRange(iHeaderSize + iItemCounter , 4 , iHeaderSize + iItemCounter, 20), itemFormat);
            xlsx.write( iHeaderSize + iItemCounter , 4 , QVariant(item.strName));

            itemFormat.setHorizontalAlignment(Format::AlignHCenter);
            xlsx.mergeCells(CellRange(iHeaderSize + iItemCounter , 21 , iHeaderSize + iItemCounter, 23), itemFormat);
            xlsx.write( iHeaderSize + iItemCounter , 21 , QVariant(item.dblCount) , itemFormat);
            dblItemSummCount = dblItemSummCount + item.dblCount;

            xlsx.mergeCells(CellRange(iHeaderSize + iItemCounter , 24 , iHeaderSize + iItemCounter, 25), itemFormat);
            xlsx.write( iHeaderSize + iItemCounter , 24 , QVariant(item.strUnitMeasure));

            xlsx.mergeCells(CellRange(iHeaderSize + iItemCounter , 26 , iHeaderSize + iItemCounter, 29), itemFormat);
            xlsx.write( iHeaderSize + iItemCounter , 26 , QVariant(QString::number(item.dblItemPrice, 'f', 2)));

            itemFormat.setRightBorderStyle(Format::BorderMedium);
            xlsx.mergeCells(CellRange(iHeaderSize + iItemCounter , 30 , iHeaderSize + iItemCounter, 33), itemFormat);
            xlsx.write( iHeaderSize + iItemCounter , 30 , QVariant(QString::number(item.dblItemPrice * item.dblCount, 'f', 2)));

            dblSummCost = dblSummCost + item.dblItemPrice * item.dblCount;
        }

        iItemCount = iItemCounter;

        QXlsx::Format cellFormat;
        cellFormat.setHorizontalAlignment(Format::AlignRight);

        QFont cellFont;
        cellFont.setFamily("Arial");
        cellFormat.setFont(cellFont);
        cellFormat.setFontBold(true);
        cellFormat.setFontSize(8);

        xlsx.mergeCells(CellRange(iHeaderSize + iItemCount + 2 , 28 , iHeaderSize + iItemCount + 2, 29), cellFormat);
        xlsx.write( iHeaderSize + iItemCount + 2 , 28 , QVariant("Итого:"),cellFormat);

        xlsx.mergeCells(CellRange(iHeaderSize + iItemCount + 2 , 30 , iHeaderSize + iItemCount + 2, 33), cellFormat);
        xlsx.write( iHeaderSize + iItemCount + 2 , 30 , QVariant(QString::number(dblSummCost, 'f', 2)), cellFormat);

        xlsx.mergeCells(CellRange(iHeaderSize + iItemCount + 3 , 24 , iHeaderSize + iItemCount + 3, 29), cellFormat);
        xlsx.write( iHeaderSize + iItemCount + 3 , 24 , QVariant("В том числе НДС::"),cellFormat);

        xlsx.mergeCells(CellRange(iHeaderSize + iItemCount + 3 , 30 , iHeaderSize + iItemCount + 3, 33), cellFormat);
        xlsx.write( iHeaderSize + iItemCount + 3 , 30 , QVariant(QString::number(0, 'f', 2)), cellFormat);

        cellFormat.setFontBold(false);
        cellFormat.setHorizontalAlignment(Format::AlignLeft);
        xlsx.mergeCells(CellRange(iHeaderSize + iItemCount + 5 , 2 , iHeaderSize + iItemCount + 5, 33), cellFormat);
        QString strItogo = QString("Всего оказано услуг %1, на сумму %2,00 руб.").arg(dblItemSummCount).arg(QString::number(dblSummCost , 'f', 2));
        xlsx.write( iHeaderSize + iItemCount + 5 , 2 , QVariant(strItogo),cellFormat);

        cellFormat.setFontBold(true);
        xlsx.mergeCells(CellRange(iHeaderSize + iItemCount + 6 , 2 , iHeaderSize + iItemCount + 6, 33), cellFormat);
        xlsx.write( iHeaderSize + iItemCount + 6 , 2 , QVariant(rublesForNumber(dblSummCost) + " 00 коп."), cellFormat);

        cellFormat.setFontBold(false);
        xlsx.mergeCells(CellRange(iHeaderSize + iItemCount + 8 , 2 , iHeaderSize + iItemCount + 8, 33), cellFormat);
        xlsx.write( iHeaderSize + iItemCount + 8 , 2 , QVariant("Вышеперечисленные услуги выполнены полностью и в срок. Заказчик претензий по объему, качеству и срокам"), cellFormat);
        xlsx.mergeCells(CellRange(iHeaderSize + iItemCount + 9 , 2 , iHeaderSize + iItemCount + 9, 33), cellFormat);
        xlsx.write( iHeaderSize + iItemCount + 9 , 2 , QVariant("оказания услуг не имеет."), cellFormat);

        bottomLineFormat.setBottomBorderStyle(Format::BorderMedium);
        xlsx.mergeCells(CellRange(iHeaderSize + iItemCount + 10 , 2 , iHeaderSize + iItemCount + 10, 33), bottomLineFormat);

        cellFormat.setFontBold(true);
        xlsx.mergeCells(CellRange(iHeaderSize + iItemCount + 12 , 2 , iHeaderSize + iItemCount + 12, 18), cellFormat);
        xlsx.write( iHeaderSize + iItemCount + 12 , 2 , QVariant("ИСПОЛНИТЕЛЬ"), cellFormat);

        xlsx.mergeCells(CellRange(iHeaderSize + iItemCount + 12 , 21 , iHeaderSize + iItemCount + 12, 33), cellFormat);
        xlsx.write( iHeaderSize + iItemCount + 12 , 21 , QVariant("ЗАКАЗЧИК"), cellFormat);

        cellFormat.setFontBold(false);
        xlsx.mergeCells(CellRange(iHeaderSize + iItemCount + 13 , 2 , iHeaderSize + iItemCount + 13, 18), cellFormat);
        xlsx.write( iHeaderSize + iItemCount + 13 , 2 , QVariant(strIspolnUL), cellFormat);

        xlsx.mergeCells(CellRange(iHeaderSize + iItemCount + 13 , 21 , iHeaderSize + iItemCount + 13, 33), cellFormat);
        xlsx.write( iHeaderSize + iItemCount + 13 , 21 , QVariant(strZakazUL), cellFormat);

        topCenterTextFormat.setTopBorderStyle(Format::BorderThin);
        xlsx.mergeCells(CellRange(iHeaderSize + iItemCount + 15 , 2 , iHeaderSize + iItemCount + 15, 18), topCenterTextFormat);
        xlsx.write( iHeaderSize + iItemCount + 15 , 2 , QVariant(strIspolnDirector), topCenterTextFormat);

        xlsx.mergeCells(CellRange(iHeaderSize + iItemCount + 15 , 21 , iHeaderSize + iItemCount + 15, 33), topCenterTextFormat);
        xlsx.write( iHeaderSize + iItemCount + 15 , 21 , QVariant(strZakazDirector), topCenterTextFormat);

        if(!xlsx.save())  qDebug()<<"QXlsx temp save error";
    }
    else qDebug()<<"QXlsx not load file "<<strFname;
}


void WriteULsSchetInfo(QString strFname , QUuid uuidZakaz , QUuid uuidIspoln, QVector<SSchetItem> vItems , QString strPNum)
{
    int iHeaderSize = 21;
    QXlsx::Format bottomLineFormat;
    bottomLineFormat.setBottomBorderStyle(Format::BorderThin);
    QXlsx::Format rigthAlignFormat;
    rigthAlignFormat.setHorizontalAlignment(Format::AlignRight);
    QXlsx::Format centerAlignFormat;
    rigthAlignFormat.setHorizontalAlignment(Format::AlignHCenter);
    QXlsx::Format topSmallTextFormat;
    topSmallTextFormat.setHorizontalAlignment(Format::AlignHCenter);
    topSmallTextFormat.setVerticalAlignment(Format::AlignTop);
    topSmallTextFormat.setFontSize(7);

    int iItemCount = vItems.size();

    QXlsx::Document xlsx(strFname);
    if(xlsx.load())
    {

        QSqlQuery query;

        QString strZakazQuery = QString("select Форма, Название, Адрес, ИНН, Банк, Счет, КПП, БИК, КоррСчет, ФамилияДиректора, ИмяДиректора ,ОтчествоДиректора from ЮЛ where id='%1'").arg(uuidZakaz.toString());
        qDebug()<<"strZakazQuery ="<<strZakazQuery;
        query.exec(strZakazQuery);

        while(query.next())
        {
            QVariant varZakazPoluchatel = QString("%1 %2 ИНН:%4  Адрес:%5").arg(query.value(0).toString()).arg(query.value(1).toString()).arg(query.value(3).toString()).arg(query.value(2).toString());
            xlsx.write(18,4,varZakazPoluchatel);//Покупатель
        }

        QString strNumber = QString("%1-%2-%3K").arg(QDate::currentDate().month()).arg(QDate::currentDate().year() - 2000).arg(strPNum);
        xlsx.write(13 , 5, QVariant(strNumber));

        //Дата
        xlsx.write(13 , 9, QVariant(QDate::currentDate().day()));
        xlsx.write(13 , 11, QVariant(months(QDate::currentDate().month())));
        xlsx.write(13 , 13, QVariant(QDate::currentDate().year() - 2000));

        //Элементы счета
        int iItemCounter = 0;
        double dblSummCost = 0;//Общая стоимость
        double dblItemSummCount = 0;//Общее количество всех услуг
        foreach(SSchetItem item , vItems)
        {
            if(item.dblItemPrice == 0) continue;

            iItemCounter++;

            QXlsx::Format itemFormat;
            itemFormat.setHorizontalAlignment(Format::AlignHCenter);
            itemFormat.setBottomBorderStyle(Format::BorderThin);
            itemFormat.setTopBorderStyle(Format::BorderThin);
            itemFormat.setRightBorderStyle(Format::BorderThin);
            itemFormat.setLeftBorderStyle(Format::BorderMedium);
            xlsx.write( iHeaderSize + iItemCounter , 1, QVariant(iItemCounter) , itemFormat);

            itemFormat.setHorizontalAlignment(Format::AlignLeft);
            itemFormat.setLeftBorderStyle(Format::BorderThin);
            xlsx.mergeCells(CellRange(iHeaderSize + iItemCounter , 2 , iHeaderSize + iItemCounter, 6), itemFormat);
            xlsx.write( iHeaderSize + iItemCounter , 2 , QVariant(item.strName));

            itemFormat.setHorizontalAlignment(Format::AlignHCenter);
            xlsx.mergeCells(CellRange(iHeaderSize + iItemCounter , 7 , iHeaderSize + iItemCounter, 8), itemFormat);
            xlsx.write( iHeaderSize + iItemCounter , 7 , QVariant(item.strUnitMeasure));

            xlsx.write( iHeaderSize + iItemCounter , 9 , QVariant(item.dblCount) , itemFormat);
            dblItemSummCount = dblItemSummCount + item.dblCount;

            xlsx.mergeCells(CellRange(iHeaderSize + iItemCounter , 10 , iHeaderSize + iItemCounter, 11), itemFormat);
            xlsx.write( iHeaderSize + iItemCounter , 10 , QVariant(QString::number(item.dblItemPrice, 'f', 2)));

            itemFormat.setRightBorderStyle(Format::BorderMedium);
            xlsx.mergeCells(CellRange(iHeaderSize + iItemCounter , 12 , iHeaderSize + iItemCounter, 14), itemFormat);
            xlsx.write( iHeaderSize + iItemCounter , 12 , QVariant(QString::number(item.dblItemPrice * item.dblCount, 'f', 2)));
            dblSummCost = dblSummCost + item.dblItemPrice * item.dblCount;
        }

        iItemCount = iItemCounter;

        QXlsx::Format cellFormat;
        cellFormat.setHorizontalAlignment(Format::AlignRight);
        cellFormat.setFontBold(true);
        QFont cellFont;
        cellFont.setFamily("Arial");
        cellFormat.setFont(cellFont);
        cellFormat.setFontSize(10);
        xlsx.mergeCells(CellRange(iHeaderSize + iItemCount + 1, 10 , iHeaderSize + iItemCount + 1, 11), cellFormat);
        xlsx.write( iHeaderSize + iItemCount + 1, 10, QVariant("Итого:"));

        cellFormat.setHorizontalAlignment(Format::AlignHCenter);
        cellFormat.setBottomBorderStyle(Format::BorderMedium);
        cellFormat.setTopBorderStyle(Format::BorderMedium);
        cellFormat.setRightBorderStyle(Format::BorderMedium);
        cellFormat.setLeftBorderStyle(Format::BorderMedium);
        cellFormat.setFontBold(false);
        xlsx.mergeCells(CellRange(iHeaderSize + iItemCount + 1, 12 , iHeaderSize + iItemCount + 1, 14), cellFormat);
        xlsx.write( iHeaderSize + iItemCount + 1, 12, QVariant(QString::number(dblSummCost, 'f', 2)));

        cellFormat.setHorizontalAlignment(Format::AlignRight);
        cellFormat.setFontBold(true);
        xlsx.mergeCells(CellRange(iHeaderSize + iItemCount + 2, 10 , iHeaderSize + iItemCount + 2, 11), cellFormat);
        xlsx.write( iHeaderSize + iItemCount + 2, 10, QVariant("Итого с НДС:"));

        cellFormat.setHorizontalAlignment(Format::AlignHCenter);
        cellFormat.setBottomBorderStyle(Format::BorderMedium);
        cellFormat.setTopBorderStyle(Format::BorderMedium);
        cellFormat.setRightBorderStyle(Format::BorderMedium);
        cellFormat.setLeftBorderStyle(Format::BorderMedium);
        cellFormat.setFontBold(false);
        xlsx.mergeCells(CellRange(iHeaderSize + iItemCount + 2, 12 , iHeaderSize + iItemCount + 2, 14), cellFormat);
        xlsx.write( iHeaderSize + iItemCount + 2, 12, QVariant(QString::number(0, 'f', 2)));

        cellFormat.setHorizontalAlignment(Format::AlignRight);
        cellFormat.setFontBold(true);
        xlsx.mergeCells(CellRange(iHeaderSize + iItemCount + 3, 10 , iHeaderSize + iItemCount + 3, 11), cellFormat);
        xlsx.write( iHeaderSize + iItemCount + 3, 10, QVariant("Всего к оплате:"));

        cellFormat.setHorizontalAlignment(Format::AlignHCenter);
        cellFormat.setBottomBorderStyle(Format::BorderMedium);
        cellFormat.setTopBorderStyle(Format::BorderMedium);
        cellFormat.setRightBorderStyle(Format::BorderMedium);
        cellFormat.setLeftBorderStyle(Format::BorderMedium);
        cellFormat.setFontBold(false);
        xlsx.mergeCells(CellRange(iHeaderSize + iItemCount + 3, 12 , iHeaderSize + iItemCount + 3, 14), cellFormat);
        xlsx.write( iHeaderSize + iItemCount + 3, 12, QVariant(QString::number(dblSummCost, 'f', 2)));

        cellFormat.setHorizontalAlignment(Format::AlignRight);
        cellFormat.setBottomBorderStyle(Format::BorderNone);
        cellFormat.setTopBorderStyle(Format::BorderNone);
        cellFormat.setRightBorderStyle(Format::BorderNone);
        cellFormat.setLeftBorderStyle(Format::BorderNone);
        xlsx.mergeCells(CellRange(iHeaderSize + iItemCount + 5, 1 , iHeaderSize + iItemCount + 5, 4), cellFormat);
        xlsx.write( iHeaderSize + iItemCount + 5, 1, QVariant("Всего наименований:"));

        cellFormat.setBottomBorderStyle(Format::BorderThin);
        cellFormat.setHorizontalAlignment(Format::AlignHCenter);
        xlsx.write( iHeaderSize + iItemCount + 5, 5, QVariant(QString::number(dblItemSummCount, 'f', 2)) , cellFormat);

        cellFormat.setHorizontalAlignment(Format::AlignHCenter);
        cellFormat.setBottomBorderStyle(Format::BorderNone);
        xlsx.mergeCells(CellRange(iHeaderSize + iItemCount + 5, 6 , iHeaderSize + iItemCount + 5, 8), cellFormat);
        xlsx.write( iHeaderSize + iItemCount + 5, 6, QVariant("на сумму"));

        cellFormat.setBottomBorderStyle(Format::BorderThin);
        cellFormat.setHorizontalAlignment(Format::AlignHCenter);
        xlsx.mergeCells(CellRange(iHeaderSize + iItemCount + 5, 9 , iHeaderSize + iItemCount + 5, 12), cellFormat);
        xlsx.write( iHeaderSize + iItemCount + 5, 9, QVariant(QString::number(dblSummCost, 'f', 2)));

        xlsx.write( iHeaderSize + iItemCount + 5, 13, QVariant("руб."));

        cellFormat.setFontBold(true);
        cellFormat.setBottomBorderStyle(Format::BorderThin);
        cellFormat.setHorizontalAlignment(Format::AlignHCenter);
        xlsx.mergeCells(CellRange(iHeaderSize + iItemCount + 6, 1 , iHeaderSize + iItemCount + 6, 12), cellFormat);
        xlsx.write( iHeaderSize + iItemCount + 6, 1, QVariant(rublesForNumber(dblSummCost) + " 00 коп."));

        cellFormat.setBottomBorderStyle(Format::BorderNone);
        xlsx.write( iHeaderSize + iItemCount + 6, 13, QVariant("руб."),cellFormat);
        xlsx.write( iHeaderSize + iItemCount + 6, 14, QVariant("00"),cellFormat);
        cellFormat.setHorizontalAlignment(Format::AlignLeft);
        xlsx.write( iHeaderSize + iItemCount + 6, 15, QVariant("коп."),cellFormat);

        //маленькие подписи под строками директора
        xlsx.mergeCells(CellRange(iHeaderSize + iItemCount + 10 , 5 , iHeaderSize + iItemCount + 10, 7) , topSmallTextFormat);
        xlsx.write( iHeaderSize + iItemCount + 10, 5, QVariant("подпись"), topSmallTextFormat);
        xlsx.mergeCells(CellRange(iHeaderSize + iItemCount + 10 , 9 , iHeaderSize + iItemCount + 10, 12) , topSmallTextFormat);
        xlsx.write( iHeaderSize + iItemCount + 10, 9, QVariant("Ф.И.О."), topSmallTextFormat);

        //маленькие подписи под строками главбуха
        xlsx.mergeCells(CellRange(iHeaderSize + iItemCount + 13 , 5 , iHeaderSize + iItemCount + 13, 7) , topSmallTextFormat);
        xlsx.write( iHeaderSize + iItemCount + 13, 5, QVariant("подпись"), topSmallTextFormat);
        xlsx.mergeCells(CellRange(iHeaderSize + iItemCount + 13 , 9 , iHeaderSize + iItemCount + 13, 12) , topSmallTextFormat);
        xlsx.write( iHeaderSize + iItemCount + 13, 9, QVariant("Ф.И.О."), topSmallTextFormat);

        xlsx.write( iHeaderSize + iItemCount + 16, 4, QVariant("М.П."));

        QString strIspolnQuery = QString("select Форма, Название, Адрес, ИНН, Банк, Счет, КПП, БИК, КоррСчет, ФамилияДиректора, ИмяДиректора ,ОтчествоДиректора from ЮЛ where id='%1'").arg(uuidIspoln.toString());

        query.exec(strIspolnQuery);
        while(query.next())
        {
            QVariant varPoluchatel = QString("%1 %2").arg(query.value(0).toString()).arg(query.value(1).toString());
            xlsx.write(9,1,varPoluchatel);//Получатель денег
            xlsx.write(7,3,query.value(3));//ИНН
            xlsx.write(11,1,query.value(4));//Банк
            xlsx.write(9,11,query.value(5));//Счет
            xlsx.write(10,11,query.value(7));//БИК
            xlsx.write(11,11,query.value(8));//КоррСчет
            xlsx.write(7,5,query.value(6));//КПП
            QString strLetA = "";
            if(query.value(10).toString().length()>0) strLetA=query.value(10).toString().first(1);
            QString strLetB = "";
            if(query.value(11).toString().length()>0) strLetB=query.value(11).toString().first(1);
            QVariant varDirector = QString("%1 %2.%3.").arg(query.value(9).toString()).arg(strLetA).arg(strLetB);

            //Подписи
            xlsx.mergeCells(CellRange(iHeaderSize + iItemCount + 9 , 1 , iHeaderSize + iItemCount + 9, 4));
            xlsx.write(iHeaderSize + iItemCount + 9 , 1 , QVariant("Руководитель предприятия") , rigthAlignFormat);
            xlsx.mergeCells(CellRange(iHeaderSize + iItemCount + 9 , 5 , iHeaderSize + iItemCount + 9, 7), bottomLineFormat);
            xlsx.mergeCells(CellRange(iHeaderSize + iItemCount + 9 , 9 , iHeaderSize + iItemCount + 9, 12), bottomLineFormat);
            xlsx.write(iHeaderSize + iItemCount + 9 , 9 , varDirector);//Директор

            xlsx.mergeCells(CellRange(iHeaderSize + iItemCount + 12 , 1 , iHeaderSize + iItemCount + 12, 4));
            xlsx.write(iHeaderSize + iItemCount + 12 , 1 , QVariant("Главный бухгалтер") , rigthAlignFormat);
            xlsx.mergeCells(CellRange(iHeaderSize + iItemCount + 12 , 5 , iHeaderSize + iItemCount + 12, 7), bottomLineFormat);
            xlsx.mergeCells(CellRange(iHeaderSize + iItemCount + 12 , 9 , iHeaderSize + iItemCount + 12, 12), bottomLineFormat);
            xlsx.write(iHeaderSize + iItemCount + 12 , 9 , varDirector);//Главбух

            QVariant varFullPoluchatel = QString("%1 %2 ИНН:%4  Адрес:%5").arg(query.value(0).toString()).arg(query.value(1).toString()).arg(query.value(3).toString()).arg(query.value(2).toString());
            xlsx.write(16,4,varFullPoluchatel);//Поставщик (получаль денег)
        }

        if(!xlsx.save())  qDebug()<<"QXlsx temp save error";

    }
    else qDebug()<<"QXlsx not load file "<<strFname;
}

void DeleteTempFiles()
{
    foreach(QString strFile , tempFiles)
        QFile::remove(strFile);
}
