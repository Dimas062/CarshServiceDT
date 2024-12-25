#include "qdiceditdlg.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSqlQuery>
#include "qdicitemeditdlg.h"
#include "qdicnewrecdlg.h"

QDicEditDlg::QDicEditDlg(QString strTable ,QStringList fields , bool bIsRemovedField)
{
    m_strTable = strTable;
    m_strlstFields = fields;
    m_bIsRemoved = bIsRemovedField;

    QVBoxLayout * pVMainLayout = new QVBoxLayout(this);
    this->setLayout(pVMainLayout);

    QLabel *pTopLabel = new QLabel(QString("<b>%1</b>").arg(m_strTable));
    pVMainLayout->addWidget(pTopLabel);


    m_pTableWidget = new QTableWidget();
    m_pTableWidget->setColumnCount(fields.size());
    connect(m_pTableWidget , SIGNAL(itemDoubleClicked(QTableWidgetItem*)) , this , SLOT(OnItemDblClk(QTableWidgetItem*)));

    pVMainLayout->addWidget(m_pTableWidget);

    QHBoxLayout * pButtonsLayout = new QHBoxLayout;

    QPushButton * pAddButton = new QPushButton("Добавить");
    pButtonsLayout->addWidget(pAddButton);
    connect(pAddButton , SIGNAL(clicked()) , this , SLOT(OnAddClicked()));

    QPushButton * pDelButton = new QPushButton("Удалить");
    pButtonsLayout->addWidget(pDelButton);
    connect(pDelButton , SIGNAL(clicked()) , this , SLOT(OnDelClicked()));
    if(!m_bIsRemoved) pDelButton->setEnabled(false);

    pVMainLayout->addLayout(pButtonsLayout);

    UpdateData();
}

void QDicEditDlg::UpdateData()
{
    m_pTableWidget->clear();
    m_pTableWidget->setRowCount(0);


    m_pTableWidget->setHorizontalHeaderLabels(m_strlstFields);
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
    qDebug()<<strQuery;


    int iRowCount = query.size();

    m_pTableWidget->setRowCount(iRowCount);

    int iColCount = m_strlstFields.size();

    int iRowCounter = 0;

    while(query.next())
    {
        for(int iColCounter = 0; iColCounter<iColCount ; iColCounter++)
        {
            QTableWidgetItem * pItem = new QTableWidgetItem(query.value(iColCounter+1).toString());
            pItem->setData(Qt::UserRole , query.value(0));
            pItem->setData(Qt::UserRole +1 , m_strlstFields.at(iColCounter));
            pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            m_pTableWidget->setItem(iRowCounter , iColCounter,  pItem);
        }
        iRowCounter++;
    }
}

void QDicEditDlg::OnItemDblClk(QTableWidgetItem * item)
{
    QDicItemEditDlg dlg(m_strTable , item->data(Qt::UserRole).toString() , item->data(Qt::UserRole +1).toString());
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
