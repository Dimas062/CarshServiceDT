#include "qproviderdlg.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QSqlQuery>
#include <QPalette>
#include "common.h"


QProviderDlg::QProviderDlg(QUuid uuidProvider)
{

    m_uuidSourceProvider = uuidProvider;

    QVBoxLayout * pVMainLayout = new QVBoxLayout;
    this->setLayout(pVMainLayout);

    m_pNameLineText = new QLineText("Название");
    pVMainLayout->addWidget(m_pNameLineText);

    QPushButton * pApplyButton = new QPushButton("Применить");
    connect(pApplyButton,SIGNAL(pressed()),this,SLOT(OnApplyPressed()));
    pVMainLayout->addWidget(pApplyButton);


    if(m_uuidSourceProvider!=QUuid()) LoadFromBd();
}

void QProviderDlg::LoadFromBd()
{
    QString strQuery = QString("select Название from Поставщики where id='%1'").arg(m_uuidSourceProvider.toString());
    QSqlQuery query;
    query.exec(strQuery);
    while(query.next())
    {
        m_pNameLineText->setText(query.value(0).toString());
    }
}



void QProviderDlg::OnApplyPressed()
{
    if(m_uuidSourceProvider==QUuid())//Новый
    {
         QUuid ProviderUuid = QUuid::createUuid();

         QString strExec = QString("insert into Поставщики (id , Название) values ('%1' , '%2')").arg(ProviderUuid.toString()).arg(m_pNameLineText->getText());

         QSqlQuery query;

         query.exec(strExec);

         accept();

         return;

    }
    else //Обновляем
    {
        QString strExec;

        strExec = QString("update Поставщики set Название='%1' where id='%2'").arg(m_pNameLineText->getText()).arg(m_uuidSourceProvider.toString());

        QSqlQuery query;

        query.exec(strExec);

        accept();

        return;
    }
    QMessageBox::information(this , "Предупреждение" , "Для записи оператора введите название и выберите логотип.");
}
