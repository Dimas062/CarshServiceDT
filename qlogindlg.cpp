#include "qlogindlg.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QUuid>
#include <QSqlQuery>
//#include "qcarshservicemaindlg.h"
#include "common.h"
#include "qregisterdlg.h"

extern QUuid uuidCurrentUser;
extern int iButtonHeight;

QLoginDlg::QLoginDlg()
{

    uuidCurrentUser = QUuid();

    int iUnderButtonSpace = 15;



    QVBoxLayout * pVMainLayout = new QVBoxLayout(this);
    this->setLayout(pVMainLayout);

    pVMainLayout->addSpacing(5);

    pVMainLayout->setContentsMargins(20,20,20,20);

    this->setMinimumWidth(300);

    QLabel * pLogoLabel = new QLabel();
    QPixmap topIconPixmap(":/icons/CarshServiceIcon.png");
    pLogoLabel->setPixmap(topIconPixmap.scaled(100,100 ,Qt::IgnoreAspectRatio , Qt::SmoothTransformation));
    pLogoLabel->setAlignment(Qt::AlignCenter);

    pVMainLayout->addWidget(pLogoLabel);

    QLabel * pLogoTextLabel = new QLabel ("<h1>Злобин Каршеринг Сервис<\h1>");
    pLogoTextLabel->setAlignment(Qt::AlignCenter);
    pVMainLayout->addWidget(pLogoTextLabel);

    pVMainLayout->addSpacing(20);

    m_pStatusLabel = new QLabel(this);
    m_pStatusLabel->setAlignment(Qt::AlignCenter);
    pVMainLayout->addWidget(m_pStatusLabel);

    pVMainLayout->addStretch();

    QLabel * pLoginLabel = new QLabel("Логин");
    pVMainLayout->addWidget(pLoginLabel);

    m_pLoginLineEdit = new QLineEdit;
    pVMainLayout->addWidget(m_pLoginLineEdit);

    QLabel * pPasswordLabel = new QLabel("Пароль");
    pVMainLayout->addWidget(pPasswordLabel);


    m_pPasswordLineEdit = new QLineEdit;
    pVMainLayout->addWidget(m_pPasswordLineEdit);

    pVMainLayout->addStretch();

    pVMainLayout->addSpacing(iUnderButtonSpace);
    QPushButton * pLoginButton = new QPushButton("Вход");
    connect(pLoginButton,SIGNAL(pressed()),this,SLOT(OnLoginPressed()));
    pLoginButton->setMaximumHeight(iButtonHeight);
    pLoginButton->setMinimumHeight(iButtonHeight);
    pVMainLayout->addWidget(pLoginButton);


    pVMainLayout->addSpacing(iUnderButtonSpace);
    QPushButton * pRegisterButton = new QPushButton("Регистрация");
    connect(pRegisterButton,SIGNAL(pressed()),this,SLOT(OnRegisterPressed()));
    pRegisterButton->setMaximumHeight(iButtonHeight);
    pRegisterButton->setMinimumHeight(iButtonHeight);
    pVMainLayout->addWidget(pRegisterButton);
}


void QLoginDlg::OnLoginPressed()
{

    /*Поиск пользователя с заданным логином/паролем*/
    QString strUserExec = QString("select id , \"Роль\" , \"Подтвержден\" from Пользователи where Логин='%1' and Пароль='%2'").arg(m_pLoginLineEdit->text()).arg(m_pPasswordLineEdit->text());

    QSqlQuery query;

    query.exec(strUserExec);



    while(query.next())
    {
         uuidCurrentUser = query.value(0).toUuid();

        if(query.value(2).toBool() == false)
        {
            m_pStatusLabel->setText("<font color=\"red\">Ваша учётная запись не подтверждена или заблокирована</font>");
            return;
        }
        else m_pStatusLabel->setText(" ");



        if(query.value(1).toUuid() == QUuid("80d4f275-0b41-40d5-b3d7-07f63a500a22")) //Каршсервис
        {
            done(CarshService);
        }
        if(query.value(1).toUuid() == QUuid("ec3f998f-f5f4-4f2d-83a7-588934c58ecf")) //Служба каршеринга
        {

            done(Carsh);
        }
        if(query.value(1).toUuid() == QUuid("512c50c1-c4a9-4542-932a-55280886715a")) //Партнер номера
        {

            done(PartnerPlate);
        }
        if(query.value(1).toUuid() == QUuid("4c476883-76b5-4f28-823a-966d69f51d46")) //Партнер оклейка
        {

            done(PartnerStick);
        }
        if(query.value(1).toUuid() == QUuid("184f8f60-a865-4bcf-996e-b26ff21d1ee3")) //Партнер мойка
        {

            done(PartnerWasher);
        }
        if(query.value(1).toUuid() == QUuid("80066f83-c025-410b-b439-f3e9b2299461")) //Сотрудник
        {
            done(Emploee);
        }
    }

}

void QLoginDlg::OnRegisterPressed()
{
     QRegisterDlg dlg;

     dlg.exec();
}


void QLoginDlg::OnSettingsModePressed()
{
    //m_bChildBackRealeseProcessed = false;
    // QSettingsDlg settDlg;
    // int dlgRetVal = settDlg.exec();
    // //m_bChildBackRealeseProcessed = settDlg.m_bBackProcessed;
    // if(dlgRetVal == QDialog::Accepted)
    // {
    //     //  QMetaObject::invokeMethod(&InteractionClient::getInstance() , "disconnect");//Переподключимся
    // }

}
