#include "qlogindlg.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QUuid>
#include <QSqlQuery>
#include <QDebug>
//#include "qcarshservicemaindlg.h"
#include "common.h"
#include "qregisterdlg.h"


extern QUuid uuidCurrentUser;
extern UserTypes CurrentUserType;
extern int iButtonHeight;



#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <QMap>
#include <QString>
#include <QUuid>
#include <QFile>

struct TaskReport {
    QString taskType;
    QString details;
    QDateTime completionDate;
};

QString generateMonthlyReport(int year, int month) {
    // Проверка валидности входных параметров
    if (month < 1 || month > 12) return "Некорректный месяц";
    if (year < 2000 || year > 2100) return "Некорректный год";

    QSqlQuery query;
    QString sql =
        "SELECT "
        "   u.\"Фамилия\", u.\"Имя\", u.\"Отчество\", "
        "   t.\"Тип\", "
        "   z.\"Дата Время\" AS task_date, "
        "   COALESCE( "
        "       rp.\"Госномер\", "
        "       rs.\"Госномер\", "
        "       rn.\"Госномер\", "
        "       rz.\"Госномер\", "
        "       '' "
        "   ) AS license_plate "
        "FROM \"Задачи\" z "
        "JOIN \"Пользователи\" u ON z.\"Исполнитель\" = u.id "
        "JOIN \"Типы задач\" t ON z.\"Тип\" = t.id "
        "LEFT JOIN \"Расширение задачи Парковка\" rp ON z.\"Расширение\" = rp.id AND z.\"Тип\" = '057b3b6f-2848-479b-a546-3f16cb531ffe' "
        "LEFT JOIN \"Расширение задачи ШС\" rs ON z.\"Расширение\" = rs.id AND z.\"Тип\" = '8078b7ce-e423-49ae-9ce6-17758b852b33' "
        "LEFT JOIN \"Расширение задачи Номера\" rn ON z.\"Расширение\" = rn.id AND z.\"Тип\" = '99b4e860-5a7b-42a4-9136-f96252ef4192' "
        "LEFT JOIN \"Расширение задачи Возврат в зону\" rz ON z.\"Расширение\" = rz.id AND z.\"Тип\" = 'fe81daf9-a838-4bac-84aa-595e038d3a12' "
        "WHERE z.\"Время выполнения\" > 0 "
        "   AND EXTRACT(YEAR FROM TO_TIMESTAMP(z.\"Дата Время\")) = :year "
        "   AND EXTRACT(MONTH FROM TO_TIMESTAMP(z.\"Дата Время\")) = :month "
        "   AND z.\"Удалено\" = false "
        "   AND u.\"Удалено\" = false "
        "ORDER BY u.\"Фамилия\", u.\"Имя\", u.\"Отчество\", z.\"Дата Время\";";

    query.prepare(sql);
    query.bindValue(":year", year);
    query.bindValue(":month", month);

    if (!query.exec()) {
        return "Ошибка выполнения запроса: " + query.lastError().text();
    }

    QMap<QString, QList<TaskReport>> reportData;

    while (query.next()) {
        TaskReport task;
        QString user = QString("%1 %2 %3")
                           .arg(query.value("Фамилия").toString())
                           .arg(query.value("Имя").toString())
                           .arg(query.value("Отчество").toString());

        task.taskType = query.value("Тип").toString();
        QString licensePlate = query.value("license_plate").toString().trimmed();

        qint64 timestamp = query.value("task_date").toLongLong();
        task.completionDate = QDateTime::fromSecsSinceEpoch(timestamp);

        task.details = task.taskType;
        if (!licensePlate.isEmpty()) {
            task.details += " [" + licensePlate + "]";
        }

        reportData[user].append(task);
    }

    QString report;
    for (auto it = reportData.begin(); it != reportData.end(); ++it) {
        report += "Сотрудник: " + it.key() + "\n";

        // Группировка по датам
        QMap<QDate, QStringList> dailyTasks;
        for (const TaskReport& task : it.value()) {
            QString dateStr = task.completionDate.toString("dd.MM.yyyy");
            dailyTasks[task.completionDate.date()] << ("- " + task.details);
        }

        // Сортировка дат
        QList<QDate> dates = dailyTasks.keys();
        std::sort(dates.begin(), dates.end());

        // Формирование вывода
        for (const QDate& date : dates) {
            report += date.toString("dd.MM.yyyy") + ":\n";
            report += dailyTasks[date].join("\n") + "\n\n";
        }
        report += "────────────────────\n\n";
    }

    return report.isEmpty() ? "Нет данных за указанный период" : report;
}




QLoginDlg::QLoginDlg()
{
    // QString str = generateMonthlyReport(2025,5);
    // qDebug()<<"report len="<<str.length();
    // const QString qPath("/Users/dimas/Documents/forTlg.txt");
    // QFile qFile(qPath);
    // if (qFile.open(QIODevice::WriteOnly)) {
    //     QTextStream out(&qFile); out << str;
    //     qFile.close();
    // }



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
            CurrentUserType =UserTypes::CarshService;
            done(CarshService);
        }
        if(query.value(1).toUuid() == QUuid("ec3f998f-f5f4-4f2d-83a7-588934c58ecf")) //Служба каршеринга
        {
            QString strZakazIdQuery = QString("select Заказчик from \"Заказчик-Пользователи\" where Пользователь = '%1'").arg(uuidCurrentUser.toString());
            QSqlQuery queryZakaz;

            queryZakaz.exec(strZakazIdQuery);
            while(queryZakaz.next())
            {
                m_strLastLoginedCarshId = queryZakaz.value(0).toString();
            }

            //strLastLoginedCarsId
            CurrentUserType =UserTypes::Carsh;
            done(Carsh);
        }
        if(query.value(1).toUuid() == QUuid("512c50c1-c4a9-4542-932a-55280886715a")) //Партнер номера
        {
            CurrentUserType =UserTypes::PartnerPlate;
            done(PartnerPlate);
        }
        if(query.value(1).toUuid() == QUuid("4c476883-76b5-4f28-823a-966d69f51d46")) //Партнер оклейка
        {
            CurrentUserType =UserTypes::PartnerStick;
            done(PartnerStick);
        }
        if(query.value(1).toUuid() == QUuid("184f8f60-a865-4bcf-996e-b26ff21d1ee3")) //Партнер мойка
        {
            CurrentUserType =UserTypes::PartnerWasher;
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
