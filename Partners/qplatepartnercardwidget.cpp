#include "qplatepartnercardwidget.h"
#include <QVBoxLayout>
#include <QString>
#include <QSqlQuery>
#include <QListWidgetItem>

QPlatePartnerCardWidget::QPlatePartnerCardWidget(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout * pVMainLayout = new QVBoxLayout;


    QLabel * pTopLabel = new QLabel("<b>Партнер \"Номера\"</b>");
    pTopLabel->setStyleSheet("font-size: 20px;\
                               color: black;");
    pTopLabel->setAlignment(Qt::AlignHCenter);
    pVMainLayout->addWidget(pTopLabel);

    m_pNameLabel = new QLabel("Название: ");
    m_pNameLabel->setStyleSheet("font-size: 16px;\
                               color: black;");
    pVMainLayout->addWidget(m_pNameLabel);

    m_pPostavshikLabel = new QLabel("Заказчик: ");
    m_pPostavshikLabel->setStyleSheet("font-size: 16px;\
                               color: black;");
    pVMainLayout->addWidget(m_pPostavshikLabel);

    m_pAdressLabel = new QLabel("Адрес: ");
    m_pAdressLabel->setStyleSheet("font-size: 16px;\
                               color: black;");
    pVMainLayout->addWidget(m_pAdressLabel);

    m_pINNLabel = new QLabel("ИНН: ");
    m_pINNLabel->setStyleSheet("font-size: 16px;\
                               color: black;");
    pVMainLayout->addWidget(m_pINNLabel);

    m_pBankLabel = new QLabel("Банк: ");
    m_pBankLabel->setStyleSheet("font-size: 16px;\
                               color: black;");
    pVMainLayout->addWidget(m_pBankLabel);

    m_pScetLabel = new QLabel("счет: ");
    m_pScetLabel->setStyleSheet("font-size: 16px;\
                               color: black;");
    pVMainLayout->addWidget(m_pScetLabel);

    QLabel * pPointsLabel = new QLabel("<b>Точки</b>");
    pPointsLabel->setStyleSheet("font-size: 16px;\
                               color: black;");
    pPointsLabel->setAlignment(Qt::AlignHCenter);
    pVMainLayout->addWidget(pPointsLabel);

    m_pPointsListWidget = new QListWidget;
    pVMainLayout->addWidget(m_pPointsListWidget);

    this->setLayout(pVMainLayout);
}


void QPlatePartnerCardWidget::SetActivPartner(QString strUuid)
{
    QString strPartnerExec = QString("select ЮЛ.Название, ЮЛ.Адрес, ЮЛ.ИНН, ЮЛ.Банк, ЮЛ.Счет, Поставщики.Название from ЮЛ, Поставщики, Партнеры where Партнеры.id='%1' and Партнеры.Поставщик=Поставщики.id and Партнеры.ЮЛ=ЮЛ.id").arg(strUuid);
    QSqlQuery query;
    query.exec(strPartnerExec);

    if(query.next())
    {
        m_pNameLabel->setText(QString("<b>Название: %1<\b>").arg(query.value(0).toString()));
        m_pPostavshikLabel->setText(QString("<b>Заказчик: %1<\b>").arg(query.value(5).toString()));
        m_pAdressLabel->setText(QString("<b>Адрес: %1<\b>").arg(query.value(1).toString()));
        m_pINNLabel->setText(QString("<b>ИНН: %1<\b>").arg(query.value(2).toString()));
        m_pBankLabel->setText(QString("<b>Банк: %1<\b>").arg(query.value(3).toString()));
        m_pScetLabel->setText(QString("<b>Счет: %1<\b>").arg(query.value(4).toString()));
    }

    //Точки
    QString strPointsExec = QString("select Название, Адрес from \"Точки Партнеров\" where Партнер='%1' and Удалено<>'true'").arg(strUuid);

    QSqlQuery PointsQuery;
    PointsQuery.exec(strPointsExec);
    while(PointsQuery.next())
    {
        QListWidgetItem * pItem = new QListWidgetItem;
        pItem->setText(QString("%1 (%2)").arg(PointsQuery.value(0).toString()).arg(PointsQuery.value(1).toString()));
        m_pPointsListWidget->addItem(pItem);
    }
}
