#include "qdiceditdlg.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSqlQuery>
#include "qdicitemeditdlg.h"
#include "qdicnewrecdlg.h"
#include <QUuid>

QDicEditDlg::QDicEditDlg(QString strTable ,QStringList fields , bool bIsRemovedField , bool bIsCarshsPrices, bool bIsAddedField)
{
    m_strTable = strTable;
    m_strlstFields = fields;
    m_bIsRemoved = bIsRemovedField;
    m_iCarshsCount = 0;
    m_bIsCarshsPrices = bIsCarshsPrices;

    //Если справочник с ценами для заказчиков, то количество столбцов увеличиваем на количество заказчиков, для этого получаем их количество
    //и запомним самих заказчиков
    if(bIsCarshsPrices)
    {
        QString strQuery = QString("select id , Название from Заказчики where Удалено<>true");
        QSqlQuery query;
        query.exec(strQuery);
        while(query.next())
        {
            SCarshs Carsh;
            Carsh.strUuid = query.value(0).toString();
            Carsh.strName = query.value(1).toString();
            m_vCarshs.push_back(Carsh);
            m_iCarshsCount++;
        }
    }

    QVBoxLayout * pVMainLayout = new QVBoxLayout(this);
    this->setLayout(pVMainLayout);

    QLabel *pTopLabel = new QLabel(QString("<b>%1</b>").arg(m_strTable));
    pVMainLayout->addWidget(pTopLabel);


    m_pTableWidget = new QTableWidget();
    m_pTableWidget->setColumnCount(fields.size() + m_iCarshsCount);
    connect(m_pTableWidget , SIGNAL(itemDoubleClicked(QTableWidgetItem*)) , this , SLOT(OnItemDblClk(QTableWidgetItem*)));

    pVMainLayout->addWidget(m_pTableWidget);

    QHBoxLayout * pButtonsLayout = new QHBoxLayout;

    QPushButton * pAddButton = new QPushButton("Добавить");
    pButtonsLayout->addWidget(pAddButton);
    connect(pAddButton , SIGNAL(clicked()) , this , SLOT(OnAddClicked()));
    if(!bIsAddedField) pAddButton->setEnabled(false);

    QPushButton * pDelButton = new QPushButton("Удалить");
    pButtonsLayout->addWidget(pDelButton);
    connect(pDelButton , SIGNAL(clicked()) , this , SLOT(OnDelClicked()));
    if(!m_bIsRemoved) pDelButton->setEnabled(false);

    pVMainLayout->addLayout(pButtonsLayout);

    UpdateData();

    setMinimumWidth(600);
}

void QDicEditDlg::UpdateData()
{
    m_pTableWidget->clear();
    m_pTableWidget->setRowCount(0);

    //добавим названия заказчиков
    QList<QString> headers = m_strlstFields;
    if(m_bIsCarshsPrices)
    {
        foreach(SCarshs Carsh , m_vCarshs)
        {
            headers<<Carsh.strName;
        }
    }

    m_pTableWidget->setHorizontalHeaderLabels(headers);
    QString strQuery = QString("select id");
    foreach(QString field , m_strlstFields)
    {
        strQuery.append(", \"");
        strQuery.append(field);
        strQuery.append("\"");
    }

    strQuery.append(" from \"");
    strQuery.append(m_strTable);
    strQuery.append("\"");

    if(m_bIsRemoved)
        strQuery.append(" where Удалено<>true");


    QSqlQuery query;
    query.exec(strQuery);

    int iRowCount = query.size();

    m_pTableWidget->setRowCount(iRowCount);

    int iColCount = m_strlstFields.size();

    int iRowCounter = 0;

    while(query.next())
    {
        for(int iColCounter = 0 ; iColCounter<(iColCount); iColCounter++)
        {
            QTableWidgetItem * pItem = new QTableWidgetItem(query.value(iColCounter+1).toString());
            pItem->setData(Qt::UserRole , query.value(0));
            pItem->setData(Qt::UserRole +1 , m_strlstFields.at(iColCounter));
            pItem->setData(Qt::UserRole +2 , ECellType::Standart);
            pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            m_pTableWidget->setItem(iRowCounter , iColCounter,  pItem);
        }
        if(m_bIsCarshsPrices)
        {
            int iCarshCounter = 0;
            foreach(SCarshs Carsh , m_vCarshs)
            {
                //Ищем эту цену для этого заказчика
                QSqlQuery PriceQuery;
                QString strPriceQuery = QString("select Цена, id from ЦеныЗаказчиков where Заказчик='%1' and ТипЗадачи='%2'").arg(Carsh.strUuid).arg(query.value(0).toString());
                PriceQuery.exec(strPriceQuery);
                qint32 iPrice = -1;
                QString strCarshPriceId;
                while(PriceQuery.next())
                {
                    iPrice = PriceQuery.value(0).toInt();
                    strCarshPriceId = PriceQuery.value(1).toString();
                }
                if(iPrice == -1)//Если цена для этой задачи для этого заказчика не задана, то присвоим цену по умолчанию (цену исполнителя)
                {
                    iPrice = query.value(2).toInt();//В справочниках цен(типов задач) цена третьим столбцом(id,название, цена)
                    strCarshPriceId = QUuid::createUuid().toString();

                    /*Добавим в базу новую запись для этого заказчика с ценой этой задачи*/
                    strPriceQuery = QString("insert into ЦеныЗаказчиков (id , Заказчик  , ТипЗадачи , Цена) values  ('%1' , '%2' , '%3' , '%4')").arg(strCarshPriceId).arg(Carsh.strUuid).arg(query.value(0).toString()).arg(query.value(2).toInt());
                    PriceQuery.exec(strPriceQuery);
                }
                //Записываем в ячейку цену для этой задачи (внешний цикл) для этого заказчика (Carsh)
                QTableWidgetItem * pItem = new QTableWidgetItem(QString("%1").arg(iPrice));
                pItem->setData(Qt::UserRole , strCarshPriceId);
                pItem->setData(Qt::UserRole +1 , QVariant("Цена"));
                pItem->setData(Qt::UserRole +2 , ECellType::CarshPrice);
                pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                m_pTableWidget->setItem(iRowCounter , iColCount + iCarshCounter,  pItem);
                iCarshCounter++;
            }
        }
        iRowCounter++;
    }
    m_pTableWidget->resizeColumnsToContents();
}

void QDicEditDlg::OnItemDblClk(QTableWidgetItem * item)
{
    QString strTableName = m_strTable;
    if(item->data(Qt::UserRole +2) == ECellType::CarshPrice)
        strTableName = "ЦеныЗаказчиков";
    QDicItemEditDlg dlg(strTableName , item->data(Qt::UserRole).toString() , item->data(Qt::UserRole +1).toString());
    if(dlg.exec()==QDialog::Accepted)
        UpdateData();
}

void QDicEditDlg::OnAddClicked()
{
    QDicNewRecDlg dlg(m_strTable , m_strlstFields);
    if(dlg.exec()==QDialog::Accepted)
        UpdateData();
}

void QDicEditDlg::OnDelClicked()
{

}
