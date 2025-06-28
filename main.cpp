//#include "qmaindialog.h"
#ifdef Q_OS_WINDOWS
#include <windows.h>
#endif
//#include <QApplication>
#include <QtWidgets/QApplication>
#include <QUuid>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include "qlogindlg.h"
#include "qcarshservicemaindlg.h"
#include "Carshs/qcarshmaindlg.h"
#include "common.h"
#include <QSplashScreen>
#include "qscreen.h"

QRect screenGeometry;

QUuid uuidCurrentUser;
QString strCurrentZakazId;

int iButtonHeight = 50;
int iUserType = 0;

QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                   "dimas062",
                   "CarshServiceDT");

QT_USE_NAMESPACE



// #ifdef Q_OS_WINDOWS
// extern "C" {
// __declspec(dllexport) DWORD GetCurrentPackageFullName(UINT32*, PWSTR) {
//     return 0x80073D54; // Ошибка "Пакет не найден" для классических приложений
// }
// }
// #endif

int main(int argc, char *argv[])
{
    qputenv("QT_ASSUME_STDERR_HAS_CONSOLE", "1");

    QApplication a(argc, argv);

    QPixmap pixmap(":/icons/Splash.png");
    QSplashScreen splash(pixmap);
    splash.show();
    splash.showMessage("Подключение к БД");

    screenGeometry = QRect(qApp->screens()[0]->size().width()/8,0,qApp->screens()[0]->size().width() - qApp->screens()[0]->size().width()/4 ,qApp->screens()[0]->size().height());

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");

    db.setHostName("188.243.205.147");
    db.setDatabaseName("CarshService");
    db.setPassword("VthctltcWKR200");
    db.setUserName("postgres");
    db.setPort(5432);

    // Установка параметров keepalive
    db.setConnectOptions(
        "keepalives=1;"
        "keepalives_idle=30;"    // Время до первого keepalive (сек)
        "keepalives_interval=10;"// Интервал между проверками (сек)
        "keepalives_count=5;"    // Количество попыток
        "connect_timeout=15"     // Таймаут подключения (опционально)
        );

    if(!db.open())
    {
        QMessageBox::information(NULL , "Злобин Каршеринг сервис", QString("Не удаётся соединиться с сервером: %1").arg(db.lastError().text()) , "Закрыть");
        return 0;
    }

    OpenServerBD();//Соединение с БД через прокси для мобилок для диалогов с мобилок

    QThread::msleep(200);

    splash.showMessage("Загрузка модулей");

    QThread::msleep(300);
    splash.close();

    QLoginDlg loginDlg;
    iUserType = loginDlg.exec();

    if(iUserType == 0) return 0;

    if(iUserType == CarshService)
    {
        QCarshServiceMainDlg dlg;
        dlg.show();

        return a.exec();
    }
    if(iUserType == Carsh)
    {
        strCurrentZakazId = loginDlg.m_strLastLoginedCarshId;
        QCarshMainDlg dlg;
        dlg.show();

        return a.exec();
    }

    return a.exec();
}
