#include "qcarshparkingtaskcarddlg.h"
#include "common.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QString>
#include <QUuid>
#include <QSqlQuery>
#include <QDateTime>

QCarshParkingTaskCardDlg::QCarshParkingTaskCardDlg(QString strTaskUuid)
{
    QVBoxLayout * pVMainLayout = new QVBoxLayout(this);
    this->setLayout(pVMainLayout);

    QLabel * pTopLabel = new QLabel();
    pVMainLayout->addWidget(pTopLabel);
    pTopLabel->setText("<b>Закрытая территория<\b>");
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

        QString strExtenQuery = QString("select \"Платежи сотрудников\".Сумма , Госномер , \"Платежи сотрудников\".id from \"Расширение задачи Парковка\", \"Платежи сотрудников\" where \"Расширение задачи Парковка\".\"Оплата парковки\"=\"Платежи сотрудников\".id and \"Расширение задачи Парковка\".id='%1'").arg(query.value(2).toString());
        QSqlQuery extendQuery;

        extendQuery.exec(strExtenQuery);
        while(extendQuery.next())
        {
            QLabel * pSummLabel = new QLabel();
            pVMainLayout->addWidget(pSummLabel);
            pSummLabel->setText(QString("<b>Сумма: %1 руб.<\b>").arg(extendQuery.value(0).toString()));
            pSummLabel->setStyleSheet("font-size: 16px;");

            QLabel * pPlateLabel = new QLabel();
            pVMainLayout->addWidget(pPlateLabel);
            pPlateLabel->setText(QString("<b>Госномер: %1<\b>").arg(extendQuery.value(1).toString()));
            pPlateLabel->setStyleSheet("font-size: 16px;");

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

            strPicsExec = QString("select Изображение from Документы where id in (select Документ from \"Платеж сотрудника - Документы\" where Платеж = '%1')").arg(extendQuery.value(2).toString());
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
