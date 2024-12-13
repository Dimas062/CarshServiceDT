#include "qemploeeswidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QPixmap>
#include <QSqlQuery>
#include <QListWidgetItem>
#include <QDebug>



QEmploeesWidget::QEmploeesWidget(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout * pVMainLayout = new QVBoxLayout;


    QHBoxLayout * pHEmploeeLayout = new QHBoxLayout;

    m_pEmploeeListWidget = new QListWidget();

    pHEmploeeLayout->addWidget(m_pEmploeeListWidget);

    m_pEmplTabWidget = new QTabWidget;

    m_pEmplTasksWidget = new QEmplTasksWidget;

    m_pEmplTabWidget->addTab(m_pEmplTasksWidget , "Задачи");

    m_pEmplWorkDayWidget = new QEmplWorkDayWidget;

    //m_pEmplTabWidget->addTab(m_pEmplWorkDayWidget , "Смены");

    m_pEmplCardWidget = new QEmplCardWidget;

    m_pEmplTabWidget->addTab(m_pEmplCardWidget , "Карточка");

    pHEmploeeLayout->addWidget(m_pEmplTabWidget);

    pVMainLayout->addLayout(pHEmploeeLayout);


    /*Заполним пользователей*/
    QString strEmplQuery("select id , Фамилия, Имя, Отчество from Пользователи where Удалено<>true and Роль='80066f83-c025-410b-b439-f3e9b2299461'");
    QSqlQuery EmplQuery;
    EmplQuery.exec(strEmplQuery);
    while(EmplQuery.next())
    {
        QListWidgetItem * pItem = new QListWidgetItem;
        pItem->setText(QString("%1 %2 %3").arg(EmplQuery.value(1).toString()).arg(EmplQuery.value(2).toString()).arg(EmplQuery.value(3).toString()));
        pItem->setData(Qt::UserRole , EmplQuery.value(0));
        m_pEmploeeListWidget->addItem(pItem);
    }

    connect(m_pEmploeeListWidget , SIGNAL(itemClicked(QListWidgetItem*)) , this , SLOT(EmplClicked(QListWidgetItem*)));


    this->setLayout(pVMainLayout);
}

void QEmploeesWidget::EmplClicked(QListWidgetItem* item)
{
    QString strEmplUuid = item->data(Qt::UserRole).toString();
    m_pEmplTasksWidget->SetActivEmpl(strEmplUuid);
    m_pEmplWorkDayWidget->SetActivEmpl(strEmplUuid);
    m_pEmplCardWidget->SetActivEmpl(strEmplUuid);
}
