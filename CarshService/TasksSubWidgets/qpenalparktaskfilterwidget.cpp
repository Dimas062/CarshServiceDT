#include "qpenalparktaskfilterwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QString>
#include <QLabel>
#include <QSqlQuery>
#include <QUuid>

QPenalParkTaskFilterWidget::QPenalParkTaskFilterWidget(QWidget *parent)
    : QWidget{parent}
{
    QHBoxLayout * pMainHLayout = new QHBoxLayout;

    /*Отделы Gibdd*/
    QLabel * pGibddLabel = new QLabel("Отдел ГАИ:");
    pMainHLayout->addWidget(pGibddLabel);
    m_pGibddCombo = new QComboBox;
    pMainHLayout->addWidget(m_pGibddCombo);

    m_pGibddCombo->addItem("Все" , QVariant(QUuid()));

    QString strGibddQuery = "select id, Название from \"Отделы ГИБДД\"";
    QSqlQuery query;
    query.exec(strGibddQuery);
    while(query.next())
    {
        m_pGibddCombo->addItem(query.value(1).toString() , query.value(0));
    }

    pMainHLayout->addStretch();

    QLabel * pParkingLabel = new QLabel("Штрафстоянка:");
    pMainHLayout->addWidget(pParkingLabel);
    m_pParkingCombo = new QComboBox;
    pMainHLayout->addWidget(m_pParkingCombo);

    m_pParkingCombo->addItem("Все" , QVariant(QUuid()));

    QString strParkingQuery = "select id, Название from Штрафстоянки";
    query.exec(strParkingQuery);
    while(query.next())
    {
        m_pParkingCombo->addItem(query.value(1).toString() , query.value(0));
    }

    pMainHLayout->addStretch();

    QLabel * pReasonLabel = new QLabel("Причина задержания:");
    pMainHLayout->addWidget(pReasonLabel);
    m_pReasonCombo = new QComboBox;
    pMainHLayout->addWidget(m_pReasonCombo);

    m_pReasonCombo->addItem("Все" , QVariant(QUuid()));

    QString strReasonQuery = "select id, Название from \"Причины задержания\"";
    query.exec(strReasonQuery);
    while(query.next())
    {
        m_pReasonCombo->addItem(query.value(1).toString() , query.value(0));
    }


    this->setLayout(pMainHLayout);
}
