#include "qselempldlg.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QSqlQuery>

QSelEmplDlg::QSelEmplDlg()
{
    QVBoxLayout * pVMainLayout = new QVBoxLayout;

    m_pEmploeeListWidget = new QListWidget();
    pVMainLayout->addWidget(m_pEmploeeListWidget);

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

    connect(m_pEmploeeListWidget , SIGNAL(itemDoubleClicked(QListWidgetItem*)) , this , SLOT(OnEmplDblClicked(QListWidgetItem*)));

    QPushButton * pSetEmplButton = new QPushButton("Выбрать");
    connect(pSetEmplButton,SIGNAL(pressed()),this,SLOT(OnSetEmplPressed()));
    pVMainLayout->addWidget(pSetEmplButton);

    QPushButton * pCancelButton = new QPushButton("Отменить");
    connect(pCancelButton,SIGNAL(pressed()),this,SLOT(reject()));
    pVMainLayout->addWidget(pCancelButton);


    this->setLayout(pVMainLayout);
}

void QSelEmplDlg::OnSetEmplPressed()
{
    if(m_pEmploeeListWidget->selectedItems().size() > 0)
    {
        QListWidgetItem* item  = m_pEmploeeListWidget->selectedItems()[0];
        if(item)
        {
            m_uuidSelectedEmpl = item->data(Qt::UserRole).toUuid();
            accept();
        }
    }
}

void QSelEmplDlg::OnEmplDblClicked(QListWidgetItem* item)
{
    m_uuidSelectedEmpl = item->data(Qt::UserRole).toUuid();
    accept();
}
