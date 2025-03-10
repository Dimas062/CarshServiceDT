#include "qcarshpenaltyparktaskcarddlg.h"
#include "common.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QString>
#include <QUuid>
#include <QSqlQuery>
#include <QDateTime>

QCarshPenaltyParkTaskCardDlg::QCarshPenaltyParkTaskCardDlg(QString strTaskUuid)
{
    QVBoxLayout * pVMainLayout = new QVBoxLayout(this);
    this->setLayout(pVMainLayout);

    QLabel * pTopLabel = new QLabel();
    pVMainLayout->addWidget(pTopLabel);
    pTopLabel->setText("<b>Штрафстоянка<\b>");
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


        QSqlQuery extendQuery;

        QString strExtenQuery = QString("select \"Платежи сотрудников\".Сумма , \"Платежи сотрудников\".id from \"Расширение задачи ШС\", \"Платежи сотрудников\" where  \"Расширение задачи ШС\".\"Оплата парковки\"=\"Платежи сотрудников\".id and \"Расширение задачи ШС\".id='%1'").arg(query.value(2).toString());
        extendQuery.exec(strExtenQuery);
        if(extendQuery.next())
        {
            QLabel * pSummLabel = new QLabel();
            pVMainLayout->addWidget(pSummLabel);
            pSummLabel->setText(QString("<b>Сумма: %1 р.<\b>").arg(extendQuery.value(0).toString()));
            pSummLabel->setStyleSheet("font-size: 16px;");
        }

        QString strPayId =extendQuery.value(1).toString();


        strExtenQuery = QString("select Госномер from \"Расширение задачи ШС\" where \"Расширение задачи ШС\".id='%1'").arg(query.value(2).toString());
        extendQuery.exec(strExtenQuery);
        if(extendQuery.next())
        {
            QLabel * pPlateLabel = new QLabel();
            pVMainLayout->addWidget(pPlateLabel);
            pPlateLabel->setText(QString("<b>Госномер: %1<\b>").arg(extendQuery.value(0).toString()));
            pPlateLabel->setStyleSheet("font-size: 16px;");
        }


        strExtenQuery = QString("select  \"Отделы ГИБДД\".Название, \"Отделы ГИБДД\".Адрес from \"Расширение задачи ШС\", \"Отделы ГИБДД\" where \"Расширение задачи ШС\".\"Отдел ГАИ\"=\"Отделы ГИБДД\".id and \"Расширение задачи ШС\".id='%1'").arg(query.value(2).toString());
        extendQuery.exec(strExtenQuery);
        if(extendQuery.next())
        {
            QLabel * pGAILabel = new QLabel();
            pVMainLayout->addWidget(pGAILabel);
            pGAILabel->setText(QString("<b>Отдел ГАИ: %1 ( %2 )<\b>").arg(extendQuery.value(0).toString()).arg(extendQuery.value(1).toString()));
            pGAILabel->setStyleSheet("font-size: 16px;");
        }


        strExtenQuery = QString("select \"Причины задержания\".Название  from \"Расширение задачи ШС\", \"Причины задержания\" where  \"Причины задержания\".id=\"Расширение задачи ШС\".\"Причина задержания\" and \"Расширение задачи ШС\".id='%1'").arg(query.value(2).toString());
        extendQuery.exec(strExtenQuery);
        if(extendQuery.next())
        {
            QLabel * pReasonLabel = new QLabel();
            pVMainLayout->addWidget(pReasonLabel);
            pReasonLabel->setText(QString("<b>Причина задержания: %1<\b>").arg(extendQuery.value(0).toString()));
            pReasonLabel->setStyleSheet("font-size: 16px;");
        }

        strExtenQuery = QString("select Штрафстоянки.Название , Штрафстоянки.Адрес from \"Расширение задачи ШС\", Штрафстоянки where Штрафстоянки.id=\"Расширение задачи ШС\".\"Штрафстоянка\" and \"Расширение задачи ШС\".id='%1'").arg(query.value(2).toString());
        extendQuery.exec(strExtenQuery);
        if(extendQuery.next())
        {
            QLabel * pPenParkingLabel = new QLabel();
            pVMainLayout->addWidget(pPenParkingLabel);
            pPenParkingLabel->setText(QString("<b>Штрафстоянка: %1 ( %2 )<\b>").arg(extendQuery.value(0).toString()).arg(extendQuery.value(1).toString()));
            pPenParkingLabel->setStyleSheet("font-size: 16px;");
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

        strPicsExec = QString("select Изображение from Документы where id in (select Документ from \"Платеж сотрудника - Документы\" where Платеж = '%1')").arg(strPayId);

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
