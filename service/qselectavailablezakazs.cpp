#include "qselectavailablezakazs.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSqlQuery>
#include <QCheckBox>

QSelectAvailableZakazs::QSelectAvailableZakazs(QString strEmplPartnerUUid)
{
    m_strEmplPartnerUUid = strEmplPartnerUUid;

    QVBoxLayout * pVMainLayout = new QVBoxLayout;

    m_pZakazsTable  = new QTableWidget;
    m_pZakazsTable->setColumnCount(2);
    m_pZakazsTable->setColumnWidth(0, 220);
    m_pZakazsTable->setColumnWidth(1, 20);

    QStringList headers;
    headers<<"Заказчик"<<" ";
    m_pZakazsTable->setHorizontalHeaderLabels(headers);

    pVMainLayout->addWidget(m_pZakazsTable);

    QHBoxLayout * pHExitButtonsLayout = new QHBoxLayout;

    QPushButton * pApplyButton = new QPushButton("Применить");
    connect(pApplyButton , SIGNAL(clicked()) , this , SLOT(OnAcceptPressed()));
    pHExitButtonsLayout->addWidget(pApplyButton);
    pApplyButton->setDefault(true);

    QPushButton * pRejectButton = new QPushButton("Отменить");
    connect(pRejectButton , SIGNAL(clicked()) , this , SLOT(reject()));
    pHExitButtonsLayout->addWidget(pRejectButton);

    pVMainLayout->addLayout(pHExitButtonsLayout);

    LoadZakazs();

    this->setLayout(pVMainLayout);
}

void QSelectAvailableZakazs::LoadZakazs()
{
    m_pZakazsTable->clear();
    QStringList headers;
    headers<<"Заказчик"<<" ";
    m_pZakazsTable->setHorizontalHeaderLabels(headers);

    QString strQuery =  QString("SELECT Заказчики.id , Заказчики.Название , EXISTS(select id ИсполнителиЗаказчики from ИсполнителиЗаказчики where ИсполнительПартнер='%1' and Заказчик = Заказчики.id) from Заказчики").arg(m_strEmplPartnerUUid);
    QSqlQuery query;
    query.exec(strQuery);

    m_pZakazsTable->setRowCount(query.size());

    int iRowCounter = 0;

    while(query.next())
    {
        /*Название заказчика*/
        QTableWidgetItem * pItem = new QTableWidgetItem(query.value(1).toString());
        pItem->setData(Qt::UserRole , query.value(0));
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pZakazsTable->setItem(iRowCounter , 0,  pItem);

        /*Чек-бокс*/
        QCheckBox * pCheckBox = new QCheckBox();
        if(query.value(2).toBool())
            pCheckBox->setChecked(true);
        m_pZakazsTable->setCellWidget(iRowCounter , 1 , pCheckBox);

        iRowCounter++;
    }
}

void QSelectAvailableZakazs::OnAcceptPressed()
{
    int iRowCount = m_pZakazsTable->rowCount();
    QSqlQuery query;
    for(int iRowCounter = 0 ; iRowCounter<iRowCount; iRowCounter++)
    {
        QString strZakazUuid = m_pZakazsTable->item(iRowCounter , 0)->data(Qt::UserRole).toString();

        QString execStr;

        if(((QCheckBox *)m_pZakazsTable->cellWidget(iRowCounter , 1))->isChecked() == false)
        //Удаляем запись если есть
        {
            execStr = QString("delete from ИсполнителиЗаказчики where ИсполнительПартнер='%1' and Заказчик = '%2'").arg(m_strEmplPartnerUUid).arg(strZakazUuid);
        }
        //Добавляем запись, если ее нет
        else
        {
            execStr = QString("INSERT INTO ИсполнителиЗаказчики (id , ИсполнительПартнер, Заказчик) select uuid_generate_v4() , '%1' , '%2' WHERE NOT EXISTS (SELECT 1 FROM ИсполнителиЗаказчики where ИсполнительПартнер = '%1' and Заказчик = '%2');").arg(m_strEmplPartnerUUid).arg(strZakazUuid);
        }

        query.exec(execStr);
    }

    accept();
}
