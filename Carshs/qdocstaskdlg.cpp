#include "qdocstaskdlg.h"
#include "common.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QString>
#include <QUuid>
#include <QSqlQuery>
#include <QDateTime>
#include <QUuid>

QDocsTaskDlg::QDocsTaskDlg(QString strTaskUuid)
{
    QVBoxLayout * pVMainLayout = new QVBoxLayout(this);
    this->setLayout(pVMainLayout);

    QLabel * pTopLabel = new QLabel();
    pVMainLayout->addWidget(pTopLabel);
    pTopLabel->setText("<b>Документы<\b>");
    pTopLabel->setStyleSheet("font-size: 20px;");

    pVMainLayout->addSpacing(5);

    QString strQuery = QString("select \"Дата Время\" , Комментарий, Расширение from Задачи where id='%1'").arg(strTaskUuid);

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

        QString strExtenQuery = QString("select \"Расширение задачи Документы\".Документ , \"Расширение задачи Документы\".Количество , \"Документы задачи документы\".Документ from \"Расширение задачи Документы\" , \"Документы задачи документы\" where \"Расширение задачи Документы\".id='%1' and \"Документы задачи документы\".id = \"Расширение задачи Документы\".Документ").arg(query.value(2).toString());
        extendQuery.exec(strExtenQuery);

        if(extendQuery.next())
        {
            QLabel * pDocLabel = new QLabel();
            pVMainLayout->addWidget(pDocLabel);
            pDocLabel->setText(QString("<b>Документ: %1.<\b>").arg(extendQuery.value(2).toString()));
            pDocLabel->setStyleSheet("font-size: 16px;");

            QLabel * pCountLabel = new QLabel();
            pVMainLayout->addWidget(pCountLabel);
            pCountLabel->setText(QString("<b>Количество: %1 шт.<\b>").arg(extendQuery.value(1).toString()));
            pCountLabel->setStyleSheet("font-size: 16px;");

            /*СТС - показываем оплату*/
            if(QUuid::fromString(extendQuery.value(0).toString()) == QUuid::fromString("d7e6ca81-d6fe-405e-8a39-961ceb9bd1f6"))
            {
                strExtenQuery = QString("select \"Платежи сотрудников\".Сумма , \"Платежи сотрудников\".id from \"Расширение задачи Документы\", \"Платежи сотрудников\" where  \"Расширение задачи Документы\".Оплата=\"Платежи сотрудников\".id and \"Расширение задачи Документы\".id='%1'").arg(query.value(2).toString());
                extendQuery.exec(strExtenQuery);
                if(extendQuery.next())
                {
                    QLabel * pSummLabel = new QLabel();
                    pVMainLayout->addWidget(pSummLabel);
                    pSummLabel->setText(QString("<b>Сумма: %1 р.<\b>").arg(extendQuery.value(0).toString()));
                    pSummLabel->setStyleSheet("font-size: 16px;");

                    /*Фото чека*/
                    m_pPicturesWidget = new QPicturesWidget(nullptr , true , true);
                    m_pPicturesWidget->setMinimumHeight(400);
                    pVMainLayout->addWidget(m_pPicturesWidget);



                    QString strPicsExec = QString("select Изображение from Документы where id in (select Документ from \"Платеж сотрудника - Документы\" where Платеж = '%1')").arg(extendQuery.value(1).toString());
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
        }

    }

    pVMainLayout->addStretch();

}

