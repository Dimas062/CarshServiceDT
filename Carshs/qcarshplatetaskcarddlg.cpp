#include "qcarshplatetaskcarddlg.h"
#include "common.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QString>
#include <QUuid>
#include <QSqlQuery>
#include <QDebug>
#include <QDateTime>
#include <QDebug>

QCarshPlateTaskCardDlg::QCarshPlateTaskCardDlg(QString strTaskUuid)
{
    QVBoxLayout * pVMainLayout = new QVBoxLayout(this);
    this->setLayout(pVMainLayout);

    QLabel * pTopLabel = new QLabel();
    pVMainLayout->addWidget(pTopLabel);
    pTopLabel->setText("<h1>Номера<\h1>");

    pVMainLayout->addSpacing(5);

    QString strQuery = QString("select \"Дата Время\" ,  Комментарий, Расширение from Задачи where id='%1'").arg(strTaskUuid);

    QSqlQuery query;
    query.exec(strQuery);
    while(query.next())
    {
        QLabel * pDateTimeLabel = new QLabel();
        pVMainLayout->addWidget(pDateTimeLabel);
        pDateTimeLabel->setText("<h3>" + QDateTime::fromSecsSinceEpoch(query.value(0).toInt()).toString("dd.MM.yyyy hh:mm") + "<\h3>");

        if(query.value(1).toString().length()>1)
        {
            QLabel * pCommentLabel = new QLabel();
            pVMainLayout->addWidget(pCommentLabel);
            pCommentLabel->setText(QString("<h3>Комментарий: %1<\h3>").arg(query.value(1).toString()));
        }

        QString strExtenQuery = QString("select \"Количество рамок\" , Госномер from \"Расширение задачи Номера\" where \"Расширение задачи Номера\".id='%1'").arg(query.value(2).toString());
        QSqlQuery extendQuery;

        extendQuery.exec(strExtenQuery);
        while(extendQuery.next())
        {
            QLabel * pSummLabel = new QLabel();
            pVMainLayout->addWidget(pSummLabel);
            pSummLabel->setText(QString("<h3>Количество рамок: %1 шт.<\h3>").arg(extendQuery.value(0).toString()));

            QLabel * pPlateLabel = new QLabel();
            pVMainLayout->addWidget(pPlateLabel);
            pPlateLabel->setText(QString("<h3>Госномер: %1<\h3>").arg(extendQuery.value(1).toString()));

            m_pPicturesWidget = new QPicturesWidget(nullptr , true , true);
            m_pPicturesWidget->setMinimumHeight(400);
            pVMainLayout->addWidget(m_pPicturesWidget);

            QString strPicsExec = QString("select Документы.Изображение from Документы, \"Задача-Документы задач\" where Документы.id=\"Задача-Документы задач\".Документ and \"Задача-Документы задач\".Задача='%1'").arg(strTaskUuid);
            QSqlQuery PicsQuery;
            PicsQuery.exec(strPicsExec);
            while(PicsQuery.next())
            {
                QString tmpStr = PicsQuery.value(0).toString();
                QImage tmpImg = Base64ToImage(tmpStr);
                m_pPicturesWidget->AddImage(tmpImg);
            }
        }
    }

    pVMainLayout->addStretch();
}
