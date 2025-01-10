#include "quleditdlg.h"
#include <QVBoxLayout>
#include <QString>
#include <QSqlQuery>
#include <QPushButton>
#include <QUuid>


QULEditDlg::QULEditDlg()
{


    QVBoxLayout * pVMainLayout = new QVBoxLayout;

    m_pFormaLineText = new QLineText("Форма (ООО, ИП)");
    pVMainLayout->addWidget(m_pFormaLineText);

    m_pNazvanieLineText = new QLineText("Название");
    pVMainLayout->addWidget(m_pNazvanieLineText);

    m_pAdressLineText = new QLineText("Адрес");
    pVMainLayout->addWidget(m_pAdressLineText);

    m_pINNLineText = new QLineText("ИНН");
    pVMainLayout->addWidget(m_pINNLineText);

    m_pBankLineText = new QLineText("Банк");
    pVMainLayout->addWidget(m_pBankLineText);

    m_pSchetLineText = new QLineText("Счет");
    pVMainLayout->addWidget(m_pSchetLineText);

    m_pKPPLineText = new QLineText("КПП");
    pVMainLayout->addWidget(m_pKPPLineText);

    m_pBIKLineText = new QLineText("БИК");
    pVMainLayout->addWidget(m_pBIKLineText);

    m_pKorrSchetLineText = new QLineText("Корр. счет");
    pVMainLayout->addWidget(m_pKorrSchetLineText);

    m_pFamDirLineText = new QLineText("Фамилия директора");
    pVMainLayout->addWidget(m_pFamDirLineText);

    m_pNameDirLineText = new QLineText("Имя директора");
    pVMainLayout->addWidget(m_pNameDirLineText);

    m_pOtchDirLineText = new QLineText("Отчество директора");
    pVMainLayout->addWidget(m_pOtchDirLineText);


    QHBoxLayout *pButtonsLayout = new QHBoxLayout;


    QPushButton * pApplyCardButton = new QPushButton("Применить");
    connect(pApplyCardButton,SIGNAL(pressed()),this,SLOT(OnApplyCardPressed()));
    pButtonsLayout->addWidget(pApplyCardButton);

    QPushButton * pDeleteCardButton = new QPushButton("Отменить");
    connect(pDeleteCardButton,SIGNAL(pressed()),this,SLOT(reject()));
    pButtonsLayout->addWidget(pDeleteCardButton);

    pVMainLayout->addLayout(pButtonsLayout);

    this->setLayout(pVMainLayout);

}


void QULEditDlg::LoadFromBD(QString strIdUL)
{
    m_strIdUL = strIdUL;
    QString strQuery = QString("select Название, Адрес, ИНН, Банк, Счет, КПП, БИК, КоррСчет, ФамилияДиректора, ИмяДиректора, ОтчествоДиректора, Форма from ЮЛ where id='%1' ").arg(m_strIdUL);
    QSqlQuery query;
    query.exec(strQuery);
    while(query.next())
    {
        m_pNazvanieLineText->setText(query.value(0).toString());
        m_pAdressLineText->setText(query.value(1).toString());
        m_pINNLineText->setText(query.value(2).toString());
        m_pBankLineText->setText(query.value(3).toString());
        m_pSchetLineText->setText(query.value(4).toString());
        m_pKPPLineText->setText(query.value(5).toString());
        m_pBIKLineText->setText(query.value(6).toString());
        m_pKorrSchetLineText->setText(query.value(7).toString());
        m_pFamDirLineText->setText(query.value(8).toString());
        m_pNameDirLineText->setText(query.value(9).toString());
        m_pOtchDirLineText->setText(query.value(10).toString());
        m_pFormaLineText->setText(query.value(11).toString());
    }
}

QString QULEditDlg::SaveToBD()
{
    if(m_strIdUL == QString())
    {
        QString strUid = QUuid::createUuid().toString();
        m_strIdUL = strUid;
        QString strQuery = QString("insert into ЮЛ (id, Название, Адрес, ИНН, Банк, Счет, КПП, БИК, КоррСчет, ФамилияДиректора, ИмяДиректора, ОтчествоДиректора , Форма) values ('%1' , '%2' ,'%3' ,'%4' ,'%5' ,'%6' ,'%7' ,'%8' ,'%9' ,'%10' ,'%11' ,'%12' ,'%13')  ").arg(strUid).arg(m_pNazvanieLineText->getText()).arg(m_pAdressLineText->getText()).arg(m_pINNLineText->getText()).arg(m_pBankLineText->getText()).arg(m_pSchetLineText->getText()).arg(m_pKPPLineText->getText()).arg(m_pBIKLineText->getText()).arg(m_pKorrSchetLineText->getText()).arg(m_pFamDirLineText->getText()).arg(m_pNameDirLineText->getText()).arg(m_pOtchDirLineText->getText()).arg(m_pFormaLineText->getText());
        QSqlQuery query;
        query.exec(strQuery);
    }
    else
    {
        QSqlQuery query;
        QString strQuery = QString("update ЮЛ set Название='%1' where id='%2'").arg(m_pNazvanieLineText->getText()).arg(m_strIdUL);
        query.exec(strQuery);

        strQuery = QString("update ЮЛ set Адрес='%1' where id='%2'").arg(m_pAdressLineText->getText()).arg(m_strIdUL);
        query.exec(strQuery);

        strQuery = QString("update ЮЛ set ИНН='%1' where id='%2'").arg(m_pINNLineText->getText()).arg(m_strIdUL);
        query.exec(strQuery);

        strQuery = QString("update ЮЛ set Банк='%1' where id='%2'").arg(m_pBankLineText->getText()).arg(m_strIdUL);
        query.exec(strQuery);

        strQuery = QString("update ЮЛ set Счет='%1' where id='%2'").arg(m_pSchetLineText->getText()).arg(m_strIdUL);
        query.exec(strQuery);

        strQuery = QString("update ЮЛ set КПП='%1' where id='%2'").arg(m_pKPPLineText->getText()).arg(m_strIdUL);
        query.exec(strQuery);

        strQuery = QString("update ЮЛ set БИК='%1' where id='%2'").arg(m_pBIKLineText->getText()).arg(m_strIdUL);
        query.exec(strQuery);

        strQuery = QString("update ЮЛ set КоррСчет='%1' where id='%2'").arg(m_pKorrSchetLineText->getText()).arg(m_strIdUL);
        query.exec(strQuery);

        strQuery = QString("update ЮЛ set ФамилияДиректора='%1' where id='%2'").arg(m_pFamDirLineText->getText()).arg(m_strIdUL);
        query.exec(strQuery);

        strQuery = QString("update ЮЛ set ИмяДиректора='%1' where id='%2'").arg(m_pNameDirLineText->getText()).arg(m_strIdUL);
        query.exec(strQuery);

        strQuery = QString("update ЮЛ set ОтчествоДиректора='%1' where id='%2'").arg(m_pOtchDirLineText->getText()).arg(m_strIdUL);
        query.exec(strQuery);

        strQuery = QString("update ЮЛ set Форма='%1' where id='%2'").arg(m_pFormaLineText->getText()).arg(m_strIdUL);
        query.exec(strQuery);
    }

    return m_strIdUL;
}

void QULEditDlg::OnApplyCardPressed()
{
    accept();
}
