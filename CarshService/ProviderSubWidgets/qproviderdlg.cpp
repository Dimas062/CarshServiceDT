#include "qproviderdlg.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QSqlQuery>
#include <QPalette>



QProviderDlg::QProviderDlg(QUuid uuidProvider)
{

    m_uuidSourceProvider = uuidProvider;

    QVBoxLayout * pVMainLayout = new QVBoxLayout;
    this->setLayout(pVMainLayout);

    m_pNameLineText = new QLineText("Название");
    pVMainLayout->addWidget(m_pNameLineText);

    QPushButton * pULButton = new QPushButton("Реквизиты ЮЛ");
    connect(pULButton,SIGNAL(pressed()),this,SLOT(OnULPressed()));
    pVMainLayout->addWidget(pULButton);

    QPushButton * pApplyButton = new QPushButton("Применить");
    connect(pApplyButton,SIGNAL(pressed()),this,SLOT(OnApplyPressed()));
    pVMainLayout->addWidget(pApplyButton);


    if(m_uuidSourceProvider!=QUuid()) LoadFromBd();
}

void QProviderDlg::LoadFromBd()
{
    QString strQuery = QString("select Название, ЮЛ from Поставщики where id='%1'").arg(m_uuidSourceProvider.toString());
    QSqlQuery query;
    query.exec(strQuery);
    while(query.next())
    {
        m_pNameLineText->setText(query.value(0).toString());
        ulDlg.LoadFromBD(query.value(1).toString());
    }
}

void QProviderDlg::OnULPressed()
{
    ulDlg.exec();
}

void QProviderDlg::OnApplyPressed()
{
    if(m_uuidSourceProvider==QUuid())//Новый
    {
         QUuid ProviderUuid = QUuid::createUuid();

         QString strULUuid =  ulDlg.SaveToBD();

         QString strExec = QString("insert into Поставщики (id , Название , ЮЛ) values ('%1' , '%2' , '%3')").arg(ProviderUuid.toString()).arg(m_pNameLineText->getText()).arg(strULUuid);

         QSqlQuery query;

         query.exec(strExec);  

         accept();

         return;

    }
    else //Обновляем
    {
        ulDlg.SaveToBD();

        QString strExec;

        strExec = QString("update Поставщики set Название='%1' where id='%2'").arg(m_pNameLineText->getText()).arg(m_uuidSourceProvider.toString());

        QSqlQuery query;

        query.exec(strExec);

        accept();

        return;
    }
    QMessageBox::information(this , "Предупреждение" , "Для записи оператора введите название и выберите логотип.");
}
