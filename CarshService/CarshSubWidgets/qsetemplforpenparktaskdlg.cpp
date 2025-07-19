#include "qsetemplforpenparktaskdlg.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QSqlQuery>
#include <QMessageBox>
#include "BDPatterns.h"

QSetEmplForPenParkTaskDlg::QSetEmplForPenParkTaskDlg(QUuid penTaskId)
{
    QVBoxLayout * pVMainLayout = new QVBoxLayout;
    m_uuidPenTask = penTaskId;

    m_pEmploeeListWidget = new QListWidget();
    pVMainLayout->addWidget(m_pEmploeeListWidget);

    /*Заполним пользователей*/
    QString strEmplQuery("select id , Фамилия, Имя, Отчество from Пользователи where Удалено<>true and Роль='80066f83-c025-410b-b439-f3e9b2299461'");
    QSqlQuery EmplQuery;
    EmplQuery.exec(strEmplQuery);
    while(EmplQuery.next())
    {
        QListWidgetItem * pItem = new QListWidgetItem;
        pItem->setText(QString("%1 %2 %3").arg(EmplQuery.value(1).toString()).arg(EmplQuery.value(2).toString()).arg(EmplQuery.value(3).toString()));
        pItem->setData(Qt::UserRole , EmplQuery.value(0));
        m_pEmploeeListWidget->addItem(pItem);
    }

    connect(m_pEmploeeListWidget , SIGNAL(itemDoubleClicked(QListWidgetItem*)) , this , SLOT(OnEmplDblClicked(QListWidgetItem*)));

    QPushButton * pSetEmplButton = new QPushButton("Назначить");
    connect(pSetEmplButton,SIGNAL(pressed()),this,SLOT(OnSetEmplPressed()));
    pVMainLayout->addWidget(pSetEmplButton);

    QPushButton * pCancelButton = new QPushButton("Отменить");
    connect(pCancelButton,SIGNAL(pressed()),this,SLOT(reject()));
    pVMainLayout->addWidget(pCancelButton);


    this->setLayout(pVMainLayout);
}

void QSetEmplForPenParkTaskDlg::SetForEmpl(QUuid uuidEmpl)
{
    QString strQuery = QString("select Штрафстоянка, Госномер, ДатаВремяПомещения , Заказчик from ЗадачиЗаказчикаШС where id='%1'").arg(m_uuidPenTask.toString());
    qDebug()<<"QSetEmplForPenParkTaskDlg "<<strQuery;
    QSqlQuery query;
    query.exec(strQuery);
    while(query.next())
    {
        QString strPenParkUuid = query.value(0).toString();
        QString strGrz = query.value(1).toString();
        //QString strPenParkDateTime = query.value(2).toString();
        QString strCarshServiceUuid = query.value(3).toString();

        QSqlQuery queryForAccept;

        //Создадим новую задачу
        QString strNewTaskUuid = QUuid::createUuid().toString();
        QString strNewTaskExtensionUuid = QUuid::createUuid().toString();
        QString strExec= QString("insert into Задачи (id , \"Дата Время\" , Тип , Расширение, Исполнитель, Заказчик) values ('%1' , '%2' ,'8078b7ce-e423-49ae-9ce6-17758b852b33' ,'%3' ,'%4' ,'%5')").arg(strNewTaskUuid).arg(QDateTime::currentSecsSinceEpoch()).arg(strNewTaskExtensionUuid).arg(uuidEmpl.toString()).arg(strCarshServiceUuid);
        queryForAccept.exec(strExec);


        QUuid uuidPay = CreatePayRecord(0 , PayTypes::Undefined , QDateTime::currentSecsSinceEpoch());

        strExec= QString("insert into \"Расширение задачи ШС\" (id , Госномер , Штрафстоянка , \"Оплата парковки\") values ('%1' , '%2' , '%3' , '%4')").arg(strNewTaskExtensionUuid).arg(strGrz).arg(strPenParkUuid).arg(uuidPay.toString());
        queryForAccept.exec(strExec);

        //Обновим заявку, установив текущего пользователя взявшим
        strExec= QString("Update ЗадачиЗаказчикаШС set ПереведенаВЗадачу='%1' where id='%2'").arg(strNewTaskUuid).arg(m_uuidPenTask.toString());
        queryForAccept.exec(strExec);

        accept();
        return;

    }
    qDebug()<<"Ошибка Error Назначение задачи ШС. Не найдена задача ШС заказчика";
    QMessageBox::warning(this , "CarshService" , "Задача заказчика Штрафстоянка не найдена.");
    reject();
}

void QSetEmplForPenParkTaskDlg::OnSetEmplPressed()
{
    if(m_pEmploeeListWidget->selectedItems().size() > 0)
    {
        QListWidgetItem* item  = m_pEmploeeListWidget->selectedItems()[0];
        if(item)
            SetForEmpl(item->data(Qt::UserRole).toUuid());
    }
}

void QSetEmplForPenParkTaskDlg::OnEmplDblClicked(QListWidgetItem* item)
{
    SetForEmpl(item->data(Qt::UserRole).toUuid());
}
