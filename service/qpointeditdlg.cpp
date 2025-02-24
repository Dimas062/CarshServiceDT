#include "qpointeditdlg.h"
#include <QVBoxLayout>
#include <QString>
#include <QSqlQuery>
#include <QPushButton>
#include <QUuid>

QPointEditDlg::QPointEditDlg()
{
    QVBoxLayout * pVMainLayout = new QVBoxLayout;

    m_pPointNameLineText = new QLineText("Название");
    pVMainLayout->addWidget(m_pPointNameLineText);

    m_pPointAdressLineText = new QLineText("Адрес");
    pVMainLayout->addWidget(m_pPointAdressLineText);

    m_pAliasAdressLineText = new QLineText("Псевдоним");
    m_pAliasAdressLineText->SetToolTip("Используется при идентификации точки альтернативным названием,\n например при выгрузке в Google-таблицу заказчика, где названия отличаются.\n Нужно ввести как у заказчика в таблице, что бы система нашла в таблице данную точку");
    pVMainLayout->addWidget(m_pAliasAdressLineText);


    QHBoxLayout *pButtonsLayout = new QHBoxLayout;


    QPushButton * pApplyCardButton = new QPushButton("Применить");
    connect(pApplyCardButton,SIGNAL(pressed()),this,SLOT(OnApplyPressed()));
    pButtonsLayout->addWidget(pApplyCardButton);

    QPushButton * pDeleteCardButton = new QPushButton("Отменить");
    connect(pDeleteCardButton,SIGNAL(pressed()),this,SLOT(reject()));
    pButtonsLayout->addWidget(pDeleteCardButton);

    pVMainLayout->addLayout(pButtonsLayout);

    this->setLayout(pVMainLayout);
}



void QPointEditDlg::OnApplyPressed()
{
    accept();
}

QString QPointEditDlg::SaveUpdateToBD()
{
    QSqlQuery query;
    QString strQuery = QString("update \"Точки Партнеров\" set Название='%1' where id='%2'").arg(m_pPointNameLineText->getText()).arg(m_strIdPoint);
    query.exec(strQuery);

    strQuery = QString("update \"Точки Партнеров\" set Адрес='%1' where id='%2'").arg(m_pPointAdressLineText->getText()).arg(m_strIdPoint);
    query.exec(strQuery);

    strQuery = QString("update \"Точки Партнеров\" set Псевдоним='%1' where id='%2'").arg(m_pAliasAdressLineText->getText()).arg(m_strIdPoint);
    query.exec(strQuery);

    return m_strIdPoint;
}

QString QPointEditDlg::SaveNewToBD(QString strPartnerUuid)
{
    m_strIdPartner = strPartnerUuid;
    m_strIdPoint = QUuid::createUuid().toString();
    QString strQuery = QString("insert into \"Точки Партнеров\" (id, Название, Адрес, Партнер , Псевдоним) values ('%1' , '%2' ,'%3' ,'%4' , '%5')  ").arg(m_strIdPoint).arg(m_pPointNameLineText->getText()).arg(m_pPointAdressLineText->getText()).arg(m_strIdPartner).arg(m_pAliasAdressLineText->getText());
    QSqlQuery query;
    query.exec(strQuery);
    return m_strIdPoint;
}

void QPointEditDlg::LoadFromBD(QString strIdPoint)
{
    m_strIdPoint = strIdPoint;
    QString strQuery = QString("select Название, Адрес, Партнер, Псевдоним from \"Точки Партнеров\" where id='%1' ").arg(m_strIdPoint);
    QSqlQuery query;
    query.exec(strQuery);
    while(query.next())
    {
        m_pPointNameLineText->setText(query.value(0).toString());
        m_pPointAdressLineText->setText(query.value(1).toString());
        m_strIdPartner = query.value(2).toString();
        m_pAliasAdressLineText->setText(query.value(3).toString());
    }
}
