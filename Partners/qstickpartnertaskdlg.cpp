#include "qstickpartnertaskdlg.h"
#include "common.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QString>
#include <QUuid>
#include <QSqlQuery>
#include <QDateTime>


QStickPartnerTaskDlg::QStickPartnerTaskDlg(QString strTaskUuid)
{
    QVBoxLayout * pVMainLayout = new QVBoxLayout(this);
    this->setLayout(pVMainLayout);

    QLabel * pTopLabel = new QLabel();
    pVMainLayout->addWidget(pTopLabel);
    pTopLabel->setText("<b>Оклейка<\b>");
    pTopLabel->setStyleSheet("font-size: 20px;");

    pVMainLayout->addSpacing(5);

    QString strExec = QString("select ДатаВремя, Точка, Номер , \"Время выполнения\" , Комментарий , Партнер from \"Задачи партнера Оклейка\"  where id='%1'").arg(strTaskUuid);
    QSqlQuery query;
    query.exec(strExec);
    while(query.next())
    {
        QLabel * pDateTimeLabel = new QLabel();
        pVMainLayout->addWidget(pDateTimeLabel);
        pDateTimeLabel->setText("<b>" + QDateTime::fromSecsSinceEpoch(query.value(3).toInt()).toString("dd.MM.yyyy hh:mm") + "<\b>");
        pDateTimeLabel->setStyleSheet("font-size: 16px;");

        QString strPointQuery = QString("select Название, Адрес from \"Точки Партнеров\" where id='%1'").arg(query.value(1).toString());
        QSqlQuery pointQuery;
        pointQuery.exec(strPointQuery);
        while(pointQuery.next())
        {
            QLabel * pPointLabel = new QLabel();
            pVMainLayout->addWidget(pPointLabel);
            pPointLabel->setText(QString("<b>Точка: %1 (%2) <\b>").arg(pointQuery.value(0).toString()).arg(pointQuery.value(1).toString()));
            pPointLabel->setStyleSheet("font-size: 16px;");
        }



        QLabel * pPlateLabel = new QLabel();
        pVMainLayout->addWidget(pPlateLabel);
        pPlateLabel->setText(QString("<b>Госномер: %1<\b>").arg(query.value(2).toString()));
        pPlateLabel->setStyleSheet("font-size: 16px;");

        if(query.value(1).toString().length()>1)
        {
            QLabel * pCommentLabel = new QLabel();
            pVMainLayout->addWidget(pCommentLabel);
            pCommentLabel->setText(QString("<b>Комментарий: %1<\b>").arg(query.value(4).toString()));
            pCommentLabel->setStyleSheet("font-size: 16px;");
        }

        QString strPartnerQuery = QString("select ЮЛ.Название from Партнеры, ЮЛ where Партнеры.ЮЛ = ЮЛ.id and Партнеры.id = '%1'").arg(query.value(5).toString());
        QSqlQuery PartnerQuery;
        PartnerQuery.exec(strPartnerQuery);
        while(PartnerQuery.next())
        {
            QLabel * pPartnerLabel = new QLabel();
            pVMainLayout->addWidget(pPartnerLabel);
            pPartnerLabel->setText(QString("<b>Партнер: %1<\b>").arg(PartnerQuery.value(0).toString()));
            pPartnerLabel->setStyleSheet("font-size: 16px;");
        }

        QString strTypesExec=QString("select \"Типы задач Оклейка\".Тип, \"Типы задач Оклейка\".Цена from \"Задача Оклейка - Типы\" , \"Типы задач Оклейка\"  where \"Типы задач Оклейка\".id = \"Задача Оклейка - Типы\".Тип and  \"Задача Оклейка - Типы\".Задача = '%1'").arg(strTaskUuid);
        QSqlQuery TypesQuery;
        QString strWorks;
        TypesQuery.exec(strTypesExec);
        while(TypesQuery.next())
        {
            QString strWork = QString(" %1 (%2 руб.)").arg(TypesQuery.value(0).toString()).arg(TypesQuery.value(1).toString());
            strWorks.append("\n ");
            strWorks.append(strWork);
            strWorks.append(";");
        }
        QLabel * pWorksLabel = new QLabel();
        pVMainLayout->addWidget(pWorksLabel);
        pWorksLabel->setWordWrap(true);
        pWorksLabel->setText(QString("<b>Работы: %1<\b>").arg(strWorks));
        pWorksLabel->setStyleSheet("font-size: 16px;");


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
