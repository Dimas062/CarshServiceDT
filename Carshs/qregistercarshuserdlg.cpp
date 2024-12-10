#include "qregistercarshuserdlg.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSqlQuery>
#include <QString>
#include <QMessageBox>
#include <QUuid>
#include <QDateTime>
#include "common.h"
extern int iButtonHeight;

QRegisterCarshUserDlg::QRegisterCarshUserDlg()
{
    QVBoxLayout * pVMainLayout = new QVBoxLayout;
    this->setLayout(pVMainLayout);

    m_pLogoLabel = new QLabel();
    pVMainLayout->addWidget(m_pLogoLabel);

    QHBoxLayout * pCarshsHLayout = new QHBoxLayout;
    // QLabel * pCarshsComboLabel = new QLabel("Служба каршеринга*");
    // pCarshsHLayout->addWidget(pCarshsComboLabel);
    // m_pCarshsCombo = new QComboBox();
    // m_pCarshsCombo->setIconSize(QSize(100,30));
    // pCarshsHLayout->addWidget(m_pCarshsCombo);
    pVMainLayout->addLayout(pCarshsHLayout);

    m_pLoginLineText = new QLineText("Логин*:");
    pVMainLayout->addWidget(m_pLoginLineText);
    m_pPassLineText = new QLineText("Пароль*:");
    pVMainLayout->addWidget(m_pPassLineText);
    pVMainLayout->addSpacing(10);
    m_p1NameLineText = new QLineText("Фамилия*:");
    pVMainLayout->addWidget(m_p1NameLineText);
    m_p2NameLineText = new QLineText("Имя:");
    pVMainLayout->addWidget(m_p2NameLineText);
    m_p3NameLineText = new QLineText("Отчество:");
    pVMainLayout->addWidget(m_p3NameLineText);
    m_pPositionLineText = new QLineText("Должность:");
    pVMainLayout->addWidget(m_pPositionLineText);
    m_pMailLineText = new QLineText("e-mail:");
    pVMainLayout->addWidget(m_pMailLineText);
    m_pTelegrammLineText = new QLineText("Телеграмм:");
    pVMainLayout->addWidget(m_pTelegrammLineText);
    m_pPhoneLineText = new QLineText("Телефон*:");
    pVMainLayout->addWidget(m_pPhoneLineText);

    QPushButton * pRegisterButton = new QPushButton("Регистрация");
    connect(pRegisterButton,SIGNAL(pressed()),this,SLOT(OnRegisterPressed()));
    pRegisterButton->setMaximumHeight(iButtonHeight);
    pRegisterButton->setMinimumHeight(iButtonHeight);
    pVMainLayout->addWidget(pRegisterButton);

    // /*Заполнение комбобокс со службами */
    // QString strCarshsExec = "select id, Название, Логотип from Заказчики where Удалено=false";
    // QSqlQuery query;
    // query.exec(strCarshsExec);
    // while(query.next())
    // {
    //     QString strLogo(query.value(2).toString());

    //     QImage imgLogo = Base64ToImage(strLogo);

    //     QPixmap pxmpLogo = QPixmap::fromImage(imgLogo);

    //     m_pCarshsCombo->addItem(QIcon(pxmpLogo) , query.value(1).toString() , QVariant(query.value(0).toString()));
    // }

}

void QRegisterCarshUserDlg::OnRegisterPressed()
{
    /*Проверка заполенности полей*/
    if(m_pLoginLineText->getText().length()<1)
    {
        QMessageBox::information(this , "Регистрация" , "Введите логин.");
        return;
    }
    if(m_pPassLineText->getText().length()<1)
    {
        QMessageBox::information(this , "Регистрация" , "Введите пароль.");
        return;
    }
    if(m_p1NameLineText->getText().length()<1)
    {
        QMessageBox::information(this , "Регистрация" , "Введите Фамилию.");
        return;
    }
    if(m_pPhoneLineText->getText().length()<1)
    {
        QMessageBox::information(this , "Регистрация" , "Введите телефон.");
        return;
    }


    QUuid uuidUser = QUuid::createUuid();
    //QUuid uuidUserCarsh = QUuid::createUuid();

    QSqlQuery query;

    QString strExec = QString("insert into Пользователи (id , Фамилия, Имя, Отчество, Пароль, Логин, Роль, \"Дата регистрации\" , Телефон , Почта , Телеграм , Должность ) values ('%1','%2','%3','%4','%5','%6','ec3f998f-f5f4-4f2d-83a7-588934c58ecf','%7','%8','%9','%10' ,'%11')").arg(uuidUser.toString()).arg(m_p1NameLineText->getText()).arg(m_p2NameLineText->getText()).arg(m_p3NameLineText->getText()).arg(m_pPassLineText->getText()).arg(m_pLoginLineText->getText()).arg(QDateTime::currentSecsSinceEpoch()).arg(m_pPhoneLineText->getText()).arg(m_pMailLineText->getText()).arg(m_pTelegrammLineText->getText()).arg(m_pPositionLineText->getText());
    query.exec(strExec);


    QMessageBox::information(this , "Регистрация" , "Регистрационные денные отправлены.");

    accept();
}
