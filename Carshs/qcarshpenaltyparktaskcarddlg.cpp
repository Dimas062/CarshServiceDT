#include "qcarshpenaltyparktaskcarddlg.h"
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

QCarshPenaltyParkTaskCardDlg::QCarshPenaltyParkTaskCardDlg(QString strTaskUuid)
{
    QVBoxLayout * pVMainLayout = new QVBoxLayout(this);
    this->setLayout(pVMainLayout);

    QLabel * pTopLabel = new QLabel();
    pVMainLayout->addWidget(pTopLabel);
    pTopLabel->setText("<h1>Штрафстоянка<\h1>");

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


        QSqlQuery extendQuery;

        QString strExtenQuery = QString("select \"Платежи сотрудников\".Сумма from \"Расширение задачи ШС\", \"Платежи сотрудников\" where  \"Расширение задачи ШС\".\"Оплата парковки\"=\"Платежи сотрудников\".id and \"Расширение задачи ШС\".id='%1'").arg(query.value(2).toString());
        extendQuery.exec(strExtenQuery);
        if(extendQuery.next())
        {
            QLabel * pSummLabel = new QLabel();
            pVMainLayout->addWidget(pSummLabel);
            pSummLabel->setText(QString("<h3>Сумма: %1 р.<\h3>").arg(extendQuery.value(0).toString()));
        }


        strExtenQuery = QString("select Госномер from \"Расширение задачи ШС\" where \"Расширение задачи ШС\".id='%1'").arg(query.value(2).toString());
        extendQuery.exec(strExtenQuery);
        if(extendQuery.next())
        {
            QLabel * pPlateLabel = new QLabel();
            pVMainLayout->addWidget(pPlateLabel);
            pPlateLabel->setText(QString("<h3>Госномер: %1<\h3>").arg(extendQuery.value(0).toString()));
        }


        strExtenQuery = QString("select  \"Отделы ГИБДД\".Название, \"Отделы ГИБДД\".Адрес from \"Расширение задачи ШС\", \"Отделы ГИБДД\" where \"Расширение задачи ШС\".\"Отдел ГАИ\"=\"Отделы ГИБДД\".id and \"Расширение задачи ШС\".id='%1'").arg(query.value(2).toString());
        extendQuery.exec(strExtenQuery);
        if(extendQuery.next())
        {
            QLabel * pGAILabel = new QLabel();
            pVMainLayout->addWidget(pGAILabel);
            pGAILabel->setText(QString("<h3>Отдел ГАИ: %1 ( %2 )<\h3>").arg(extendQuery.value(0).toString()).arg(extendQuery.value(1).toString()));
        }


        strExtenQuery = QString("select \"Причины задержания\".Название  from \"Расширение задачи ШС\", \"Причины задержания\" where  \"Причины задержания\".id=\"Расширение задачи ШС\".\"Причина задержания\" and \"Расширение задачи ШС\".id='%1'").arg(query.value(2).toString());
        extendQuery.exec(strExtenQuery);
        if(extendQuery.next())
        {
            QLabel * pReasonLabel = new QLabel();
            pVMainLayout->addWidget(pReasonLabel);
            pReasonLabel->setText(QString("<h3>Причина задержания: %1<\h3>").arg(extendQuery.value(0).toString()));
        }

        strExtenQuery = QString("select Штрафстоянки.Название , Штрафстоянки.Адрес from \"Расширение задачи ШС\", Штрафстоянки where Штрафстоянки.id=\"Расширение задачи ШС\".\"Штрафстоянка\" and \"Расширение задачи ШС\".id='%1'").arg(query.value(2).toString());
        extendQuery.exec(strExtenQuery);
        if(extendQuery.next())
        {
            QLabel * pPenParkingLabel = new QLabel();
            pVMainLayout->addWidget(pPenParkingLabel);
            pPenParkingLabel->setText(QString("<h3>Штрафстоянка: %1 ( %2 )<\h3>").arg(extendQuery.value(0).toString()).arg(extendQuery.value(1).toString()));
        }

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

    pVMainLayout->addStretch();

}
