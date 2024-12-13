#include "qsmenataskdlg.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QString>
#include <QUuid>
#include <QSqlQuery>
#include <QDateTime>


QSmenaTaskDlg::QSmenaTaskDlg(QString strTaskUuid)
{
    QVBoxLayout * pVMainLayout = new QVBoxLayout(this);
    this->setLayout(pVMainLayout);

    QLabel * pTopLabel = new QLabel();
    pVMainLayout->addWidget(pTopLabel);
    pTopLabel->setText("<b>Смена<\b>");
    pTopLabel->setStyleSheet("font-size: 20px;");
    pVMainLayout->addSpacing(5);

    QString strQuery = QString("select \"Дата Время\" ,  Комментарий, Расширение from Задачи where id='%1'").arg(strTaskUuid);

    QSqlQuery query;
    query.exec(strQuery);
    while(query.next())
    {
        QLabel * pDateTimeLabel = new QLabel();
        pVMainLayout->addWidget(pDateTimeLabel);
        pDateTimeLabel->setText("<b>" + QDateTime::fromSecsSinceEpoch(query.value(0).toInt()).toString("dd.MM.yyyy hh:mm") + "<\b>");
        pDateTimeLabel->setStyleSheet("font-size: 16px;");
        if(query.value(1).toString().length()>1)
        {
            QLabel * pCommentLabel = new QLabel();
            pVMainLayout->addWidget(pCommentLabel);
            pCommentLabel->setText(QString("<b>Комментарий: %1<\b>").arg(query.value(1).toString()));
            pCommentLabel->setStyleSheet("font-size: 16px;");
        }

        QString strExtenQuery = QString("select \"Количество часов в смене\".Количество from \"Расширение задачи Смена\" , \"Количество часов в смене\"  where \"Расширение задачи Смена\".id='%1' and \"Расширение задачи Смена\".\"Количество часов\" = \"Количество часов в смене\".id").arg(query.value(2).toString());

        QSqlQuery extendQuery;

        extendQuery.exec(strExtenQuery);
        while(extendQuery.next())
        {

            QLabel * pPlateLabel = new QLabel();
            pVMainLayout->addWidget(pPlateLabel);
            pPlateLabel->setText(QString("<b>Часов в смене: %1 ч.<\b>").arg(extendQuery.value(0).toString()));
            pPlateLabel->setStyleSheet("font-size: 16px;");
        }
    }

    pVMainLayout->addStretch();
}
