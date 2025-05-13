#include "qwashemplwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSqlQuery>
#include "common.h"
#include <QMessageBox>
#include <QVariant>
#include <QUuid>

extern int iButtonHeight;

QWashEmplWidget::QWashEmplWidget(QWidget *parent)
    : QWidget{parent}
{
    m_strCurUserId = QString("");

    QVBoxLayout * pVMainLayout = new QVBoxLayout;
    this->setLayout(pVMainLayout);

    QHBoxLayout * pHEmploeeLayout = new QHBoxLayout;

    m_pEmploeeListWidget = new QListWidget();

    pHEmploeeLayout->addWidget(m_pEmploeeListWidget);

    QVBoxLayout * pVCardDataLayout = new QVBoxLayout;


    QHBoxLayout * pActivHLayout = new QHBoxLayout;
    QLabel * pActivComboLabel = new QLabel("Активирована: ");
    pActivHLayout->addWidget(pActivComboLabel);
    m_pActivCombo = new QComboBox();
    pActivHLayout->addWidget(m_pActivCombo);
    pVCardDataLayout->addLayout(pActivHLayout);

    m_pLoginLineText = new QLineText("Логин*:");
    pVCardDataLayout->addWidget(m_pLoginLineText);
    m_pPassLineText = new QLineText("Пароль*:");
    pVCardDataLayout->addWidget(m_pPassLineText);
    pVCardDataLayout->addSpacing(10);
    m_p1NameLineText = new QLineText("Фамилия*:");
    pVCardDataLayout->addWidget(m_p1NameLineText);
    m_p2NameLineText = new QLineText("Имя:");
    pVCardDataLayout->addWidget(m_p2NameLineText);
    m_p3NameLineText = new QLineText("Отчество:");
    pVCardDataLayout->addWidget(m_p3NameLineText);
    m_pPhoneLineText = new QLineText("Телефон*:");
    pVCardDataLayout->addWidget(m_pPhoneLineText);

    QHBoxLayout *pButtonsLayout = new QHBoxLayout;


    QPushButton * pApplyCardButton = new QPushButton("Применить");
    connect(pApplyCardButton,SIGNAL(pressed()),this,SLOT(OnApplyCardPressed()));
    pApplyCardButton->setMaximumHeight(iButtonHeight);
    pApplyCardButton->setMinimumHeight(iButtonHeight);
    pButtonsLayout->addWidget(pApplyCardButton);

    QPushButton * pDeleteCardButton = new QPushButton("Удалить");
    connect(pDeleteCardButton,SIGNAL(pressed()),this,SLOT(OnDeleteCardPressed()));
    pDeleteCardButton->setMaximumHeight(iButtonHeight);
    pDeleteCardButton->setMinimumHeight(iButtonHeight);
    pButtonsLayout->addWidget(pDeleteCardButton);

    pVCardDataLayout->addLayout(pButtonsLayout);

    pHEmploeeLayout->addLayout(pVCardDataLayout);

    pVMainLayout->addLayout(pHEmploeeLayout);

    UpdateEmplList();

    connect(m_pEmploeeListWidget , SIGNAL(itemClicked(QListWidgetItem*)) , this , SLOT(EmplClicked(QListWidgetItem*)));

    /*Заполним компбо активно/не активно*/
    m_pActivCombo->addItem("Активна" , QVariant(true));
    m_pActivCombo->addItem("Не активна" , QVariant(false));
}

void QWashEmplWidget::UpdateEmplList()
{
    m_pEmploeeListWidget->clear();

    /*Заполним пользователей мойщиков*/
    QString strEmplQuery("select id , Фамилия, Имя, Отчество from Пользователи where Удалено<>true and Роль='773d9bea-12e1-4500-a149-2138ba284e6f'");
    QSqlQuery EmplQuery;
    EmplQuery.exec(strEmplQuery);
    while(EmplQuery.next())
    {
        QListWidgetItem * pItem = new QListWidgetItem;
        pItem->setText(QString("Мойщик %1 %2 %3").arg(EmplQuery.value(1).toString()).arg(EmplQuery.value(2).toString()).arg(EmplQuery.value(3).toString()));
        pItem->setData(Qt::UserRole , EmplQuery.value(0));
        m_pEmploeeListWidget->addItem(pItem);
    }

    /*Заполним пользователей администраторов*/
    strEmplQuery = QString("select id , Фамилия, Имя, Отчество from Пользователи where Удалено<>true and Роль='cfc94367-9ddf-4491-b01d-31d6984da9e6'");
    EmplQuery.exec(strEmplQuery);
    while(EmplQuery.next())
    {
        QListWidgetItem * pItem = new QListWidgetItem;
        pItem->setText(QString("Мойщик %1 %2 %3").arg(EmplQuery.value(1).toString()).arg(EmplQuery.value(2).toString()).arg(EmplQuery.value(3).toString()));
        pItem->setData(Qt::UserRole , EmplQuery.value(0));
        m_pEmploeeListWidget->addItem(pItem);
    }

    if(m_pEmploeeListWidget->count() > 0)
    {
        m_pEmploeeListWidget->setCurrentRow( 0 );
        EmplClicked(m_pEmploeeListWidget->item(0));
    }
}

void QWashEmplWidget::EmplClicked(QListWidgetItem* item)
{
    m_strCurUserId = item->data(Qt::UserRole).toString();

    /*Заполним пользователей*/
    QString strEmplQuery= QString("select id , Фамилия, Имя, Отчество, Логин, Пароль, Телефон , Подтвержден from Пользователи where id='%1'").arg(m_strCurUserId);
    QSqlQuery EmplQuery;
    EmplQuery.exec(strEmplQuery);
    while(EmplQuery.next())
    {
        //m_pCarshsCombo;
        m_pLoginLineText->setText(EmplQuery.value(4).toString());
        m_pPassLineText->setText(EmplQuery.value(5).toString());
        m_p1NameLineText->setText(EmplQuery.value(1).toString());
        m_p2NameLineText->setText(EmplQuery.value(2).toString());
        m_p3NameLineText->setText(EmplQuery.value(3).toString());
        m_pPhoneLineText->setText(EmplQuery.value(6).toString());

        /*Выберем активность/подтверждённость*/
        m_pActivCombo->setCurrentIndex(m_pActivCombo->findData(EmplQuery.value(7).toBool()));
    }
}

void QWashEmplWidget::OnDeleteCardPressed()
{
    if(m_strCurUserId.length()>1)
    {
        QSqlQuery query;

        QString strExec = QString("update Пользователи set Удалено = 'true' where id='%1'").arg(m_strCurUserId);
        query.exec(strExec);

        QMessageBox::information(this , "Пользователи" , "Пользователь удален.");
    }

    UpdateEmplList();
}

void QWashEmplWidget::OnApplyCardPressed()
{
    if(m_strCurUserId.length()>1)
    {
        QSqlQuery query;

        QString strExec = QString("update Пользователи set Фамилия = '%2', Имя='%3', Отчество='%4', Пароль='%5', Логин='%6', Подтвержден='%7' where id='%1'").arg(m_strCurUserId).arg(m_p1NameLineText->getText()).arg(m_p2NameLineText->getText()).arg(m_p3NameLineText->getText()).arg(m_pPassLineText->getText()).arg(m_pLoginLineText->getText()).arg(m_pActivCombo->currentData().toString());
        query.exec(strExec);

        QMessageBox::information(this , "Пользователи" , "Регистрационные денные обновлены.");
    }
}

