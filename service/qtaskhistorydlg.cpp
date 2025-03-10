#include "qtaskhistorydlg.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSqlQuery>
#include <QLabel>
#include <QVector>

QTaskHistoryDlg::QTaskHistoryDlg(QString strTaskUuid)
{
    m_strTaskUuid = strTaskUuid;

    QVBoxLayout * pVMainLayout = new QVBoxLayout(this);
    this->setLayout(pVMainLayout);

    QLabel * pTopLabel = new QLabel();
    pVMainLayout->addWidget(pTopLabel);
    pTopLabel->setText("<b>История задачи<\b>");
    pTopLabel->setStyleSheet("font-size: 20px;");

    pVMainLayout->addSpacing(5);


    m_pHistoryTable = new QTableWidget;
    m_pHistoryTable->setColumnCount(3);
    QStringList headers;
    headers << "№" << "Время" << "Событие";
    m_pHistoryTable->setHorizontalHeaderLabels(headers);


    pVMainLayout->addWidget(m_pHistoryTable);

    UpdateHistory();
}


void QTaskHistoryDlg::UpdateHistory()
{
    struct SSmena
    {
        QString strIspolnFrom;
        QString strIspolnTo;
        qint64 iDateTime;
    };

    QVector<SSmena> vSmenas;

    QString strExec = QString("select ИсполнительОт, ИсполнительК, ДатаВремя from ПередачиЗадач where Задача = '%1' order by ДатаВремя").arg(m_strTaskUuid);
    QSqlQuery query;
    query.exec(strExec);
    while(query.next())
    {
        SSmena Smena;
        Smena.iDateTime = query.value(2).toInt();
        Smena.strIspolnFrom = query.value(0).toString();
        Smena.strIspolnTo = query.value(1).toString();
        vSmenas.push_back(Smena);
    }

    //Первая строка - создание задачи если не было смен. Если смены были, то исполнителя создания берем из первой смены От
    if(vSmenas.empty())
    {

    }
    else
    {

    }


}
