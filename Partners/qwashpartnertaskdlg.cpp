#include "qwashpartnertaskdlg.h"
#include "common.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QString>
#include <QUuid>
#include <QSqlQuery>
#include <QDateTime>
#include "qwashpartnerpenaltydlg.h"
#include "service/qdelopenpendlg.h"

extern int iUserType;


QWashPartnerTaskDlg::QWashPartnerTaskDlg(QString strTaskUuid)
{
    QVBoxLayout * pVMainLayout = new QVBoxLayout(this);
    this->setLayout(pVMainLayout);

    m_strTaskId = strTaskUuid;

    QLabel * pTopLabel = new QLabel();
    pVMainLayout->addWidget(pTopLabel);
    pTopLabel->setText("<b>Мойка<\b>");
    pTopLabel->setStyleSheet("font-size: 20px;");

    pVMainLayout->addSpacing(5);

    QString strExec = QString("select ДатаВремя, Точка, \"Время выполнения\" , Комментарий , Партнер from \"Задачи партнера Мойка\"  where id='%1'").arg(strTaskUuid);
    QSqlQuery query;
    query.exec(strExec);
    while(query.next())
    {
        QLabel * pDateTimeLabel = new QLabel();
        pVMainLayout->addWidget(pDateTimeLabel);
        pDateTimeLabel->setText("<b>" + QDateTime::fromSecsSinceEpoch(query.value(2).toInt()).toString("dd.MM.yyyy hh:mm") + "<\b>");
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



        if(query.value(3).toString().length()>1)
        {
            QLabel * pCommentLabel = new QLabel();
            pVMainLayout->addWidget(pCommentLabel);
            pCommentLabel->setText(QString("<b>Комментарий: %1<\b>").arg(query.value(3).toString()));
            pCommentLabel->setStyleSheet("font-size: 16px;");
        }

        QString strPartnerQuery = QString("select ЮЛ.Название from Партнеры, ЮЛ where Партнеры.ЮЛ = ЮЛ.id and Партнеры.id = '%1'").arg(query.value(4).toString());
        QSqlQuery PartnerQuery;
        PartnerQuery.exec(strPartnerQuery);
        while(PartnerQuery.next())
        {
            QLabel * pPartnerLabel = new QLabel();
            pVMainLayout->addWidget(pPartnerLabel);
            pPartnerLabel->setText(QString("<b>Партнер: %1<\b>").arg(PartnerQuery.value(0).toString()));
            pPartnerLabel->setStyleSheet("font-size: 16px;");
        }

        m_pTypesTableWidget = new QTableWidget;
        pVMainLayout->addWidget(m_pTypesTableWidget);
        connect(m_pTypesTableWidget , SIGNAL(cellDoubleClicked(int, int)) , this , SLOT(typesCellDoubleClicked(int, int)));


        m_pTypesTableWidget->setMinimumHeight(200);
        m_pTypesTableWidget->setMinimumWidth(600);

        UpdateTable();

        m_pPicturesWidget = new QPicturesWidget(nullptr , true , true);
        m_pPicturesWidget->setMinimumHeight(300);
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

void QWashPartnerTaskDlg::UpdateTable()
{
    QStringList headers;
    headers << "Вид работы" << "Время суток"<< "Количество" << "Цена"<<"Стоимость"<<"Отмена (кол-во)"<<"Отмена стоимость"<<"Итого";
    m_pTypesTableWidget->clear();
    m_pTypesTableWidget->clearSpans();
    m_pTypesTableWidget->setRowCount(0);
    m_pTypesTableWidget->setColumnCount(8);
    m_pTypesTableWidget->setHorizontalHeaderLabels(headers);

    QString strTypesExec=QString("select \"Типы задач Мойка\".Тип, \"Типы задач Мойка\".Цена , \"Задача Мойка - Типы\".Количество, \"Задача Мойка - Типы\".Ночь , \"Типы задач Мойка\".id from \"Задача Мойка - Типы\" , \"Типы задач Мойка\" where \"Типы задач Мойка\".id = \"Задача Мойка - Типы\".Тип and \"Задача Мойка - Типы\".Задача = '%1'").arg(m_strTaskId);
    QSqlQuery TypesQuery;
    QString strWorks;
    TypesQuery.exec(strTypesExec);

    m_pTypesTableWidget->setRowCount(TypesQuery.size() + 1);//+1 для итого

    int iRowCounter = 0;
    double dblSumm = 0;
    double dblPenSumm = 0;

    while(TypesQuery.next())
    {
        int iPenCount = 0;
        QUuid idPenWash = QUuid();
        QString strPenExec = QString("select \"Отмена Мойки\".id , \"Отмена Мойки\".Количество from \"Отмена Мойки\" where \"Отмена Мойки\".Задача='%1' and \"Отмена Мойки\".Тип='%2' and \"Отмена Мойки\".Ночь='%3' and \"Отмена Мойки\".Удалено=false").arg(m_strTaskId).arg(TypesQuery.value(4).toString()).arg(TypesQuery.value(3).toString());
        QSqlQuery penQuery;
        penQuery.exec(strPenExec);
        while(penQuery.next())
        {
            idPenWash = penQuery.value(0).toUuid();
            iPenCount = penQuery.value(1).toInt();
        }

        QTableWidgetItem * pItem = new QTableWidgetItem(TypesQuery.value(0).toString());
        pItem->setData(Qt::UserRole , TypesQuery.value(4));//Тип
        pItem->setData(Qt::UserRole + 1, TypesQuery.value(3));//Ночь
        pItem->setData(Qt::UserRole + 2, idPenWash); //Отмена/штраф
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pTypesTableWidget->setItem(iRowCounter , 0,  pItem);

        QString strDayTime;
        if(TypesQuery.value(3).toBool())
            strDayTime="Ночь";
        else strDayTime="День";

        pItem = new QTableWidgetItem(strDayTime);
        pItem->setData(Qt::UserRole , TypesQuery.value(4));
        pItem->setData(Qt::UserRole + 1, TypesQuery.value(3));
        pItem->setData(Qt::UserRole + 2, idPenWash); //Отмена/штраф
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pTypesTableWidget->setItem(iRowCounter , 1,  pItem);

        pItem = new QTableWidgetItem(TypesQuery.value(2).toString());
        pItem->setData(Qt::UserRole , TypesQuery.value(4));
        pItem->setData(Qt::UserRole + 1, TypesQuery.value(3));
        pItem->setData(Qt::UserRole + 2, idPenWash); //Отмена/штраф
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pTypesTableWidget->setItem(iRowCounter , 2,  pItem);

        pItem = new QTableWidgetItem(TypesQuery.value(1).toString());
        pItem->setData(Qt::UserRole , TypesQuery.value(4));
        pItem->setData(Qt::UserRole + 1, TypesQuery.value(3));
        pItem->setData(Qt::UserRole + 2, idPenWash); //Отмена/штраф
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pTypesTableWidget->setItem(iRowCounter , 3,  pItem);

        int iSumm = TypesQuery.value(1).toInt() * TypesQuery.value(2).toInt();

        pItem = new QTableWidgetItem(QString("%1").arg(iSumm));
        pItem->setData(Qt::UserRole , TypesQuery.value(4));
        pItem->setData(Qt::UserRole + 1, TypesQuery.value(3));
        pItem->setData(Qt::UserRole + 2, idPenWash); //Отмена/штраф
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pTypesTableWidget->setItem(iRowCounter , 4,  pItem);

        dblSumm = dblSumm + iSumm;

        //Количество штрафов
        pItem = new QTableWidgetItem(QString("%1").arg(iPenCount));
        pItem->setData(Qt::UserRole , TypesQuery.value(4));
        pItem->setData(Qt::UserRole + 1, TypesQuery.value(3));
        pItem->setData(Qt::UserRole + 2, idPenWash); //Отмена/штраф
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pTypesTableWidget->setItem(iRowCounter , 5,  pItem);

        int iPenSum = TypesQuery.value(1).toInt() * iPenCount;
        dblPenSumm = dblPenSumm + iPenSum;

        //Сумма штрафов
        pItem = new QTableWidgetItem(QString("%1").arg(iPenSum));
        pItem->setData(Qt::UserRole , TypesQuery.value(4));
        pItem->setData(Qt::UserRole + 1, TypesQuery.value(3));
        pItem->setData(Qt::UserRole + 2, idPenWash); //Отмена/штраф
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pTypesTableWidget->setItem(iRowCounter , 6,  pItem);

        //Итого
        pItem = new QTableWidgetItem(QString("%1").arg(iSumm - iPenSum));
        pItem->setData(Qt::UserRole , TypesQuery.value(4));
        pItem->setData(Qt::UserRole + 1, TypesQuery.value(3));
        pItem->setData(Qt::UserRole + 2, idPenWash); //Отмена/штраф
        pItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        m_pTypesTableWidget->setItem(iRowCounter , 7,  pItem);

        iRowCounter++;
    }

    /*Добавим Итого*/
    m_pTypesTableWidget->setSpan(iRowCounter , 0 , 1 , 5);
    QTableWidgetItem * pItem = new QTableWidgetItem(QString("Итого (с учетом вычетов): %1 руб.").arg(dblSumm - dblPenSumm));
    m_pTypesTableWidget->setItem(iRowCounter , 0,  pItem);
    m_pTypesTableWidget->resizeColumnsToContents();
}

void QWashPartnerTaskDlg::typesCellDoubleClicked(int r, int c)
{
    if(iUserType == Carsh) return;
    QTableWidgetItem * pItem= m_pTypesTableWidget->item(r ,c);
    if(pItem)
    {
        if(pItem->data(Qt::UserRole + 2).toUuid()!=QUuid()) //Если есть рекламация
        {
            QDelOpenPenDlg dlg;
            int iOpenRes = dlg.exec();
            if(iOpenRes == QDelOpenPenDlg::Open)
            {
                QString strIdType = pItem->data(Qt::UserRole).toString();
                bool bNigth = pItem->data(Qt::UserRole + 1).toBool();

                QWashPartnerPenaltyDlg dlg(m_strTaskId , strIdType ,bNigth);
                if(QDialog::Accepted == dlg.exec()) UpdateTable();
            }
            if(iOpenRes == QDelOpenPenDlg::Del)
            {
                QString strExec = QString("update \"Отмена Мойки\" set Удалено = 'true' where id='%1'").arg(pItem->data(Qt::UserRole + 2).toString());
                QSqlQuery query;
                query.exec(strExec);
            }
        }
        else//Если нет рекламации, то откроем окно создания
        {
            QString strIdType = pItem->data(Qt::UserRole).toString();
            bool bNigth = pItem->data(Qt::UserRole + 1).toBool();

            QWashPartnerPenaltyDlg dlg(m_strTaskId , strIdType ,bNigth);
            if(QDialog::Accepted == dlg.exec()) UpdateTable();
        }
        UpdateTable();
    }
}
