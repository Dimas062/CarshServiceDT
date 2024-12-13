#include "qplatepartnerswidget.h"
#include <QHBoxLayout>
#include <QSqlQuery>


QPlatePartnersWidget::QPlatePartnersWidget(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout * pVMainLayout = new QVBoxLayout;
    this->setLayout(pVMainLayout);

    QHBoxLayout * pHMainLayout = new QHBoxLayout;

    QHBoxLayout * pHPlatePartnerLayout = new QHBoxLayout;

    m_pPlatePartnerListWidget = new QListWidget();

    pHPlatePartnerLayout->addWidget(m_pPlatePartnerListWidget);

    m_pPlatePartnerTabWidget = new QTabWidget;

    m_pPlatePartnerTasksWidget = new QPlatePartnerTaskWidget;

    m_pPlatePartnerTabWidget->addTab(m_pPlatePartnerTasksWidget , "Задачи");

    m_pPlatePartnerCardWidget = new QPlatePartnerCardWidget;

    m_pPlatePartnerTabWidget->addTab(m_pPlatePartnerCardWidget , "Карточка");

    pHPlatePartnerLayout->addWidget(m_pPlatePartnerTabWidget);

    pVMainLayout->addLayout(pHPlatePartnerLayout);


    /*Заполним пользователей*/
    QString strPlatePartnerQuery("select Партнеры.id , ЮЛ.Название from Партнеры, ЮЛ where Партнеры.ЮЛ = ЮЛ.id and Партнеры.Удалено<>true and Партнеры.Тип='9c671ee9-2749-4717-a343-b18825855c29'");
    QSqlQuery PlatePartnerQuery;
    PlatePartnerQuery.exec(strPlatePartnerQuery);
    while(PlatePartnerQuery.next())
    {
        QListWidgetItem * pItem = new QListWidgetItem;
        pItem->setText(QString("%1").arg(PlatePartnerQuery.value(1).toString()));
        pItem->setData(Qt::UserRole , PlatePartnerQuery.value(0));
        m_pPlatePartnerListWidget->addItem(pItem);
    }

    connect(m_pPlatePartnerListWidget , SIGNAL(itemClicked(QListWidgetItem*)) , this , SLOT(PlatePartnerClicked(QListWidgetItem*)));

    this->setLayout(pHMainLayout);
}


void QPlatePartnersWidget::PlatePartnerClicked(QListWidgetItem * item)
{
    qDebug()<<"QPlatePartnersWidget::PlatePartnerClicked(QListWidgetItem * item)";
    QString strPartnerUuid = item->data(Qt::UserRole).toString();
    m_pPlatePartnerCardWidget->SetActivPartner(strPartnerUuid);
    m_pPlatePartnerTasksWidget->SetActivPartner(strPartnerUuid);

}
