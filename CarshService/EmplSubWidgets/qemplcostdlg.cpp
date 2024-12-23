#include "qemplcostdlg.h"
#include "common.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QString>
#include <QUuid>
#include <QSqlQuery>
#include <QDateTime>

QEmplCostDlg::QEmplCostDlg(QString strCostId)
{
    QVBoxLayout * pVMainLayout = new QVBoxLayout(this);
    this->setLayout(pVMainLayout);

    m_sourceCost = strCostId;

    QLabel * pTopLabel = new QLabel();
    pVMainLayout->addWidget(pTopLabel);
    pTopLabel->setText("<b>Расход<\b>");
    pTopLabel->setStyleSheet("font-size: 20px;");

    pVMainLayout->addSpacing(5);

    QString strExec= QString("SELECT Расходы.\"Дата Время\" , \"Платежи сотрудников\".Сумма , Товары.Товар , \"Статьи расходов\".\"Статья расходов\"  , Пользователи.Фамилия, Пользователи.Имя, Пользователи.Отчество , Расходы.Оплата FROM Расходы, \"Платежи сотрудников\", Товары , Пользователи , \"Статьи расходов\"  where Расходы.Удалено<>'true' and Расходы.Товар=Товары.id and Расходы.Оплата=\"Платежи сотрудников\".id and \"Статьи расходов\".id=Расходы.Статья  and Пользователи.id=Расходы.Сотрудник and Расходы.Удалено <> true and Расходы.id = '%1'").arg(strCostId);

    QSqlQuery query;
    query.exec(strExec);
    while(query.next())
    {
        QLabel * pFioLabel = new QLabel();
        pVMainLayout->addWidget(pFioLabel);
        pFioLabel->setText(QString("<b>Сотрудник: %1 %2 %3<\b>").arg(query.value(4).toString()).arg(query.value(5).toString()).arg(query.value(6).toString()));
        pFioLabel->setStyleSheet("font-size: 16px;");

        QLabel * pDateTimeLabel = new QLabel();
        pVMainLayout->addWidget(pDateTimeLabel);
        pDateTimeLabel->setText("<b>" + QDateTime::fromSecsSinceEpoch(query.value(0).toInt()).toString("dd.MM.yyyy hh:mm") + "<\b>");
        pDateTimeLabel->setStyleSheet("font-size: 16px;");

        if(query.value(1).toString().length()>1)
        {
            QLabel * pSummLabel = new QLabel();
            pVMainLayout->addWidget(pSummLabel);
            pSummLabel->setText(QString("<b>Сумма: %1 руб.<\b>").arg(query.value(1).toString()));
            pSummLabel->setStyleSheet("font-size: 16px;");
        }

        if(query.value(2).toString().length()>1)
        {
            QLabel * pItemLabel = new QLabel();
            pVMainLayout->addWidget(pItemLabel);
            pItemLabel->setText(QString("<b>Товар: %1<\b>").arg(query.value(2).toString()));
            pItemLabel->setStyleSheet("font-size: 16px;");
        }


        if(query.value(3).toString().length()>1)
        {
            QLabel * pStatiaLabel = new QLabel();
            pVMainLayout->addWidget(pStatiaLabel);
            pStatiaLabel->setText(QString("<b>Статья расходов: %1<\b>").arg(query.value(3).toString()));
            pStatiaLabel->setStyleSheet("font-size: 16px;");
        }

        m_pPicturesWidget = new QPicturesWidget(nullptr , true , true);
        m_pPicturesWidget->setMinimumHeight(400);
        pVMainLayout->addWidget(m_pPicturesWidget);

        QString strPicsExec = QString("select Изображение from Документы where id in (select Документ from \"Платеж сотрудника - Документы\" where Платеж = '%1')").arg(query.value(7).toString());
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
